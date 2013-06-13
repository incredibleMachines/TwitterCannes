#include "testApp.h"

#define USE_DOF true
#define CAM_MOVE false
#define CAM_MOUSE true
#define FACES false


//--------------------------------------------------------------
void testApp::setup(){
    
    // in bin, not data
    sqlite = new ofxSQLite("twitterCannesLions.db");
    
    gotham.setup();
    loadSQL();
    
    bDebug=false;
    bGUI=false;
    bShadowsOn=true;
    
    camState=0;
    
    camera.setDistance(40.0f);
    camera.setGlobalPosition( 30.0f, 15.0f, 0.0f );
    camera.lookAt( ofVec3f( 0.0f, 0.0f, 0.0f ) );

    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
    
    //camera, lighting, dof setup
    setupGL();
    
    if (USE_DOF) {
        dof.setup(ofGetWidth(),ofGetHeight());
        dof.setFocalDistance(10);
        dof.setFocalRange(20);
        dof.setBlurAmount(1);
    }
    
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
    }
     
    shader.load( "shaders/mainScene.vert", "shaders/mainScene.frag" );
    
    setupLights();
    
    //GUI and hashtag mesh loading
    loadHashtag();
    
    list[0].text="100000000d0ddfaskjdhflkajsh asdkfjhaslkdjfh";
    
    tweet.loadTweet(list[0], &world, &gotham);
    
    
}


void testApp::loadSQL(){
        ofxSQLiteSelect sel = sqlite->select("user_image, user_name, user_screen_name, text, media_url")
        .from("tweets")
        .execute().begin();
    db newList;
    
        while (sel.hasNext()) {
            newList.user_image = sel.getString();
            newList.user_name = sel.getString();
            newList.user_screen_name = sel.getString();
            newList.text = sel.getString();
            newList.media_url = sel.getString();
            list.push_back(newList);
            sel.next();
    }
                listCount=0;
}
//--------------------------------------------------------------
void testApp::drawObjects(){
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
//    ofScale(6,6,6);
    ofRotate(90, 0, 1, 0);
    ofRotate(330,1,0,0);
    
    for(int i = 0; i < hashCollision.size()  ; i++) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        glDisable(GL_CULL_FACE);
        ofVec3f scale=hashletters[i].scale;
        btScalar	m[16];
        ofGetOpenGLMatrixFromRigidBody( hashCollision[i]->getRigidBody(), m );
        glPushMatrix();
        ofSetColor(255,255,255);
        glMultMatrixf( m );
        glTranslatef(-hashletters[i].size.x*scale.x/2, -hashletters[i].size.y*scale.y/2, -hashletters[i].size.z*scale.z/2);
        ofScale(scale.x,scale.y,scale.z);
        if(bGUI==true&&hashletters[i].active==true){
            ofSetColor(0,255,0);
            ofSphere(hashMeshes[i].getCentroid().x,hashMeshes[i].getCentroid().y+30,hashMeshes[i].getCentroid().z,10);
        }
        hashMeshes[i].draw();
        ofSetColor(255,255,255);
        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();
    }
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    ofSetWindowTitle( ofToString( ofGetFrameRate() ) );
    
//updates bullet objects
    world.update();
    
//    cout<<list.size()<<endl;
    
    if(tweet.bFinished==true&&tweet.image.bFinished==true&&tweet.user.bFinished==true){
        tweet.destroy();
        listCount++;
        if(listCount>list.size()-1){
            listCount=0;
        }
        
        tweet.loadTweet(list[listCount], &world, &gotham);
        
        cout<<"destroy"<<endl;
    }
    else{
        tweet.update();
    }
    
   
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
	glEnable(GL_DEPTH_TEST);
    ofDisableAlphaBlending();
    ofEnableLighting();
    ofDisableArbTex();
    ofPushMatrix();
        
    float shadowX = ofMap(mouseX, 0, ofGetWidth(), -100, 100);
    float shadowY = ofMap(mouseY, 0, ofGetHeight(), -100, 100);
    
    shadowLightLeft.lookAt( ofVec3f(0.0,0.0,0.0) );
    shadowLightLeft.orbit( 90, -70, 90, ofVec3f(0.0,0.0,0.0) );
//    shadowLightLeft.orbit( 90, -80, 90, ofVec3f(0.0,0.0,0.0) );
//    shadowLightLeft.orbit( shadowX, shadowY, 90, ofVec3f(0.0,0.0,0.0) );

    
//    cout << shadowY << " " << shadowX << endl;

    // lat, long, rad, center

    // render linear depth buffer from light view
    shadowLightLeft.beginShadowMap();
    ofPushMatrix();
    ofScale(0.3, 0.3, 0.3);

    drawObjects(); // render to shader map
    
    if(bShadowsOn){
        tweet.draw();
    }
    
    ofPopMatrix();

    shadowLightLeft.endShadowMap();
    
    // render final scene
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    shader.begin();
    
    shadowLightLeft.bindShadowMapTexture(0); // bind shadow map texture to unit 0
    shader.setUniform1i("u_ShadowMap", 0); // set uniform to unit 0
    shader.setUniform1f("u_LinearDepthConstant", shadowLightLeft.getLinearDepthScalar()); // set near/far linear scalar
    shader.setUniformMatrix4f("u_ShadowTransMatrix", shadowLightLeft.getShadowMatrix(camera)); // specify shadow matrix
    
    camera.begin();
    
    shadowLightLeft.enable();
    
    // draw background for projecting shadows onto
    ofPushMatrix();
    ofScale(6,6,6);
    ofRotate(90, 0, 1, 0);
    ofRotate(330,1,0,0);
    ofRect(-15, -15, 30, 30);
    ofPopMatrix();
    
    ofPushMatrix();
    ofScale(0.3, 0.3, 0.3);
    
    material.begin();
    
    drawObjects(); // render to screen
   
    material.end();
        
    //bind image textures and draw bullet shapes
    
    material.begin();   
    tweet.draw();
    material.end();
    

    
    shadowLightLeft.disable();
    
    //enable to see physics collision wireframes
    if(bDebug==true){
        glPushMatrix();
        world.drawDebug();
        glPopMatrix();
    }
    ofPopMatrix();

    
    camera.end();
    
    shadowLightLeft.unbindShadowMapTexture();

    shader.end();
    
    
    ofDisableLighting();
    ofPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    
    if(bGUI==true){
        drawGUI();
    }
    
}



//--------------------------------------------------------------
void testApp::setupGL(){    
    
    material.setShininess(.8);
    
    //Create static collision objects
    background.create(world.world,ofVec3f(0,0,-90.),0.,175,200,10.);
    background.add();
    background.setProperties(.1,1);
    
    whiteImg.loadImage("textures/whiteBig.png");
    white=whiteImg.getTextureReference();
}

//--------------------------------------------------------------
void testApp::setupLights() {
    
    // shadow map resolution (must be power of 2), field of view, near, far
    // the larger the shadow map resolution, the better the detail, but slower
    shadowLightLeft.setup( 2048, 60.0f, 0.1f, 100.0f );
    shadowLightLeft.setBlurLevel(10.0f); // amount to blur to soften the shadows
    
    shadowLightLeft.setAmbientColor( ofFloatColor( 0.1f, 0.1f, 0.1f, 1.0f ) );
    shadowLightLeft.setDiffuseColor( ofFloatColor( 0.9f, 0.9f, 0.9f, 1.0f ) );
    shadowLightLeft.setSpecularColor( ofFloatColor( 0.1f, 0.1f, 0.1f, 1.0f ) );
    
    shadowLightLeft.setPosition( 200.0f, 0.0f, 45.0f );
    
//    shadowLightRight.setup( 2048, 45.0f, 0.1f, 100.0f );
//    shadowLightRight.setBlurLevel(4.0f); // amount to blur to soften the shadows
//    
//    shadowLightRight.setAmbientColor( ofFloatColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
//    shadowLightRight.setDiffuseColor( ofFloatColor( 0.9f, 0.9f, 0.9f, 1.0f ) );
//    shadowLightRight.setSpecularColor( ofFloatColor( 0.1f, 0.1f, 0.8f, 1.0f ) );
//    
//    shadowLightRight.setPosition( 200.0f, 0.0f, 45.0f );
//    
    ofSetGlobalAmbientColor( ofFloatColor( 0.05f, 0.05f, 0.05f ) );
}

//GUI INIT CODE - LOADS MASTER SETTINGS DOCS AND POPULATES GUI OF THEIR CONTENT

void testApp::loadHashtag()
{
    //gui variables
    
    bSingle=true;
    
    settings.loadFile("settings.xml");
    
    
    settings.pushTag("LETTERS");
    int numberOfSavedPoints = settings.getNumTags("LETTER");
    
    int idCount=0;
    //    ofDisableArbTex();
    for(int i = 0; i < 13; i++){
        
        //LOAD POSITION SETTINGS FROM MASTER SETTINGS XML DOCUMENT
        
        settings.pushTag("LETTER", i);
        
        string loadKey = settings.getValue("NAME","0");
        
        settings.pushTag("COLLISION");
    
        settings.pushTag("POS");
        Hashletter newHashCollision;
        newHashCollision.pos.x = settings.getValue("X", 0.);
        newHashCollision.pos.y = settings.getValue("Y", 0.);
        newHashCollision.pos.z = settings.getValue("Z", 0.);
        newHashCollision.key=loadKey;
        settings.popTag();
        
        settings.pushTag("ROTATION");
        newHashCollision.rot.x = settings.getValue("X", 0.);
        newHashCollision.rot.y = settings.getValue("Y", 0.);
        newHashCollision.rot.z = settings.getValue("Z", 0.);
        settings.popTag();
        
        settings.pushTag("SCALE");
        newHashCollision.scale.x = settings.getValue("X", 1.);
        newHashCollision.scale.y = settings.getValue("Y", 1.);
        newHashCollision.scale.z = settings.getValue("Z", 1.);
        settings.popTag();
        
        settings.popTag();
        
        
        string file=settings.getValue("OBJ"," ");
        settings.popTag();
        
        hashModel.loadModel(file,true );
        
//        hashModel[i].loadModel("models/cannes_placementCorrected.obj", true);
        
        ofPoint scale=newHashCollision.scale;
    
        newHashCollision.active=false;
        newHashCollision.size=hashModel.getSceneMax()-hashModel.getSceneMin();
        newHashCollision.checkbox=ofRectangle(10,i*20,10,10);
        
        hashletters.push_back(newHashCollision);
        
        hashCollision.push_back( new ofxBulletCustomShape() );
        
        btTransform trans;
        ofPoint temp=hashletters[i].pos;
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofPoint rot = hashletters[i].rot;
        btQuaternion rotate=btQuaternion(rot.x,rot.y,rot.z,ofDegToRad(1));
        trans.setRotation(rotate);
        hashCollision[i]->addMesh(hashModel.getMesh(0), scale, false);
        hashCollision[i]->create( world.world, trans,0.);
        hashCollision[i]->add();
        hashCollision[i]->setProperties(.1, 1);
        hashMeshes.push_back(hashModel.getMesh(0));
        hashModel.clear();
    }
    //    ofEnableArbTex();
    settings.popTag();
}


//USER INPUT FUNCTIONS

void testApp::mousePressed(int x, int y, int button) {
    if(bGUI==true){
        for(int i=0;i<hashletters.size();i++){
            if(hashletters[i].checkbox.inside(mouseX,mouseY)){
                hashletters[i].active=!hashletters[i].active;
            }
        }
    }
}

void testApp::keyPressed(int key){
    switch(key){
            
        case 'f':
            bSingle=false;
            return;
            
            
        case 'r':
            bRot=true;
            return;
            
        case 's':
            bScale=true;
            return;
            
        case 'c':
            camState++;
            if (camState>1){
                camState=0;
            }
            return;
            
        case ' ':
            settings.pushTag("LETTERS");
            
            for(int i = 0; i < hashletters.size(); i++){
                
                settings.pushTag("LETTER", i);
                
                settings.pushTag("COLLISION");
                
                settings.pushTag("POS");
                settings.setValue("X", hashletters[i].pos.x);
                settings.setValue("Y", hashletters[i].pos.y);
                settings.setValue("Z", hashletters[i].pos.z);
                settings.popTag();
                
                settings.pushTag("ROTATION");
                settings.setValue("X", hashletters[i].rot.x);
                settings.setValue("Y", hashletters[i].rot.y);
                settings.setValue("Z", hashletters[i].rot.z);
                settings.popTag();
                
                settings.pushTag("SCALE");
                settings.setValue("X", hashletters[i].scale.x);
                settings.setValue("Y", hashletters[i].scale.y);
                settings.setValue("Z", hashletters[i].scale.z);
                settings.popTag();
                
                settings.popTag();
                
                
                settings.popTag();
            }
            settings.popTag();
            settings.saveFile("settings.xml");
            return;
            
        case OF_KEY_DOWN:

            for(int i=0;i<hashletters.size();i++){
                    
                    
                    if(hashletters[i].active&&bScale==true){
                            if(!bSingle){
                                hashletters[i].scale.y+=.01;
                            }
                            else{
                                hashletters[i].scale.y+=.001;
                            }
                            updateCollision(i);
                        
                    }
                    
                    else if(hashletters[i].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i].rot.y-=10;
                        }
                        else{
                            hashletters[i].rot.y--;
                        }
                    }
                    
                    else if(hashletters[i].active){

                            if(!bSingle){
                                hashletters[i].pos.y--;
                            }
                            else{
                                hashletters[i].pos.y-=.1;
                            }
                            updateCollision(i);
                        }
            }
            return;
            
        case OF_KEY_UP:

            for(int i=0;i<hashletters.size();i++){
                
                    if(hashletters[i].active&&bScale==true){
                            if(!bSingle){
                                hashletters[i].scale.y-=.01;
                            }
                            else{
                                hashletters[i].scale.y-=.001;
                            }
                            updateCollision(i);
                    }
                    
                    else if(hashletters[i].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i].rot.y+=10;
                        }
                        else{
                            hashletters[i].rot.y++;
                        }
                            updateCollision(i);
                    }
                    
                    else if(hashletters[i].active){

                            if(!bSingle){
                                hashletters[i].pos.y++;
                            }
                            else{
                                hashletters[i].pos.y+=.1;
                            }
                            updateCollision(i);
                    }
            }
            return;
            
        case OF_KEY_RIGHT:

            for(int i=0;i<hashletters.size();i++){                    
                    
                    if(hashletters[i].active&&bScale==true){
                            if(!bSingle){
                                hashletters[i].scale.x+=.01;
                            }
                            else{
                                hashletters[i].scale.x+=.001;
                            }
                        updateCollision(i);                        
                    }
                    
                    else if(hashletters[i].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i].rot.x+=10;
                        }
                        else{
                            hashletters[i].rot.x++;
                        }
                            updateCollision(i);
                    }
                    
                    else if(hashletters[i].active){
                            if(!bSingle){
                                hashletters[i].pos.x++;
                            }
                            else{
                                hashletters[i].pos.x+=.1;
                            }
                            updateCollision(i);
                }
            }
            return;
            
        case OF_KEY_LEFT:

            for(int i=0;i<hashletters.size();i++){
                    
                    
                    if(hashletters[i].active&&bScale==true){
                            if(!bSingle){
                                hashletters[i].scale.x-=.01;
                            }
                            else{
                                hashletters[i].scale.x-=.001;
                            }
                            updateCollision(i);                        
                    }
                    
                    else if(hashletters[i].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i].rot.x-=10;
                        }
                        else{
                            hashletters[i].rot.x--;
                        }
                            updateCollision(i);
                    }
                    
                    else if(hashletters[i].active){

                            if(!bSingle){
                                hashletters[i].pos.x--;
                            }
                            else{
                                hashletters[i].pos.x-=.1;
                            }
                            updateCollision(i);
                    }
            }
            return;
            
        case '-':
        case '_':

            for(int i=0;i<hashletters.size();i++){

                    if(hashletters[i].active&&bScale==true){
                            if(!bSingle){
                                hashletters[i].scale.z-=.01;
                            }
                            else{
                                hashletters[i].scale.z-=.001;
                            }
                            updateCollision(i);                        
                    }
                    
                    else if(hashletters[i].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i].rot.z-=10;
                        }
                        else{
                            hashletters[i].rot.z--;
                        }
                            updateCollision(i);
                    }
                    
                    else if(hashletters[i].active){
                            if(!bSingle){
                                hashletters[i].pos.z--;
                            }
                            else{
                                hashletters[i].pos.z-=.1;
                            }
                            updateCollision(i);
                        }
            }
            return;
            
        case '+':
        case '=':

            for(int i=0;i<hashletters.size();i++){
                    if(hashletters[i].active&&bScale==true){
                            if(!bSingle){
                                hashletters[i].scale.z+=.01;
                            }
                            else{
                                hashletters[i].scale.z+=.001;
                            }
                            updateCollision(i);
                    }
                    
                    else if(hashletters[i].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i].rot.z+=10;
                        }
                        else{
                            hashletters[i].rot.z++;
                        }

                            updateCollision(i);

                    }
                    
                    else if(hashletters[i].active){

                            if(!bSingle){
                                hashletters[i].pos.z++;
                            }
                            else{
                                hashletters[i].pos.z+=.1;
                            }
                            updateCollision(i);
                }
            }
            return;
            
            
        case ']':
        case '}':
            for(int i=0;i<hashletters.size();i++){
                    
                    if(hashletters[i].active){
                            if(!bSingle){
                                hashletters[i].scale.x+=.01;
                                hashletters[i].scale.y+=.01;
                                hashletters[i].scale.z+=.01;
                            }
                            else{
                                hashletters[i].scale.x+=.001;
                                hashletters[i].scale.y+=.001;
                                hashletters[i].scale.z+=.001;
                            }
                            updateCollision(i);
                        }
                
            }
            
            return;
            
        case '[':
        case '{':
            for(int i=0;i<hashletters.size();i++){
                if(hashletters[i].active){
                            if(!bSingle){
                                hashletters[i].scale.x-=.01;
                                hashletters[i].scale.y-=.01;
                                hashletters[i].scale.z-=.01;
                            }
                            else{
                                hashletters[i].scale.x-=.001;
                                hashletters[i].scale.y-=.001;
                                hashletters[i].scale.z-=.001;
                            }
                            updateCollision(i);
                        }
            }
    
            return;
    }
    
}

void testApp::keyReleased(int key){
    switch(key){
        case 'f':
            bSingle=true;
            return;
        case 'r':
            bRot=false;
            return;
        case 's':
            bScale=false;
            return;
        case 'h':
            bGUI=!bGUI;
            break;
        case 'd':
            bDebug=!bDebug;
            break;
        case 'l':
            bShadowsOn=!bShadowsOn;
            break;
    }
}

void testApp::updateCollision(int i){
    btTransform trans;
    ofPoint temp=hashletters[i].pos;
    trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
    ofPoint rot = hashletters[i].rot;
    btQuaternion rotate=btQuaternion(rot.x,rot.y,rot.z,ofDegToRad(1));
    trans.setRotation(rotate);
    ofVec3f scale=hashletters[i].scale;
    hashCollision[i]->remove();
    hashCollision[i]->init();
    hashCollision[i]->create(world.world,trans,0.);
    hashCollision[i]->addMesh(hashMeshes[i], scale, false);
    hashCollision[i]->add();
    hashCollision[i]->setProperties(.1, 1);
    
    
}

void testApp::drawGUI(){
    ofNoFill();
    ofSetColor(255,255,255);
    for(int i=0;i<hashletters.size();i++){
        if (hashletters[i].active){
            ofSetColor(0,255,0);
        }
        ofRect(hashletters[i].checkbox);
        ofSetColor(225,255,255);
    }
    ofFill();
}


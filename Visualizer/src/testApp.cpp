#include "testApp.h"

#define tileW 30
#define tileH 30
#define tileD 2

#define USE_DOF true
#define CAM_MOVE false
#define CAM_MOUSE false
#define FACES false



//--------------------------------------------------------------
void testApp::setup(){
    
    //CALIBRATION VARIABLES -- TODO: ADD TO GUI
    
    bDebug=true;
    bTweet=true;
    
    tweetScale=ofPoint(.1,.1,.1);
    meshCollisionScale=ofPoint(5,5 ,5);
    meshDrawScale=ofPoint(10,10,20);
    image.pos=ofPoint(-10,-10,-55);
    boxScale=ofPoint(.05,.05,.05);
//    ofSetBackgroundColor(255);
    
    gotham=*new Alphabet();
    
    camState=1;
    
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
        camera2.disableMouseInput();
    }
     
    
    camera.setDistance(40.0f);
    camera.setGlobalPosition( 30.0f, 15.0f, 00.0f );
    
    camera.lookAt( ofVec3f( 0.0f, 0.0f, 0.0f ) );
    
    shader.load( "shaders/mainScene.vert", "shaders/mainScene.frag" );
//    model.loadModel("models/cannes_placementCorrected.obj", true);
//    
//    hashModel[0].setScale(0.04, 0.04, 0.04);
//    mesh = hashModel[1].getMesh(0);
    
    setupLights();
    
    bRot=false;
    
    //GUI and hashtag mesh loading
    loadHashtagGUI();
    hashtagGUI->loadSettings("GUI/hashtagGUI.xml");
    
    //camKeyframe cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    //particle keyframes animate sequence of particle behaviors for individual image or tweet
    camKeyframe=0;
    particleKeyframe=0;
    loadParticleKeyframes();
    
    //populates vector of images off of specified directory
    imgCount=0;
    if(bTweet==false){
        loadDir();
        loadImage();
    }
    else{
        loadTweet();
    }
    
    
    
    
}

//--------------------------------------------------------------
void testApp::setupLights() {
    
    // shadow map resolution (must be power of 2), field of view, near, far
    // the larger the shadow map resolution, the better the detail, but slower
    shadowLight.setup( 2048, 45.0f, 0.1f, 100.0f );
    shadowLight.setBlurLevel(4.0f); // amount to blur to soften the shadows
    
    shadowLight.setAmbientColor( ofFloatColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
    shadowLight.setDiffuseColor( ofFloatColor( 0.9f, 0.9f, 0.9f, 1.0f ) );
    shadowLight.setSpecularColor( ofFloatColor( 0.1f, 0.1f, 0.8f, 1.0f ) );
    
    shadowLight.setPosition( 200.0f, 0.0f, 45.0f );
    
    ofSetGlobalAmbientColor( ofFloatColor( 0.05f, 0.05f, 0.05f ) );
}

//--------------------------------------------------------------
void testApp::drawObjects(){
    
//    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
//    ofScale(6,6,6);
    ofRotate(90, 0, 1, 0);
    ofRotate(330,1,0,0);
    
    for(int i = 0; i < 11  ; i++) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        glDisable(GL_CULL_FACE);
        ofVec3f scale=hashletters[i].scale;
        btScalar	m[16];
        ofGetOpenGLMatrixFromRigidBody( hashCollision[i]->getRigidBody(), m );
        glPushMatrix();
        glMultMatrixf( m );
        glTranslatef(-hashletters[i].size.x*scale.x/2, -hashletters[i].size.y*scale.y/2, -hashletters[i].size.z*scale.z/2);
        ofScale(scale.x,scale.y,scale.z);
        hashModel[i].getMesh(0).draw();
        glPopMatrix();
        glPopAttrib();
    }
    
//    mesh.draw();
}


//--------------------------------------------------------------
void testApp::update(){
    
    ofSetWindowTitle( ofToString( ofGetFrameRate() ) );
    
    world.update();    //updates bullet objects
    
    
    //check whether individual particles have completed the duration of their transition
    int moving=0;
    for(int i=0;i<particles.size();i++){
        if(!particles[i].targetReached){
            moving++;
        }
    }
    
    //if all have reached, trigger next keyframe position
    if (moving==0){
        bNewParticleKey=true;
        particleKeyframe++;
        if(particleKeyframe>particleKeyframes.size()-1){
            particleKeyframe=0;
        }
    }
    
    //set new particle destinations
    if(bNewParticleKey==true){
        
        //Destroy particles action - triggers image switch and removes bullet objects/position vectors
        
        if(particleKeyframes[particleKeyframe].path==PARTICLE_PATH_DESTROY){
            particleKeyframe++;
            if(particleKeyframe>particleKeyframes.size()-1){
                particleKeyframe=0;
            }
            if(bTweet==false){
                loadImage();
            }
        }
        
        else if(particleKeyframes[particleKeyframe].path!=PARTICLE_PATH_PHYSICS){
            if(bTweet==true){
                tweetToKinematic();
            }
            else{
                imgToKinematic();
            }
        }
        
        //if switching into physics state
        
        else{
            if(bTweet==true){
                tweetToPhysics();
            }
            else{
                imgToPhysics();
            }
        }
        bNewParticleKey=false;
    }
    
    //if not new keyframe
    
    else{
        
        //if not in physics state
        
        if(bTweet==true){
            updateTweet();
        }
        
        else{
            updateImg();
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    
	glEnable(GL_DEPTH_TEST);
    ofDisableAlphaBlending();
    ofEnableLighting();
    
    shadowLight.lookAt( ofVec3f(0.0,0.0,0.0) );
    shadowLight.orbit( 90, -70, 90, ofVec3f(0.0,0.0,0.0) );
    //(long, lat, radius, ofVec3f(centerpoint))
    shadowLight.enable();
    
    // render linear depth buffer from light view
    shadowLight.beginShadowMap();
    drawObjects(); // render to shader map
    shadowLight.endShadowMap();
    
    // render final scene
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    shader.begin();
    
    shadowLight.bindShadowMapTexture(0); // bind shadow map texture to unit 0
    shader.setUniform1i("u_ShadowMap", 0); // set uniform to unit 0
    shader.setUniform1f("u_LinearDepthConstant", shadowLight.getLinearDepthScalar()); // set near/far linear scalar
    shader.setUniformMatrix4f("u_ShadowTransMatrix", shadowLight.getShadowMatrix(camera)); // specify shadow matrix
    
    camera.begin();
    
    shadowLight.enable();
    
    // draw background for projecting shadows onto
    ofPushMatrix();
    ofScale(6,6,6);
    ofRotate(90, 0, 1, 0);
    ofRotate(330,1,0,0);
    ofRect(-8, -8, 16, 16);
    ofPopMatrix();
    
    material.begin();
    white.bind();
    drawObjects(); // render to screen
    white.unbind();
    material.end();
    
    //bind image textures and draw bullet shapes
    
    material.begin();
    
    if(bTweet==true){
        for (int i=0;i<letters.size();i++){
            face[i].bind();
            //        letters[i]->draw();
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            glEnable(GL_NORMALIZE);
            glDisable(GL_CULL_FACE);
            btScalar	m[16];
            ofGetOpenGLMatrixFromRigidBody( letters[i]->getRigidBody(), m );
            glPushMatrix();
            glMultMatrixf( m );
            glTranslatef(-particles[i].size.x/2, -particles[i].size.y/2, -particles[i].size.z/2);
            gotham.draw(particles[i].letter,tweetScale);
            glPopMatrix();
            glPopAttrib();
            face[i].unbind();
        }
    }
    
    else{
        for (int i=0;i<shapes.size();i++){
            face[i].bind();
            shapes[i]->draw();
            face[i].unbind();
        }
    }
    
    material.end();
    
    shadowLight.disable();
    
   
    
    //enable to see physics collision wireframes
    if(bDebug==true){
        world.drawDebug();
    }
    
    
    
    camera.end();
    
    shadowLight.unbindShadowMapTexture();
    shader.end();
    
    
    ofDisableLighting();

    

    
    glDisable(GL_DEPTH_TEST);
    
    //     debug view of shader scene
//    shadowLight.debugShadowMap();
    
/*
    
    
	glEnable(GL_DEPTH_TEST);
    ofDisableAlphaBlending();
//    glShadeModel(GL_SMOOTH);
    ofEnableLighting();
    
    shadowLight.lookAt( ofVec3f(0.0,0.0,0.0) );
    shadowLight.orbit( 90, -70, 90, ofVec3f(0.0,0.0,0.0) );
    //(long, lat, radius, ofVec3f(centerpoint))
    shadowLight.enable();
    
    // render linear depth buffer from light view
    shadowLight.beginShadowMap();
    drawObjects(); // render to shader map
    shadowLight.endShadowMap();
    
    // render final scene
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    shader.begin();
    
    shadowLight.bindShadowMapTexture(0); // bind shadow map texture to unit 0
    shader.setUniform1i("u_ShadowMap", 0); // set uniform to unit 0
    shader.setUniform1f("u_LinearDepthConstant", shadowLight.getLinearDepthScalar()); // set near/far linear scalar
    shader.setUniformMatrix4f("u_ShadowTransMatrix", shadowLight.getShadowMatrix(camera)); // specify shadow matrix
    
    camera.begin();
    
    shadowLight.enable();
    
    // draw background for projecting shadows onto
    ofPushMatrix();
    ofScale(6,6,6);
    ofRotate(90, 0, 1, 0);
    ofRotate(330,1,0,0);
    ofRect(-8, -8, 16, 16);
    ofPopMatrix();
    
    drawObjects(); // render to screen
    shadowLight.disable();
    
    material.end();
    
    camera.end();
    
    shadowLight.unbindShadowMapTexture();
    shader.end();
    
    ofDisableLighting();
    ofDisableArbTex();
    
    
    //      enable to see physics collision wireframes
//    if(bDebug==true){
//        world.drawDebug();
//    }
    
    material.begin();
    white.bind();
    
//    btScalar	m[16];
//    ofGetOpenGLMatrixFromRigidBody( background.getRigidBody(), m );
//    glPushMatrix();
//    glMultMatrixf( m );
//    white.bind();
//    background.draw();
//    ofScale(200,200,1);
//    ofBox(0,0,0,1);
//    white.unbind();
//    glPopMatrix();
    
//    drawHashtag();
    
    white.unbind();
    
    
    //bind image textures and draw bullet letters
    if(bTweet==true){
        for (int i=0;i<letters.size();i++){
            face[i].bind();
            //        letters[i]->draw();
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            glEnable(GL_NORMALIZE);
            glDisable(GL_CULL_FACE);
            btScalar	m[16];
            ofGetOpenGLMatrixFromRigidBody( letters[i]->getRigidBody(), m );
            glPushMatrix();
            glMultMatrixf( m );
            glTranslatef(-particles[i].size.x/2, -particles[i].size.y/2, -particles[i].size.z/2);
            gotham.draw(particles[i].letter,tweetScale);
            glPopMatrix();
            glPopAttrib();
            face[i].unbind();
        }
    }
    else{
        for (int i=0;i<shapes.size();i++){
            face[i].bind();
            shapes[i]->draw();
            face[i].unbind();
        }
    }
     
    
//    material.end();
//    light.disable();
//    light3.disable();
//    camera.end();
    
    //    camera2.begin();
    //        light2.enable();
    //    if(bDebug==true){
    //        light2.draw();
    //    }
    //
    //
    //    camera2.end();
    //    light2.disable();

    
//    if (USE_DOF) {
//        dof.end();
//        //fbo of dof for drawing
//        ofFbo temp=dof.getFbo();
//        temp.draw(0,0,ofGetWidth(),ofGetHeight());
//    }
    
    glDisable(GL_DEPTH_TEST);
    
    //show framerate
    //    ofDrawBitmapString(ofToString(ofGetFrameRate()),ofPoint(800,100,0));
    
    */
}

void testApp::loadParticleKeyframes(){
    
    //Populate particle behavior Keyframes off of XML document
    
    particleXML.loadFile("keyframes/particleKeyframes/particles.xml");
    
    particleXML.pushTag("PARTICLES");
    int numParticles = particleXML.getNumTags("PARTICLE");
    for(int i = 0; i < numParticles; i++){
        particleXML.pushTag("PARTICLE", i);
        Particle::keyframe temp;
        temp.path=particleXML.getValue("PATH",0);
        
        particleXML.pushTag("TYPE");
        temp.type.x=particleXML.getValue("X",0);
        temp.type.y=particleXML.getValue("Y",0);
        temp.type.z=particleXML.getValue("Z",0);
        particleXML.popTag();
        
        particleXML.pushTag("POS");
        temp.pos.x = particleXML.getValue("X", 0);
        temp.pos.y = particleXML.getValue("Y", 0);
        temp.pos.z = particleXML.getValue("Z", 0);
        
        particleXML.pushTag("MIN");
        temp.posMin.x = particleXML.getValue("X", 0);
        temp.posMin.y = particleXML.getValue("Y", 0);
        temp.posMin.z = particleXML.getValue("Z", 0);
        particleXML.popTag();
        
        particleXML.pushTag("MAX");
        temp.posMax.x = particleXML.getValue("X", 0);
        temp.posMax.y = particleXML.getValue("Y", 0);
        temp.posMax.z = particleXML.getValue("Z", 0);
        particleXML.popTag();
        particleXML.popTag();
        
        particleXML.pushTag("DURATION");
        temp.duration= particleXML.getValue("ACTUAL", 0);
        temp.durationMax= particleXML.getValue("MAX", 0);
        temp.durationMin= particleXML.getValue("MIN", 0);
        particleXML.popTag();
        
        particleXML.pushTag("INTERPOLATIONS");
        int interpCount=particleXML.getNumTags("INTERPOLATION");
        for(int j = 0; j < interpCount; j++){
            temp.interpolations.push_back(particleXML.getValue("INTERPOLATION",0,j));
        }
        particleXML.popTag();
        
        particleXML.pushTag("GRAVITY");
        temp.gravity.x = particleXML.getValue("X", 0);
        temp.gravity.y = particleXML.getValue("Y", 0);
        temp.gravity.z = particleXML.getValue("Z", 0);
        particleXML.popTag();
        
        particleXML.popTag();
        particleKeyframes.push_back(temp);
    }
    
    particleXML.popTag();
    
    //trigger GUI update
    ofAddListener(hashtagGUI->newGUIEvent,this,&testApp::guiEvent);
}


//--------------------------------------------------------------
void testApp::setupGL(){
//    
//    //set up GL lighting and materials
//    light.setDiffuseColor(ofColor(255,255,255));
//    light.setSpecularColor(ofColor(255,255,255));
//    //    light.setDirectional();
//    //    light.setOrientation(ofPoint(0,0,-180));
//    light.setSpotlight();
//    light.setPosition(ofGetWidth()/2,ofGetHeight()/2,500);
//    light.lookAt(ofVec3f(ofGetWidth()/2,ofGetHeight()/2,-100));
//    
//    light2.setDiffuseColor(ofColor(255,255,255));
//    light2.setSpecularColor(ofColor(255,255,255));
//    light2.setSpotlight();
//    light2.setPosition(0,ofGetHeight()/2,-100);
//    light.lookAt(ofVec3f(200,ofGetHeight()/2,-40));
    
    
    material.setShininess(.1);
    
    //initialize Bullet Physics world
    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    
    //Create static collision objects
    background.create(world.world,ofVec3f(0,0,-90.),0.,175,100,10.);
    background.add();
    background.setProperties(.1,1);
    
    
    whiteImg.loadImage("textures/whiteBig.png");
    white=whiteImg.getTextureReference();
     
    
}


//SETUP PARTICLES OFF IMAGE

void testApp::loadImage(){
    
    //go to next image in vector and create new particles
    if(imgCount>images.size()-1){
        imgCount=0;
    }
    if(particleKeyframe>particleKeyframes.size()-1){
        particleKeyframe=0;
    }
    pic.loadImage(images[imgCount]);
    pic.setImageType(OF_IMAGE_COLOR);
    pic.resize(pic.width/2, pic.height/2);
    pic.mirror(false, false);
    initImgParticles();
    imgCount++;
}

void testApp::loadDir(){
    
    //use ofDirectory to load image files into vector of strings then close dir
    
    string path = "images";
    ofDirectory dir(path);
    dir.allowExt("jpg");
    dir.allowExt("png");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++){
        string newImage=dir.getPath(i);
        images.push_back(newImage);
    }
    dir.close();
}

void testApp::initImgParticles(){
    
    for(int i=0;i<shapes.size();i++){
        shapes[i]->remove();
    }
    shapes.clear();
    particles.clear();
    face.clear();
    
    boxShape = ofBtGetBoxCollisionShape(boxScale.x*tileW, boxScale.y*tileH, boxScale.z*tileD);
    
    for (int x = 0; x < pic.width / tileW; x++){
        for (int y = 0; y < pic.height / tileH; y++){
            
            //setup position values for displaying whole image
            Particle::keyframe temp;
            temp.pos.x=x*tileW*boxScale.x+image.pos.x;
            temp.pos.y=y*tileH*boxScale.y+image.pos.y;
            temp.pos.z=image.pos.z;
            
            //create Bullet shapes for image visualization
            shapes.push_back( new ofxBulletBox() );
            
            ((ofxBulletBox*)shapes[shapes.size()-1])->init(boxShape);
            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world,particleKeyframes[particleKeyframe].pos,0.);
            shapes[shapes.size()-1]->setProperties(.1,1);
            shapes[shapes.size()-1]->add();
            shapes[shapes.size()-1]->enableKinematic();
            
            //crop and allocate image boxes to tiles
            ofTexture newFace;
            ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
            tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            tileImage.mirror(false,true);
            ofEnableNormalizedTexCoords();
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            face.push_back(newFace);
            
            //create particles for controlling static shape position/rotation
            Particle particle;
            particle.setup(temp,x+x*y);
            particles.push_back(particle);
        }
    }
    
    for(int i=0;i<particles.size();i++){
        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
        particles[i].start.pos=particles[i].target.pos;
    }
}

void testApp::loadTweet(){
    assert(ofTextConverter::toUTF8(0x30A1) == "\u30A1");
    string tweetRaw="ABCDEFG HIJKLMNOP";
    ofUniString tweet= ofTextConverter::toUTF32(tweetRaw);
    
    ofPoint pos=ofPoint(-45,0,-60);
    for (int i=0;i<tweet.length();i++){
//        
//        cout<<int(chars[i])<<endl;
        
        if (tweet[i]==32){
            pos.x+=10;
            if(pos.x>40){
                pos.y+=5;
                pos.x=-45;
            }
        }
        
        else{
            //setup position values for displaying whole image
            Particle::keyframe temp;
            temp.pos.x=pos.x;
            temp.pos.y=pos.y;
            temp.pos.z=pos.z;
            ofVec3f boxDim;
            boxDim=gotham.getSize(tweet[i])*tweetScale;
            //create Bullet letters for image visualization
            letters.push_back( new ofxBulletCustomShape() );
            
            ofQuaternion startRot=ofQuaternion(0,0,1,PI);
            letters[letters.size()-1]->addMesh(gotham.getMesh(tweet[i]),tweetScale,true);
            letters[letters.size()-1]->create(world.world,particleKeyframes[particleKeyframe].pos,startRot,10.);
            letters[letters.size()-1]->add();
            letters[letters.size()-1]->setProperties(.1,.1);
            letters[letters.size()-1]->enableKinematic();
            
            
            
            ofTexture newFace=whiteImg.getTextureReference();
            face.push_back(newFace);
            
            //create particles for controlling static shape position/rotation
            Particle particle;
            particle.setup(temp,i,tweet[i],boxDim);
            particles.push_back(particle);
            pos.x+=boxDim.x+2;
        }
    }
}

//GUI INIT CODE - LOADS MASTER SETTINGS DOCS AND POPULATES GUI OF THEIR CONTENT

void testApp::loadHashtagGUI()
{
    //gui variables
    
    bSingle=true;
    float dim = 16;
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    settings.loadFile("settings.xml");
    
    hashtagGUI = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
    
    //LOAD MASTER SETTINGS FOR HASHTAG PLACEMENT
    
    settings.pushTag("HASHTAG");
    settings.pushTag("POS");
    hash.pos.x = settings.getValue("X", 0);
    hash.pos.y = settings.getValue("Y", 0);
    hash.pos.z = settings.getValue("Z", 0);
    settings.popTag();
    settings.popTag();
    hashtagGUI->addWidgetDown(new ofxUIToggle( dim, dim, false, "Entire Hashtag"))->setID(-1);
    
    settings.pushTag("LETTERS");
    int numberOfSavedPoints = settings.getNumTags("LETTER");
    
    int idCount=0;
    //    ofDisableArbTex();
    for(int i = 0; i < 12; i++){
        
        //LOAD POSITION SETTINGS FROM MASTER SETTINGS XML DOCUMENT
        
        settings.pushTag("LETTER", i);
        
        string loadKey = settings.getValue("NAME","0");
        
        settings.pushTag("COLLISION");
    
        settings.pushTag("POS");
        hashletter newHashCollision;
        newHashCollision.pos.x = settings.getValue("X", 0.);
        newHashCollision.pos.y = settings.getValue("Y", 0.);
        newHashCollision.pos.z = settings.getValue("Z", 0.);
        newHashCollision.key=loadKey;
        cout<<loadKey<<endl;
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
        
        hashModel[i].loadModel(file,true );
        
//        hashModel[i].loadModel("models/cannes_placementCorrected.obj", true);
        cout<<"meshes:"<<hashModel[i].getNumMeshes()<<endl;
        
        ofPoint scale=newHashCollision.scale;
    
        newHashCollision.active=false;
        newHashCollision.size=hashModel[i].getSceneMax()-hashModel[i].getSceneMin();
        
        hashletters.push_back(newHashCollision);
        
        hashCollision.push_back( new ofxBulletCustomShape() );
        
        btTransform trans;
        ofPoint temp=hashletters[i].pos;
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofPoint rot = hashletters[i].rot;
        btQuaternion rotate=btQuaternion(rot.x,rot.y,rot.z,ofDegToRad(1));
        trans.setRotation(rotate);
        
        hashCollision[i]->addMesh(hashModel[i].getMesh(0), scale, false);
        hashCollision[i]->create( world.world, trans,0.);
        hashCollision[i]->add();
        hashCollision[i]->setProperties(.1, 1);
        
        //ADD HASHTAG SETTINGS TO GUI
        
        hashtagGUI->addToggle( newHashCollision.key, false, dim, dim)->setID(idCount);
        idCount++;
    }
    //    ofEnableArbTex();
    settings.popTag();
    ofAddListener(hashtagGUI->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    int num=e.widget->getID();
    
    ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
    
    if(num!=-1){
        hashletters[num].active=toggle->getValue();
    }
    else{
        hash.active=toggle->getValue();
    }
}


//USER INPUT FUNCTIONS

void testApp::mousePressed(int x, int y, int button) {
    
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
            if (camState>2){
                camState=0;
            }
            bCamSwitch=true;
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
            settings.pushTag("HASHTAG");
            settings.pushTag("POS");
            settings.setValue("X", hash.pos.x);
            settings.setValue("Y", hash.pos.y);
            settings.setValue("Z", hash.pos.z);
            settings.popTag();
            settings.popTag();
            settings.saveFile("settings.xml");
            hashtagGUI->saveSettings("GUI/hashtagGUI.xml");
            return;
            
        case OF_KEY_UP:
            if(hash.active&&bRot==false){
                if(!bSingle){
                    hash.pos.y-=10;
                }
                else{
                    hash.pos.y--;
                }
                
            }
            
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
            
        case OF_KEY_DOWN:
            if(hash.active){
                if(!bSingle){
                    hash.pos.y+=10;
                }
                else{
                    hash.pos.y++;
                }
                
            }
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
            if(hash.active){
                if(!bSingle){
                    hash.pos.x+=10;
                }
                else{
                    hash.pos.x++;
                }
                
            }
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
            if(hash.active){
                if(!bSingle){
                    hash.pos.x-=10;
                }
                else{
                    hash.pos.x--;
                }
                
            }
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
            if(hash.active){
                if(!bSingle){
                    hash.pos.z-=10;
                }
                else{
                    hash.pos.z--;
                }
                
            }
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
            if(hash.active){
                if(!bSingle){
                    hash.pos.z+=10;
                }
                else{
                    hash.pos.z++;
                }
                
            }
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
                                hashletters[i].scale.x+=.1;
                                hashletters[i].scale.y+=.1;
                                hashletters[i].scale.z+=.1;
                            }
                            else{
                                hashletters[i].scale.x+=.01;
                                hashletters[i].scale.y+=.01;
                                hashletters[i].scale.z+=.01;
                            }
                            updateCollision(i);
                        }
                
            }
            
            return;
            
        case '[':
        case '{':
            for(int i=0;i<hashletters.size();i++){

                            if(!bSingle){
                                hashletters[i].scale.x-=.1;
                                hashletters[i].scale.y-=.1;
                                hashletters[i].scale.z-=.1;
                            }
                            else{
                                hashletters[i].scale.x-=.01;
                                hashletters[i].scale.y-=.01;
                                hashletters[i].scale.z-=.01;
                            }
                            updateCollision(i);
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
            hashtagGUI->toggleVisible();
            break;
        case 'd':
            bDebug=!bDebug;
            break;
            
    }
}

void testApp::tweetToKinematic(){
    for (int i=0;i<letters.size();i++){
        particles[i].update();
        
        //set vector position/rotation to bullet object position/rotation and start rotation transition
        btTransform trans;
        ofPoint temp=letters[i]->getPosition();
        if(particleKeyframe!=0){
            if(particleKeyframes[particleKeyframe-1].path==PARTICLE_PATH_PHYSICS){
                particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(particleKeyframe!=0){
            if(particleKeyframes[particleKeyframe-1].path==PARTICLE_PATH_PHYSICS){
                particles[i].bRotate=true;
                particles[i].current.quat=rotQuat;
            }
        }
        letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        letters[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
        letters[i]->enableKinematic();
    }
}

void testApp::imgToKinematic(){
    for (int i=0;i<shapes.size();i++){
        particles[i].update();
        btTransform trans;
        ofPoint temp;
        if(particleKeyframe!=0){
            if(particleKeyframes[particleKeyframe-1].path==PARTICLE_PATH_PHYSICS){
                particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = shapes[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(particleKeyframe!=0){
            if(particleKeyframes[particleKeyframe-1].path==PARTICLE_PATH_PHYSICS){
                particles[i].bRotate=true;
                particles[i].current.quat=rotQuat;
            }
        }
        shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        shapes[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
        shapes[i]->enableKinematic();
    }
}

void testApp::tweetToPhysics(){
    world.setGravity(ofVec3f(particleKeyframes[particleKeyframe].gravity.x,particleKeyframes[particleKeyframe].gravity.y,particleKeyframes[particleKeyframe].gravity.z));
    for (int i=0;i<letters.size();i++){
        btTransform trans;
        ofPoint temp=letters[i]->getPosition();
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        letters[i]->remove();
        letters[i]->addMesh(gotham.getMesh(particles[i].letter),tweetScale,false);
        letters[i]->create(world.world,trans, 1.);
        letters[i]->add();
        letters[i]->setProperties(.1,1);
        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
    }
}

void testApp::imgToPhysics(){
    world.setGravity(ofVec3f(particleKeyframes[particleKeyframe].gravity.x,particleKeyframes[particleKeyframe].gravity.y,particleKeyframes[particleKeyframe].gravity.z));
    for (int i=0;i<shapes.size();i++){
        btTransform trans;
        ofPoint temp=shapes[i]->getPosition();
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = shapes[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        shapes[i]->remove();
        shapes[i]->init(boxShape);
        
        shapes[i]->create(world.world,trans, 10);
        shapes[i]->setProperties(.1,1);
        shapes[i]->add();
        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
    }
    
}

void testApp::updateTweet(){
    if(particleKeyframes[particleKeyframe].path!=PARTICLE_PATH_PHYSICS){
        for (int i=0;i<letters.size();i++){
            particles[i].update();
            btTransform trans;
            ofPoint temp=particles[i].current.pos;
            trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
            ofQuaternion rotQuat = letters[i]->getRotationQuat();
            float newRot = rotQuat.w();
            if(particles[i].bRotate==true){
                float newRot = rotQuat.w();
                newRot -= .05f;
                if (newRot<0){
                    newRot=0;
                    particles[i].bRotate=false;
                }
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
            }
            else{
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), 0) );
            }
            letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
            letters[i]->activate();
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        for (int i=0;i<letters.size();i++){
            particles[i].update();
        }
    }
}

void testApp::updateImg(){
    if(particleKeyframes[particleKeyframe].path!=PARTICLE_PATH_PHYSICS){
        for (int i=0;i<shapes.size();i++){
            particles[i].update();
            btTransform trans;
            ofPoint temp=particles[i].current.pos;
            trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
            ofQuaternion rotQuat = shapes[i]->getRotationQuat();
            float newRot = rotQuat.w();
            if(particles[i].bRotate==true){
                float newRot = rotQuat.w();
                newRot -= .05f;
                if (newRot<0){
                    newRot=0;
                    particles[i].bRotate=false;
                }
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
            }
            else{
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), 0) );
            }
            shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
            shapes[i]->activate();
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        for (int i=0;i<shapes.size();i++){
            particles[i].update();
        }
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
    hashCollision[i]->addMesh(hashModel[i].getMesh(0), scale, false);
    //            hashCollision[i]->addMesh(collisionPlane.getMesh(0), scale, false);
    hashCollision[i]->add();
    hashCollision[i]->setProperties(.1, 1);
    
    
}

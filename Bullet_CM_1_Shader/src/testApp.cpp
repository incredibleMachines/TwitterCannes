#include "testApp.h"

#define tileW 40
#define tileH 40
#define tileD 5

#define USE_DOF true
#define CAM_MOVE false
#define CAM_MOUSE false
#define FACES false

//--------------------------------------------------------------
void testApp::setup(){
    
    sql = new ofxSQLite("test.db");
    //CALIBRATION VARIABLES -- TODO: ADD TO GUI
    
    meshCollisionScale=ofPoint(.45,.45,1);
    meshDrawScale=ofPoint(5,5,10);
    image.pos=ofPoint(0,0,0);
    boxScale=ofPoint(.05,.05,.05);
    
    camState=0;
    
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
    
    //camera, lighting, dof setup
    setupGL();
    
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
    loadDir();
    loadImage();
        
    if (USE_DOF) {
        dof.setup(ofGetWidth(),ofGetHeight());
        dof.setFocalDistance(20);
        dof.setFocalRange(100);
        dof.setBlurAmount(1);
    }
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
    }
    
    camera.setDistance(40.0f);
    camera.setGlobalPosition( 30.0f, 15.0f, 00.0f );
    
    camera.lookAt( ofVec3f( 0.0f, 0.0f, 0.0f ) );
    
    shader.load( "shaders/mainScene.vert", "shaders/mainScene.frag" );
    model.loadModel("models/cannes_placementCorrected.obj", true);
    
    hashModel[0].setScale(0.04, 0.04, 0.04);
    mesh = hashModel[1].getMesh(0);
    
    setupLights();
    
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
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    ofScale(6,6,6);
    ofRotate(90, 0, 1, 0);
    ofRotate(330,1,0,0);
        
    mesh.draw();
}

//--------------------------------------------------------------
void testApp::update(){
    
    ofSetWindowTitle( ofToString( ofGetFrameRate() ) );
    
    
    cout<<camera.getPosition()<<endl;
    //    camera2.rotate(1,10,0,0);
    
    //    hashMeshPos[0].pos.x--;
    //    hashMeshPos[0].pos.y--;
    //    hashMeshPos[0].pos.z-=10;
    
    world.update();
    //    cout<<camera.getPosition()<<endl;
    //cout<<    camera.getLookAtDir()<<endl;
    //    if (bCamSwitch==true&&camState==1){
    //        camPos=ofPoint(-200,0,-100);
    //        camera.setPosition(camPos);
    //        camera.lookAt(ofPoint(0,0,-100));
    //        bCamSwitch=false;
    //        cout<<"switch";
    //    }
    //
    //    else if (bCamSwitch==true&&camState==2){
    //        camPos=ofPoint(-350,0,0);
    //        camera.setPosition(camPos);
    //        camera.lookAt(ofPoint(0,0,0));
    //        bCamSwitch=false;
    //        cout<<"switch";
    //    }
    //
    //    else if (bCamSwitch==true&&camState==0){
    //        camPos=ofPoint(0,0,0);
    //        camera.setPosition(camPos);
    //        camera.lookAt(ofPoint(0,0,-90));
    //        bCamSwitch=false;
    //        cout<<"switch";
    //    }
    
    //updates bullet objects
    
    
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
        cout<<"next"<<endl;
    }
    
    //set new particle destinations
    if(bNewParticleKey==true){
        
        //Destroy particles action - triggers image switch and removes bullet objects/position vectors
        
        if(particleKeyframes[particleKeyframe].path==PARTICLE_PATH_DESTROY){
            particleKeyframe++;
            if(particleKeyframe>particleKeyframes.size()-1){
                particleKeyframe=0;
            }
            loadImage();
        }
        
        
        else if(particleKeyframes[particleKeyframe].path!=PARTICLE_PATH_PHYSICS){
            if(particleKeyframe!=0){
                
                //If Switching out of physics state...
                
                if(particleKeyframes[particleKeyframe-1].path==PARTICLE_PATH_PHYSICS){
                    for (int i=0;i<shapes.size();i++){
                        particles[i].update();
                        
                        //set vector position/rotation to bullet object position/rotation and start rotation transition
                        
                        btTransform trans;
                        ofPoint temp=shapes[i]->getPosition();
                        particles[i].current.pos=temp;
                        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                        ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                        float newRot = rotQuat.w();
                        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                        particles[i].bRotate=true;
                        particles[i].current.quat=rotQuat;
                        shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                        shapes[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
                        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
                        shapes[i]->enableKinematic();
                    }
                    
                }
                
                //if remaining in non-physics state (same as if not transitioning, with addition of a particles.goToPosition call
                
                else{
                    for (int i=0;i<shapes.size();i++){
                        particles[i].update();
                        btTransform trans;
                        ofPoint temp=shapes[i]->getPosition();
                        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                        ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                        float newRot = rotQuat.w();
                        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                        shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                        shapes[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
                        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
                        shapes[i]->enableKinematic();
                    }
                }
            }
            
            //if on first keyframe of vector
            
            else{
                for (int i=0;i<shapes.size();i++){
                    particles[i].update();
                    btTransform trans;
                    ofPoint temp=shapes[i]->getPosition();
                    trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                    ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                    float newRot = rotQuat.w();
                    trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                    shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                    shapes[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
                    particles[i].goToPosition(particleKeyframes[particleKeyframe]);
                    shapes[i]->enableKinematic();
                }
            }
        }
        
        //if switching into physics state
        
        else{
            cout<<"gravity"<<endl;
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
                shapes[i]->create(world.world,trans, 10.);
                //                shapes[i]->setProperties(1, 1);
                shapes[i]->add();
                particles[i].goToPosition(particleKeyframes[particleKeyframe]);
            }
        }
        bNewParticleKey=false;
    }
    
    //if not new keyframe
    
    else{
        
        //if not in physics state
        
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
    
    drawObjects(); // render to screen
    shadowLight.disable();
    
    material.end();
    
    camera.end();
    
    shadowLight.unbindShadowMapTexture();
    shader.end();
    
    
    ofDisableLighting();
    ofDisableArbTex();
    
    //bind image textures and draw bullet shapes
    
    material.begin();
    white.bind();
    background.draw();
    white.unbind();
    
    ofPushMatrix();
    
    ofScale(10, 10, 10);
    ofTranslate(60,0,60);
    
    for (int i=0;i<shapes.size();i++){
        face[i].bind();
        shapes[i]->draw();
        face[i].unbind();
    }
    
    //enable to see physics collision wireframes
    world.drawDebug();
    
    ofPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    
    //     debug view of shader scene
    //shadowLight.debugShadowMap();
    
}

void testApp::loadImage(){
    
    //go to next image in vector and create new particles
    if(imgCount>images.size()-1){
        imgCount=0;
    }
    particleKeyframe=0;
    pic.loadImage(images[imgCount]);
    pic.setImageType(OF_IMAGE_COLOR);
    //    pic.resize(pic.width, pic.height);
    pic.mirror(false, false);
    initParticles();
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
    
    //    //loads camera keyframe positions into vector
    //    loadCamKeyframes();
    
    //set up GL lighting and materials
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    //    light.setDirectional();
    //    light.setOrientation(ofPoint(0,0,-180));
    light.setSpotlight();
    light.setPosition(ofGetWidth()/2,ofGetHeight()/2,500);
    light.lookAt(ofVec3f(ofGetWidth()/2,ofGetHeight()/2,-100));
    
    light2.setDiffuseColor(ofColor(255,255,255));
    light2.setSpecularColor(ofColor(255,255,255));
    light2.setSpotlight();
    light2.setPosition(0,ofGetHeight()/2,-100);
    light.lookAt(ofVec3f(200,ofGetHeight()/2,-40));
    
    
    material.setShininess(.1);
    
    
    //initialize Bullet Physics world
    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    
    //Create static collision objects
    //    background.create(world.world,ofVec3f(0,0,-90.),0.,3000,3000,2.);
    //    background.setProperties(1, 1);
    //    background.add();
    
    whiteImg.loadImage("textures/white.jpg");
    white=whiteImg.getTextureReference();
    
    
    
}


//SETUP PARTICLES OFF IMAGE

void testApp::initParticles(){
    
    cout<<shapes.size()<<endl;
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
            temp.pos.x=x*tileW*boxScale.x+pic.getWidth()/2+image.pos.x;
            temp.pos.y=y*tileH*boxScale.y-pic.getHeight()/2+image.pos.y;
            temp.pos.z=image.pos.z;
            
            //create Bullet shapes for image visualization
            shapes.push_back( new ofxBulletBox() );
            ((ofxBulletBox*)shapes[shapes.size()-1])->init(boxShape);
            btTransform trans;
            
            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world,particleKeyframes[particleKeyframe].pos,10.);
            
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


//DRAW HASHTAG LETTERS

void testApp::drawHashtag(){
    
    glPushMatrix();
    //      camera.enableOrtho();
    ofVec3f scale=meshDrawScale;
    ofMaterial hashMat;
    hashMat.setShininess(99);
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glEnable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);
    
    hashMat.begin();
    
    white.bind();
    
    //align hashtag ofMeshes (only non-bullet items used) with bullet collision shapes
    
    for(int i = 0; i < 12; i++) {
        ofPushMatrix();
        
        //        btScalar	m[16];
        //        ofGetOpenGLMatrixFromRigidBody( hashCollision[i]->getRigidBody(), m );
        glPushMatrix();
        glTranslatef(hashMeshPos[i].pos.x,hashMeshPos[i].pos.y,hashMeshPos[i].pos.z);
        ofScale(scale.x,scale.y,scale.z);
        glRotatef(180,0,0,1);
        hashModel[i].getMesh(0).draw();
        glPopMatrix();
        
    }
    white .unbind();
    glPopAttrib();
    hashMat.end();
    glPopMatrix();
    
    //    camera.disableOrtho();
    
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
    
    hashtagGUI = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
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
    
    //    ofDisableArbTex();
    for(int i = 0; i < 12; i++){
        
        //LOAD POSITION SETTINGS FROM MASTER SETTINGS XML DOCUMENT
        
        settings.pushTag("LETTER", i);
        string loadKey = settings.getValue("NAME","0");
        settings.pushTag("MESH");
        
        settings.pushTag("POS");
        hashletter newHashMesh;
        newHashMesh.pos.x = settings.getValue("X", 0);
        newHashMesh.pos.y = settings.getValue("Y", 0);
        newHashMesh.pos.z = settings.getValue("Z", 0);
        newHashMesh.key=loadKey+"MESH";
        settings.popTag();
        settings.popTag();
        
        settings.pushTag("COLLISION");
        settings.pushTag("POS");
        hashletter newHashCollision;
        newHashCollision.pos.x = settings.getValue("X", 0);
        newHashCollision.pos.y = settings.getValue("Y", 0);
        newHashCollision.pos.z = settings.getValue("Z", 0);
        newHashCollision.key=loadKey+"COLL";
        settings.popTag();
        settings.popTag();
        
        string file=settings.getValue("MESH"," ");
        settings.popTag();
        
        //        hashModel[i].loadModel("OBJ_letters/A_lower/A_final_2.obj",true );
        hashModel[i].loadModel("models/cannes_placementCorrected.obj", true);
        cout<<"meshes:"<<hashModel[i].getNumMeshes()<<endl;
        
        ofVec3f scale=meshCollisionScale;
        ofPoint min=hashModel[i].getSceneMin();
        ofPoint max=hashModel[i].getSceneMax();
        newHashMesh.size=max-min;
        ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
        
        hashMeshPos.push_back(newHashMesh);
        hashCollisionPos.push_back(newHashCollision);
        
        cout<<newHashCollision.pos<<endl;
        
        hashCollision.push_back( new ofxBulletCustomShape() );
        //        hashCollision[i]->init(new btCompoundShape, ofVec3f(hashModel[i].getSceneCenter()));
        //        hashCollision[i]->setProperties(.3, 1.);
        hashCollision[i]->addMesh(hashModel[i].getMesh(0), scale, true);
        hashCollision[i]->create( world.world, ofVec3f(newHashCollision.pos.x,newHashCollision.pos.y,newHashCollision.pos.z),startRot,0.);
        hashCollision[i]->add();
        cout<< hashCollision[i]->getRestitution()<<endl;
        
        //ADD HASHTAG SETTINGS TO GUI
        
        hashtagGUI->addWidgetDown(new ofxUIToggle( dim, dim, false, newHashMesh.key))->setID(i);
        hashtagGUI->addWidgetDown(new ofxUIToggle( dim, dim, false, newHashCollision.key))->setID(i);
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
    if(num!=-1&&num%2==0){
        hashCollisionPos[num].active=toggle->getValue();
    }
    
    else if(num!=-1&&num%2==1){
        hashMeshPos[num].active=toggle->getValue();
    }
    
    else{
        hash.active=toggle->getValue();
    }
}


//USER INPUT FUNCTIONS

void testApp::mousePressed(int x, int y, int button) {
    
}

void testApp::keyPressed(int key){
    //    for(int i = 0; i < 12; i++){
    //
    //        btTransform trans;
    //        ofPoint temp=hashCollisionPos.pos+hash.pos;
    //        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
    //        ofQuaternion rotQuat = hashCollision[i]->getRotationQuat();
    //        float newRot = rotQuat.w();
    //        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
    //        hashCollision[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
    //        cout<<"move"<<endl;
    //    }
    switch(key){
            
        case 'f':
            bSingle=false;
            return;
            
        case 'c':
            camState++;
            if (camState>2){
                camState=0;
            }
            bCamSwitch=true;
            return;
            
        case 'l':
//            lPaused = !lPaused;
            return;
            
        case ' ':
            settings.pushTag("LETTERS");
            for(int i = 0; i < hashCollisionPos.size(); i++){
                settings.pushTag("LETTER", i);
                settings.pushTag("MESH");
                settings.pushTag("POS");
                settings.setValue("X", hashMeshPos[i].pos.x);
                settings.setValue("Y", hashMeshPos[i].pos.y);
                settings.setValue("Z", hashMeshPos[i].pos.z);
                settings.popTag();
                settings.popTag();
                settings.pushTag("MESH");
                settings.pushTag("COLLISION");
                settings.setValue("X", hashCollisionPos[i].pos.x);
                settings.setValue("Y", hashCollisionPos[i].pos.y);
                settings.setValue("Z", hashCollisionPos[i].pos.z);
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
            if(hash.active){
                if(!bSingle){
                    hash.pos.y-=10;
                }
                else{
                    hash.pos.y--;
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
            return;
    }
    
}

void testApp::keyReleased(int key){
    switch(key){
        case 'f':
            bSingle=true;
            return;
        case 'h':
            hashtagGUI->toggleVisible();
            break;
            
    }
}


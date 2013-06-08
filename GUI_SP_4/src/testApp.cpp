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
    
    bDebug=false;
        
    collisionPlane.loadModel("OBJ_letters/collision.obj");
    
    meshCollisionScale=ofPoint(.6,.6 ,.6);
    meshDrawScale=ofPoint(10,10,20);
    image.pos=ofPoint(-10,-10,-55);
    boxScale=ofPoint(.1,.1,.1);
    ofSetBackgroundColor(255);
    
    camState=0;
    
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
    
    //camera, lighting, dof setup
    setupGL();
    
    //GUI and hashtag mesh loading
    loadHashtagGUI();
    hashtagGUI->loadSettings("GUI/hashtagGUI.xml");
    hashtagGUI->toggleVisible();
    
    //camKeyframe cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    //particle keyframes animate sequence of particle behaviors for individual image or tweet
    camKeyframe=0;
    particleKeyframe=0;
    loadParticleKeyframes();
    
    //populates vector of images off of specified directory
    imgCount=0;
    loadDir();
    loadImage();
    
    if(camState==0){
        camera2.enableOrtho();
        camPos=ofPoint(10,0,0);
        ofPoint look=ofPoint(0,0,-80);
        
        camera2.setPosition(camPos);
        camera2.setNearClip(-1000);
        camera2.setFarClip(2000);
        camera2.lookAt(look);
        
        camera.setPosition(camPos);
        camera.lookAt(look);
        camera.setFarClip(1000.);
        //        camera.setFov(50);
    }
    
    else if(camState==1){
        camera2.enableOrtho();
        camPos=ofPoint(50,0,-40);
        ofPoint look=ofPoint(0,0,-40);
        camera2.setPosition(camPos);
        camera2.setNearClip(-1000);
        camera2.setFarClip(1000);
        camera2.lookAt(look);
        
        camera.setPosition(camPos);
        camera.lookAt(look);
        //    camera.setFov(120);
        //        camera.setFarClip(1000.);
    }
    else if(camState==2){
        camera2.enableOrtho();
        camPos=ofPoint(0,0,0);
        ofPoint look=ofPoint(0,0,-40);
        camera2.setPosition(camPos);
        camera2.setNearClip(-1000);
        camera2.setFarClip(1000);
        camera2.lookAt(look);
        
        camera.setPosition(camPos);
        camera.lookAt(look);
        //    camera.setFov(120);
        camera.setFarClip(1000.);
    }
    
    if (USE_DOF) {
        dof.setup(ofGetWidth(),ofGetHeight());
        dof.setFocalDistance(10.9);
        dof.setFocalRange(4.5);
        dof.setBlurAmount(.7);
    }
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
        camera2.disableMouseInput();
    }
    
    bRot=false;
}


//--------------------------------------------------------------
void testApp::update(){
    
    world.update();    //updates bullet objects
    
    
    //check whether individual particles have completed the duration of their transition
    int moving=0;
    for(int i=0;i<particles.size();i++){
        if(!particles[i].targetReached){
            moving++;
        }
    }
    
    for(int i=0;i<hashletters.size();i++){
        if(hashletters[i][0].scale.z<5.){
            hashletters[i][0].scale.z+=.0333;
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
            if(bDebug==false){
                loadImage();
            }
        }
        
        else if (particleKeyframes[particleKeyframe].path==PARTICLE_PATH_EXPLODE){
            world.setGravity(ofVec3f(0,50,0));
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
                shapes[i]->applyForce(ofVec3f(500,500,500),image.pos);
                shapes[i]->setProperties(.1,1);
                shapes[i]->add();
                particles[i].goToPosition(particleKeyframes[particleKeyframe]);
            }
            cout<<"GO"<<endl;
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
            

        }
    
        //if switching into physics state
        
        else if(particleKeyframes[particleKeyframe].path==PARTICLE_PATH_PHYSICS){
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
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    ofEnableLighting();
    dof.begin();
    
    
    if (USE_DOF) {
        
        camera.begin(dof.getDimensions());
    } else {
        camera.begin();
    }
    
    //      enable to see physics collision wireframes
    
    if(bDebug==true){
        world.drawDebug();
    }
    
    //draw lighting


    light.enable();
    light3.enable();
    if(bDebug==true){
        light.draw();
    }
    
    material.begin();
    white.bind();
    background.draw();
    white.unbind();
    
    //bind image textures and draw bullet shapes
    for (int i=0;i<shapes.size();i++){
        face[i].bind();
        shapes[i]->draw();
        face[i].unbind();
    }
    
    material.end();
        light.disable();
    light3.disable();
    camera.end();
    
    camera2.begin();
        light2.enable();
    if(bDebug==true){
        light2.draw();
    }
    drawHashtag();
    camera2.end();
    light2.disable();
    
    ofDisableLighting();
    ofDisableArbTex();
    
    if (USE_DOF) {
        dof.end();
        //fbo of dof for drawing
        ofFbo temp=dof.getFbo();
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    
    glDisable(GL_DEPTH_TEST);
    
    //show framerate
//    ofDrawBitmapString(ofToString(ofGetFrameRate()),ofPoint(800,100,0));
}

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
//        light.setDirectional();
//        light.setOrientation(ofPoint(0,0,0));
    light.setSpotlight();
    light.setPosition(0,0,0);
    light.lookAt(ofVec3f(0,0,-500));

    light2.setDiffuseColor(ofColor(150,150,150));
    light2.setSpecularColor(ofColor(255,255,255));
    light2.setSpotlight();
    light2.setPosition(1500,800,-1900);
    light2.lookAt(ofVec3f(-100,400,-100));
    
    //set up GL lighting and materials
    light3.setDiffuseColor(ofColor(255,255,0));
    light3.setSpecularColor(ofColor(255,255,0));
    //        light.setDirectional();
    //        light.setOrientation(ofPoint(0,0,0));
    light3.setSpotlight();
    light3.setPosition(0,0,-60);
    light3.lookAt(ofVec3f(0,0,-500));
    
    material.setShininess(.1);
    
    
    //initialize Bullet Physics world
    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    
    //Create static collision objects
    background.create(world.world,ofVec3f(0,0,-90.),0.,3000,3000,0.);
    background.setProperties(1, 1);
    background.add();
    
//    ground.create(world.world,ofVec3f(0,0,-40.),0.,3000,3000,3.);
//    ground.setProperties(.2, .2);
//    ground.add();
    
    
    whiteImg.loadImage("textures/white.jpg");
    white=whiteImg.getTextureReference();
    
}


//SETUP PARTICLES OFF IMAGE

void testApp::initParticles(){
    
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
            btTransform trans;
            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world,particleKeyframes[particleKeyframe].pos,10.);
            shapes[shapes.size()-1]->setProperties(.1,.1);
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
    
    ofMaterial hashMat;
    hashMat.setShininess(2);
    
    hashMat.begin();
    //align hashtag ofMeshes (only non-bullet items used) with bullet collision shapes
    
    for(int i = 0; i < hashletters.size(); i++) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        glDisable(GL_CULL_FACE);
            ofVec3f scale=hashletters[i][0].scale;
        //        btScalar	m[16];
        //        ofGetOpenGLMatrixFromRigidBody( hashCollision[i]->getRigidBody(), m );
        glPushMatrix();
        glTranslatef(hashletters[i][0].pos.x,hashletters[i][0].pos.y,hashletters[i][0].pos.z);
        ofScale(scale.x,scale.y,scale.z);
        glRotatef(hashletters[i][0].rot.x,1,0,0);
        glRotatef(hashletters[i][0].rot.y,0,1,0);
        glRotatef(hashletters[i][0].rot.z,0,0,1);
        white.bind();
        hashModel[i].getMesh(0).drawFaces();
        white .unbind();
        glPopMatrix();
        glPopAttrib();

//        glPushMatrix();
//        glClearColor(0,0,0,.5);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_ONE, GL_ZERO);
//        glTranslatef(hashletters[i][0].pos.x+48,hashletters[i][0].pos.y, hashletters[i][0].pos.z+400);
//        ofScale(scale.x,scale.y,scale.z*.1);
//        glRotatef(hashletters[i][0].rot.x,1,0,0);
//        glRotatef(hashletters[i][0].rot.y,0,1,0);
//        glRotatef(hashletters[i][0].rot.z,0,0,1);
//        hashModel[i].getMesh(0).drawFaces();
//        ofSetColor(255,255,255,255);
//        glPopMatrix();
        
    }
        hashMat.end();
    glPopMatrix();
    ofEnableAlphaBlending();
    

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
        settings.pushTag("MESH");
        
        settings.pushTag("POS");
        hashletter newHashMesh;
        newHashMesh.pos.x = settings.getValue("X", 0.);
        newHashMesh.pos.y = settings.getValue("Y", 0.);
        newHashMesh.pos.z = settings.getValue("Z", 0.);
        newHashMesh.key=loadKey+" MESH:";
        settings.popTag();
        
        settings.pushTag("ROTATION");
        newHashMesh.rot.x = settings.getValue("X", 0.);
        newHashMesh.rot.y = settings.getValue("Y", 0.);
        newHashMesh.rot.z = settings.getValue("Z", 0.);
        settings.popTag();
        
        settings.pushTag("SCALE");
        newHashMesh.scale.x = settings.getValue("X", 1.);
        newHashMesh.scale.y = settings.getValue("Y", 1.);
//        newHashMesh.scale.z = settings.getValue("Z", 1.);
        newHashMesh.scale.z = 0.1;
        settings.popTag();
        
        settings.popTag();
        
        settings.pushTag("COLLISION");
        
        settings.pushTag("POS");
        hashletter newHashCollision;
        newHashCollision.pos.x = settings.getValue("X", 0.);
        newHashCollision.pos.y = settings.getValue("Y", 0.);
        newHashCollision.pos.z = settings.getValue("Z", 0.);
        newHashCollision.key=loadKey+" COLLISION:";
        settings.popTag();
        
        settings.pushTag("ROTATION");
        newHashCollision.rot.x = settings.getValue("X", 0.);
        newHashCollision.rot.y = settings.getValue("Y", 0.);
        newHashCollision.rot.z = settings.getValue("Z", 0.);
        newHashCollision.key=loadKey+" COLLISION:";
        settings.popTag();
        
        settings.pushTag("SCALE");
        newHashCollision.scale.x = settings.getValue("X", 1.);
        newHashCollision.scale.y = settings.getValue("Y", 1.);
        newHashCollision.scale.z = settings.getValue("Z", 1.);
        newHashMesh.key=loadKey+" MESH:";
        
        settings.popTag();
        settings.popTag();
        
        
        string file=settings.getValue("OBJ"," ");
        settings.popTag();
        
        hashModel[i].loadModel(file,true );
        
        ofPoint scale=newHashCollision.scale;
        
        ofPoint min=hashModel[i].getSceneMin();
        ofPoint max=hashModel[i].getSceneMax();
        newHashMesh.size=max-min;
        
        vector<hashletter> pushMeshes;
        
        newHashCollision.active=false;
        newHashMesh.active=false;
        
        pushMeshes.push_back(newHashMesh);
        pushMeshes.push_back(newHashCollision);
        
        hashletters.push_back(pushMeshes);
        
        hashCollision.push_back( new ofxBulletCustomShape() );
        funnelCollision.push_back (new ofxBulletBox() );
        
        btTransform trans;
        ofPoint temp=hashletters[i][1].pos;
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofPoint rot = hashletters[i][1].rot;
        btQuaternion rotate=btQuaternion(rot.x,rot.y,rot.z,ofDegToRad(1));
        trans.setRotation(rotate);
        
        ofQuaternion startRot=ofQuaternion(newHashCollision.rot.x,newHashCollision.rot.y,newHashCollision.rot.z,ofDegToRad(1));
        hashCollision[i]->addMesh(hashModel[i].getMesh(0), scale, false);
        hashCollision[i]->create( world.world, trans,0.);
        hashCollision[i]->add();
        hashCollision[i]->setProperties(.1, 1);
//        funnelCollision[i]->init(5,10,.5);
//        funnelCollision[i]->create(world.world, ofVec3f(temp.x,temp.y-5.5,temp.z+2.2), ofQuaternion(1,0,0,-PI/8),0.);
//        funnelCollision[i]->add();
//        funnelCollision[i]->setProperties(1,.1);
        
        //ADD HASHTAG SETTINGS TO GUI
        
        hashtagGUI->addToggle( newHashMesh.key, false, dim, dim)->setID(idCount);
    	hashtagGUI->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        idCount++;
        hashtagGUI->addToggle( newHashCollision.key, false, dim, dim)->setID(idCount);
        idCount++;
        hashtagGUI->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
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
        hashletters[num/2][num%2].active=toggle->getValue();
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
                
                
                settings.pushTag("MESH");
                
                settings.pushTag("POS");
                settings.setValue("X", hashletters[i][0].pos.x);
                settings.setValue("Y", hashletters[i][0].pos.y);
                settings.setValue("Z", hashletters[i][0].pos.z);
                settings.popTag();
                
                settings.pushTag("ROTATION");
                settings.setValue("X", hashletters[i][0].rot.x);
                settings.setValue("Y", hashletters[i][0].rot.y);
                settings.setValue("Z", hashletters[i][0].rot.z);
                settings.popTag();
                
                settings.pushTag("SCALE");
                settings.setValue("X", hashletters[i][0].scale.x);
                settings.setValue("Y", hashletters[i][0].scale.y);
                settings.setValue("Z", hashletters[i][0].scale.z);
                settings.popTag();
                
                settings.popTag();
                
                
                settings.pushTag("COLLISION");
                
                settings.pushTag("POS");
                settings.setValue("X", hashletters[i][1].pos.x);
                settings.setValue("Y", hashletters[i][1].pos.y);
                settings.setValue("Z", hashletters[i][1].pos.z);
                settings.popTag();
                
                settings.pushTag("ROTATION");
                settings.setValue("X", hashletters[i][1].rot.x);
                settings.setValue("Y", hashletters[i][1].rot.y);
                settings.setValue("Z", hashletters[i][1].rot.z);
                settings.popTag();
                
                settings.pushTag("SCALE");
                settings.setValue("X", hashletters[i][1].scale.x);
                settings.setValue("Y", hashletters[i][1].scale.y);
                settings.setValue("Z", hashletters[i][1].scale.z);
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
                for(int j=0;j<2;j++){
                    
                    
                    if(hashletters[i][j].active&&bScale==true){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.y+=.01;
                            }
                            else{
                                hashletters[i][j].scale.y+=.001;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.y+=1;
                            }
                            else{
                                hashletters[i][j].scale.y+=.1;
                            }
                        }
                    
                    }
                    
                    else if(hashletters[i][j].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i][j].rot.y-=10;
                        }
                        else{
                            hashletters[i][j].rot.y--;
                        }
                        if(j==1){
                            updateCollision(i);
                        }
                    }
                    
                    else if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].pos.y--;
                            }
                            else{
                                hashletters[i][j].pos.y-=.1;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].pos.y-=10;
                            }
                            else{
                                hashletters[i][j].pos.y--;
                            }
                        }
                    }
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
                for(int j=0;j<2;j++){
                    
                    
                    if(hashletters[i][j].active&&bScale==true){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.y-=.01;
                            }
                            else{
                                hashletters[i][j].scale.y-=.001;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.y-=1;
                            }
                            else{
                                hashletters[i][j].scale.y-=.1;
                            }
                        }
                        
                    }
                    
                    else if(hashletters[i][j].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i][j].rot.y+=10;
                        }
                        else{
                            hashletters[i][j].rot.y++;
                        }
                        if(j==1){
                            updateCollision(i);
                        }
                    }
                    
                    else if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].pos.y++;
                            }
                            else{
                                hashletters[i][j].pos.y+=.1;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].pos.y+=10;
                            }
                            else{
                                hashletters[i][j].pos.y++;
                            }
                        }
                    }
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
                for(int j=0;j<2;j++){
                    
                    
                    if(hashletters[i][j].active&&bScale==true){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.x+=.01;
                            }
                            else{
                                hashletters[i][j].scale.x+=.001;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.x+=1;
                            }
                            else{
                                hashletters[i][j].scale.x+=.1;
                            }
                        }
                        
                    }
                    
                    else if(hashletters[i][j].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i][j].rot.x+=10;
                        }
                        else{
                            hashletters[i][j].rot.x++;
                        }
                        if(j==1){
                            updateCollision(i);
                        }
                    }
                    
                    else if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].pos.x++;
                            }
                            else{
                                hashletters[i][j].pos.x+=.1;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].pos.x+=10;
                            }
                            else{
                                hashletters[i][j].pos.x++;
                            }
                        }
                    }
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
                for(int j=0;j<2;j++){
                    
                    
                    if(hashletters[i][j].active&&bScale==true){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.x-=.01;
                            }
                            else{
                                hashletters[i][j].scale.x-=.001;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.x-=1;
                            }
                            else{
                                hashletters[i][j].scale.x-=.1;
                            }
                        }
                        
                    }
                    
                    else if(hashletters[i][j].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i][j].rot.x-=10;
                        }
                        else{
                            hashletters[i][j].rot.x--;
                        }
                        if(j==1){
                            updateCollision(i);
                        }
                    }
                    
                    else if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].pos.x--;
                            }
                            else{
                                hashletters[i][j].pos.x-=.1;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].pos.x-=10;
                            }
                            else{
                                hashletters[i][j].pos.x--;
                            }
                        }
                    }
                }
            }            return;
            
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
                for(int j=0;j<2;j++){
                    
                    
                    if(hashletters[i][j].active&&bScale==true){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.z-=.01;
                            }
                            else{
                                hashletters[i][j].scale.z-=.001;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.z-=1;
                            }
                            else{
                                hashletters[i][j].scale.z-=.1;
                            }
                        }
                        
                    }
                    
                    else if(hashletters[i][j].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i][j].rot.z-=10;
                        }
                        else{
                            hashletters[i][j].rot.z--;
                        }
                        if(j==1){
                            updateCollision(i);
                        }
                    }
                    
                    else if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].pos.z--;
                            }
                            else{
                                hashletters[i][j].pos.z-=.1;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].pos.z-=10;
                            }
                            else{
                                hashletters[i][j].pos.z--;
                            }
                        }
                    }
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
                for(int j=0;j<2;j++){
                    
                    if(hashletters[i][j].active&&bScale==true){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.z+=.01;
                            }
                            else{
                                hashletters[i][j].scale.z+=.001;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.z+=1;
                            }
                            else{
                                hashletters[i][j].scale.z+=.1;
                            }
                        }
                        
                    }
                    
                    else if(hashletters[i][j].active&&bRot==true){
                        if(!bSingle){
                            hashletters[i][j].rot.z+=10;
                        }
                        else{
                            hashletters[i][j].rot.z++;
                        }
                        if(j==1){
                            updateCollision(i);
                        }
                    }
                    
                    else if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].pos.z++;
                            }
                            else{
                                hashletters[i][j].pos.z+=.1;
                            }
                            updateCollision(i);
                        }
                        else{
                            if(!bSingle){
                                hashletters[i][j].pos.z+=10;
                            }
                            else{
                                hashletters[i][j].pos.z++;
                            }
                        }
                    }
                }
            }
        return;
            
            
        case ']':
        case '}':
            for(int i=0;i<hashletters.size();i++){
                for(int j=0;j<2;j++){
                    
                    if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.x+=.1;
                                hashletters[i][j].scale.y+=.1;
                                hashletters[i][j].scale.z+=.1;
                            }
                            else{
                                hashletters[i][j].scale.x+=.01;
                                hashletters[i][j].scale.y+=.01;
                                hashletters[i][j].scale.z+=.01;
                            }
                            updateCollision(i);
                        }
                
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.x+=1;
                                hashletters[i][j].scale.y+=1;
                                hashletters[i][j].scale.z+=1;
                            }
                            else{
                                hashletters[i][j].scale.x+=.1;
                                hashletters[i][j].scale.y+=.1;
                                hashletters[i][j].scale.z+=.1;
                            }
                        }
                    }
                }
            }

            return;
            
        case '[':
        case '{':
            for(int i=0;i<hashletters.size();i++){
                for(int j=0;j<2;j++){
                    
                    if(hashletters[i][j].active){
                        if(j==1){
                            if(!bSingle){
                                hashletters[i][j].scale.x-=.1;
                                hashletters[i][j].scale.y-=.1;
                                hashletters[i][j].scale.z-=.1;
                            }
                            else{
                                hashletters[i][j].scale.x-=.01;
                                hashletters[i][j].scale.y-=.01;
                                hashletters[i][j].scale.z-=.01;
                            }
                            updateCollision(i);
                        }
                        
                        else{
                            if(!bSingle){
                                hashletters[i][j].scale.x-=1;
                                hashletters[i][j].scale.y-=1;
                                hashletters[i][j].scale.z-=1;
                            }
                            else{
                                hashletters[i][j].scale.x-=.1;
                                hashletters[i][j].scale.y-=.1;
                                hashletters[i][j].scale.z-=.1;
                            }
                        }
                    }
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
            hashtagGUI->toggleVisible();
            break;
        case 'd':
            bDebug=!bDebug;
            break;
            
    }
}

void testApp::updateCollision(int i){
    btTransform trans;
    ofPoint temp=hashletters[i][1].pos;
    trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
    ofPoint rot = hashletters[i][1].rot;
    btQuaternion rotate=btQuaternion(rot.x,rot.y,rot.z,ofDegToRad(1));
    trans.setRotation(rotate);
    ofVec3f scale=hashletters[i][1].scale;
    hashCollision[i]->remove();
    hashCollision[i]->init();
    hashCollision[i]->create(world.world,trans,0.);
    hashCollision[i]->addMesh(hashModel[i].getMesh(0), scale, false);
//            hashCollision[i]->addMesh(collisionPlane.getMesh(0), scale, false);
    hashCollision[i]->add();
    hashCollision[i]->setProperties(.1, 1);
}

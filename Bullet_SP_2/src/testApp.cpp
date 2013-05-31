#include "testApp.h"

#define tileW 20
#define tileH 20

#define USE_DOF true
#define CAM_MOVE false
#define CAM_MOUSE false
#define FACES false

#define CAMERA_PATH_LINE 0
#define CAMERA_PATH_CURVE 1
#define CAMERA_RATE_LINEAR 0
#define CAMERA_RATE_EASE 1
#define CAMERA_RATE_SPEED 2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
    
	ofBackground(255);
    
    setupGL();
    
    int w=50;
    int h=50;
    
    //imgCount cycles through the images found in specified image directory
    imgCount=0;
    
    //camKeyframe cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    camKeyframe=0;
    particleKeyframe=0;

    //Loads Meshes of 3DS files for #CannesLions logo
    loadTextMeshes();
    
    //populates vector of images off of specified directory
    loadDir();
    loadImage();
    
    //loads camera keyframe positions into vector
    loadCamKeyframes();
    
    //loads particle behavior keyframes
    //    loadParticleKeyframes();  // REIMPLEMENT to include gravity/physics keyframe
    
    //load init camera position
    camPos=camKeyframes[0].pos;
	camera.setPosition(camPos);
	camera.lookAt(camKeyframes[0].lookAt);
    
    if (USE_DOF) {
		dof.setup(ofGetWidth(), ofGetHeight());
        dof.setFocalDistance(200);
		dof.setFocalRange(1000);
        dof.setBlurAmount(1);
	}
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
    }
    
    whiteImg.loadImage("white.jpg");
    white=whiteImg.getTextureReference();
    
}
 

//--------------------------------------------------------------
void testApp::update(){
    
    world.update();
    
    if(CAM_MOVE) {
        if(camPos.distance(camKeyframes[camKeyframe].pos)<2){
            camKeyframe++;
        }
        if(camKeyframe>camKeyframes.size()-1){
            camKeyframe=0;
        }
        if (camKeyframe==0){
            ofPoint move=camKeyframes[camKeyframe].pos-camKeyframes[camKeyframes.size()-1].pos;
            move.normalize();
            camPos=camPos+camKeyframes[camKeyframe].rate*move;
        }
        else{
            ofPoint move=camKeyframes[camKeyframe].pos-camKeyframes[camKeyframe-1].pos;
            move.normalize();
            camPos=camPos+camKeyframes[camKeyframe].rate*move;
        }
        camera.setPosition(camPos);
        camera.lookAt(camKeyframes[camKeyframe].lookAt);
        ofPoint temp=ofPoint(camKeyframes[camKeyframe].lookAt-camPos);;
        if(USE_DOF){
            dof.setFocalDistance(temp.distance(camKeyframes[camKeyframe].lookAt)/4);
            dof.setFocalRange(1000);
        }
    }
    
    else if(bCamChange==true){
        camera.setPosition(camKeyframes[camKeyframe].pos);
        camera.lookAt(camKeyframes[camKeyframe].lookAt);
        ofPoint temp=ofPoint(camKeyframes[camKeyframe].lookAt-camPos);
        if(USE_DOF){
            dof.setFocalDistance(temp.distance(camKeyframes[camKeyframe].lookAt)/4);
            dof.setFocalRange(1000);
        }
        bCamChange=false;
    }
    
    
    //switch out of Bullet physics and into eased particle functions
    if(bDrawPhysics==false){
        if(bSwitchPhysics==true){
            for (int i=0;i<shapes.size();i++){
                particles[i].bullet.pos=shapes[i]->getPosition();
                
                if(i==0)
                {
                    cout<<"init: "<<shapes[i]->getRotation()<<endl;
                }
                
                particles[i].bullet.quat = shapes[i]->getRotationQuat();

                particles[i].pixel.pos=particles[i].pixel.pos-particles[i].bullet.pos;
                particles[i].goToPosition(particles[i].pixel);
                shapes[i]->enableKinematic();
            }
            bSwitchPhysics=false;
        }
        else{
            for (int i=0;i<shapes.size();i++){
                particles[i].update();
                
                if(particles[i].bRotate==true){
                    ofVec3f pos = shapes[i]->getPosition();
                    btTransform trans;
                    trans.setOrigin( btVector3( btScalar(pos.x), btScalar(pos.y), btScalar(pos.z)) );
                    ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                    float newRot = rotQuat.w();
                    newRot -= .05f;
                    if (newRot<0){
                        newRot=0;
                        particles[i].bRotate=false;
                    }
                    trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                    shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                    shapes[i]->activate();
                }

            }
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    glEnable(GL_DEPTH_TEST);
    ofEnableLighting();
    
	if (USE_DOF) {
		dof.begin();
		camera.begin(dof.getDimensions());
	} else {
		camera.begin();
	}
    
    
    light.enable();
    light2.enable();
    light.draw();
    
    //material for 
    material.begin();
    drawHashtag(-250,500,200);
    white.bind();
    background.draw();
    white.unbind();
    
    for (int i=0;i<shapes.size();i++){
        face[i].bind();
        if(bDrawPhysics==true){
            shapes[i]->draw();
        }
        else{
            ofPushMatrix();
            ofTranslate(particles[i].current.pos);
            shapes[i]->draw();
            ofPopMatrix();
        }
        face[i].unbind();
    }
    
    material.end();
	
	camera.end();
    ofDisableLighting();
    ofDisableArbTex();

	if (USE_DOF) {
        dof.end();
        //fbo of dof for drawing
        ofFbo temp=dof.getFbo();
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
	}
    glDisable(GL_DEPTH_TEST);	
	
}

void testApp::loadImage(){
    
    //go to next image in vector, create particles out of it
    
    if(imgCount>images.size()-1){
        imgCount=0;
    }
    particleKeyframe=0;
    pic.loadImage(images[imgCount]);
    pic.setImageType(OF_IMAGE_COLOR);
    pic.resize(pic.width/2, pic.height/2);
    pic.mirror(false, false);
    initParticles();
    imgCount++;
}

void testApp::loadDir(){
    
    //uses ofDirectory to load image files into vector of strings then closes dir
    
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
    Particle::keyframe start;
    start.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_ABS);
    start.posMin.z=600;
    start.posMax.z=800;
    particleKeyframes.push_back(start);
    
    Particle::keyframe imagePlane;
    imagePlane.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL);
    imagePlane.duration=4000;
    imagePlane.durationMin=0;
    imagePlane.durationMax=2500;
    imagePlane.interpolation = PARTICLE_INTERPOLATE_QUINT_EASE_OUT;
    particleKeyframes.push_back(imagePlane);
    
    Particle::keyframe holdImage;
    holdImage.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL);
    holdImage.duration=2000;
    particleKeyframes.push_back(holdImage);
    
    Particle::keyframe exit;
    exit.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_ABS);
    exit.pos.z=500;
    exit.posMin.z=0;
    exit.posMax.z=100;
    exit.duration=5000;
    exit.durationMin=0;
    exit.durationMax=500;
    exit.interpolation = PARTICLE_INTERPOLATE_ELASTIC_EASE_IN;
    particleKeyframes.push_back(exit);
}

void testApp::loadCamKeyframes(){
    
    bCamChange=false;
    
    //sequentially loads camera positions, rate, and reached into vector of camPoint objects
    
    camPoint newCamPoint;
    newCamPoint.pos=ofPoint(200,400,800);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(200,400,0);
    camKeyframes.push_back(newCamPoint);
    
    newCamPoint.pos=ofPoint(0,-500,100);
    newCamPoint.rate=.5;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(0,0,0);
    camKeyframes.push_back(newCamPoint);
    
}

//--------------------------------------------------------------
void testApp::setupGL(){
    
    ofSetSmoothLighting(true);
    
    //set up GL lighting and material
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setSpotlight();
    light.setPosition(100, -100, 300);
    light.lookAt(ofPoint(-50,550,-200));
    
    light2.setDiffuseColor(ofColor(255,255,255));
    light2.setSpecularColor(ofColor(255,255,255));
    light2.setSpotlight();
    light2.setPosition(700, 0, 200);
    light2.lookAt(ofPoint(850,600,0));
    
    material.setShininess(100);
    material.setSpecularColor(ofColor(255,255,100));
    
    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    world.setGravity(ofVec3f(0,2000,0));
    
    ground.create(world.world, ofVec3f(0., 400., 160.), 0., 100, 28, 80 );
	ground.setProperties(1, 1);
	ground.add();
    groundLow.create(world.world, ofVec3f(200., 400., 160.), 0., 800, 28, 80 );
	groundLow.setProperties(1, 1);
	groundLow.add();
    
    background.create(world.world,ofVec3f(-1000.,-1000.,-500.),0.,10000.,10000.,10.);
    background.setProperties(1, 1);
    background.add();
    
}

void testApp::loadTextMeshes(){
    hashtag.loadModel("OBJ_letters/hashtag/hashtag.3ds",100);
    hashtag.setRotation(1, 180, 1, 0, 0);
    C_upper.loadModel("OBJ_letters/C_upper/C_upper.3ds",100);
    C_upper.setRotation(1, 180, 1, 0, 0);
    A_lower.loadModel("OBJ_letters/A_lower/A_lower.3ds",100);
    A_lower.setRotation(1, 180, 1, 0, 0);
    N_lower.loadModel("OBJ_letters/N_lower/N_lower.3ds",100);
    N_lower.setRotation(1, 180, 1, 0, 0);
    E_lower.loadModel("OBJ_letters/E_lower/E_lower.3ds",100);
    E_lower.setRotation(1, 180, 1, 0, 0);
    S_lower.loadModel("OBJ_letters/S_lower/S_lower.3ds",100);
    S_lower.setRotation(1, 180, 1, 0, 0);
    L_upper.loadModel("OBJ_letters/L_upper/L_upper.3ds",100);
    L_upper.setRotation(1, 180, 1, 0, 0);
    I_lower.loadModel("OBJ_letters/I_lower/I_lower.3ds",100);
    I_lower.setRotation(1, 180, 1, 0, 0);
    O_lower.loadModel("OBJ_letters/O_lower/O_lower.3ds",100);
    O_lower.setRotation(1, 180, 1, 0, 0);
}

void testApp::initParticles(){
    
    shapes.clear();
    
    //particle bools
    bDrawPhysics=true;
    bSwitchPhysics=false;
    bStart=true;
    
    boxShape = ofBtGetBoxCollisionShape(tileW, tileH, tileW);
    
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
            //crop image for texture on particle
			ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            shapes.push_back( new ofxBulletBox() );
            ((ofxBulletBox*)shapes[shapes.size()-1])->init(boxShape);
            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world, ofVec3f(ofRandom(0, ofGetWidth()), ofRandom(-200,-600), ofRandom(150,160)), 1);
            shapes[shapes.size()-1]->setActivationState( DISABLE_DEACTIVATION );
            shapes[shapes.size()-1]->add();
            tileImage.mirror(false,true);
            ofEnableNormalizedTexCoords();
            ofTexture newFace;
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            face.push_back(newFace);
            
            Particle::keyframe temp;
            temp.pos.x=x*tileW;
            temp.pos.y=y*tileH;
            temp.pos.z=0;
            temp.duration=10000;
            temp.interpolation=PARTICLE_INTERPOLATE_EXPO_EASE_OUT;
            Particle particle;
            particle.setup(temp,x+x*y);
            particles.push_back(particle);
        }
    }
}

void testApp::drawHashtag(ofPoint pos){
    
    ofPushMatrix();
    ofTranslate(pos);
    
    glPushMatrix();
    glTranslatef(0,0,0);
    hashtag.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(100,0,0);
    C_upper.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(200,0,0);
    A_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(300,0,0);
    N_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(400,0,0);
    N_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(500,0,0);
    E_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(600,0,0);
    S_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(700,0,0);
    L_upper.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(800,0,0);
    I_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(900,0,0);
    O_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1000,0,0);
    N_lower.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1100,0,0);
    S_lower.draw();
    glPopMatrix();
    
    ofPopMatrix();
    
}

void testApp::drawHashtag(int x, int y, int z){
    drawHashtag(ofPoint(x,y,z));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	world.setGravity(ofVec3f(0, 0, 0));
    bDrawPhysics=false;
    if(bStart==true){
    bSwitchPhysics=true;
        bStart=false;
    }
}

void testApp::keyReleased(int key){
    switch(key){
            case ' ':
            camKeyframe++;
            if(camKeyframe>camKeyframes.size()-1){
                camKeyframe=0;
            }
            bCamChange=true;
            return;
    }
}
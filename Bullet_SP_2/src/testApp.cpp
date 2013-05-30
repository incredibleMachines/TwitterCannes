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
    
    //camCount cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    camKeyframe=0;

    
    //populates vector of images off of specified director and loads camera positions into vector
//    loadParticleKeyframes();
    loadDir();
    loadImage();
    loadCamKeyframes();
    
    hashtag.loadModel("OBJ_letters/O_lower/O_lower.3ds",100);
    
    ofIndexType index=0;

    
    //load init camera position
    camPos.x=camKeyframes[0].pos.x;
    camPos.y=camKeyframes[0].pos.y;
    camPos.z=camKeyframes[0].pos.z;
	camera.setPosition(camPos);
	camera.lookAt(ofPoint(pic.width/2,pic.height/2,0));
    
    //setup image boxes
    
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
            temp.interpolation=PARTICLE_INTERPOLATE_LINEAR;
            Particle particle;
            particle.setup(temp,x+x*y);
            particles.push_back(particle);
        }
    }
    
    if (USE_DOF) {
		dof.setup(ofGetWidth(), ofGetHeight());
        dof.setFocalDistance(200);
		dof.setFocalRange(100);
        dof.setBlurAmount(1);
	}
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
    }
    
    camKeyframe++;
    particleKeyframe=0;
    
    bZ=true;
    zPos=-100;
    
    drawPhysics=true;
    bSwitchPhysics=false;

}
 

//--------------------------------------------------------------
void testApp::update(){
    
    
//    // Set this flag to true and flip it to false if any of the particles haven't arrived
//    bool allParticlesArrived = true;
//	
//    // Loop through and update all particles
//    // Check if all have reached their target for the current keyframe
//    for(int i=0; i<particles.size(); i++){
//        particles[i].update();
//		if (!particles[i].targetReached) {
//            allParticlesArrived = false;
//		}
//    }
//    
//    // Advance the keyframe counter and set the next position for each particle
//    if(allParticlesArrived){
//        particleKeyframe++;
//        for(int i=0; i<particles.size(); i++){
//            particles[i].goToPosition(particleKeyframes[particleKeyframe]);
//        }
//        
//    }
    
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
        dof.setFocalDistance(temp.distance(camKeyframes[camKeyframe].lookAt)/4);
		dof.setFocalRange(100);
    }
    
    world.update();
    
    if(drawPhysics==false){
        for (int i=0;i<shapes.size();i++){
            if(bSwitchPhysics==true){
                world.disableCollisionEvents();
                shapes[i]->enableKinematic();
                particles[i].bullet.pos=shapes[i]->getPosition();
                
                ofPoint tempRot=shapes[i]->getRotation();
                tempRot.x=ofRadToDeg(tempRot.x);
                tempRot.y=ofRadToDeg(tempRot.y);
                tempRot.z=ofRadToDeg(tempRot.z);
                if(i==0)
                {
                    cout<<"init: "<<shapes[i]->getRotation()<<endl;
                }
                particles[i].bullet.rotation=tempRot;
                particles[i].pixel.pos=particles[i].pixel.pos-particles[i].bullet.pos;
                particles[i].goToPosition(particles[i].pixel);
            }
            else{
                particles[i].update();
            }
            if(i==0)
            {
                cout<<"c: "<<particles[i].current.pos<<endl;
                cout<<"t: "<<particles[i].target.pos<<endl;
                cout<<"s: "<<particles[i].start.pos<<endl;
                cout<<"b: "<<particles[i].bullet.rotation<<endl;
            }
        }
        bSwitchPhysics=false;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //GL
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
    
    material.begin();
    
    whiteMat.begin();
    
    glPushMatrix();
    glTranslatef(0,400,200);
    hashtag.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(100,400,200);
    hashtag.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(200,400,200);
    hashtag.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(300,400,200);
    hashtag.draw();
    glPopMatrix();
    
    whiteMat.end();

    material.begin();
    
    for (int i=0;i<shapes.size();i++){
        face[i].bind();
        if(drawPhysics==true){
            shapes[i]->draw();
        }
        else{

            
            glPushMatrix();
            glTranslatef(particles[i].bullet.pos.x,particles[i].bullet.pos.y,particles[i].bullet.pos.z);
            glRotatef(-50,1,0,0);
            glRotatef(-particles[i].bullet.rotation.y,0,1,0);
            glRotatef(-particles[i].bullet.rotation.z,0,0,1);
            glPushMatrix();
            glTranslatef(-particles[i].bullet.pos.x,-particles[i].bullet.pos.y,-particles[i].bullet.pos.z);
            shapes[i]->draw();
            glPopMatrix();
            glPopMatrix();
        }
        face[i].unbind();
    }
    
//    ground.draw();


//    for(int i=0; i<particles.size(); i++){
//        particles[i].draw();
//    }
    
    material.end();
	
	camera.end();
    ofDisableLighting();
    ofDisableArbTex();

	if (USE_DOF) {
        dof.end();
        //fbo of dof for drawing
        ofFbo temp=dof.getFbo();
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
//
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
    
//    createParticles();
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

void testApp::createParticles(){
    int particleIndex = 0;
    int particleCount = pic.width / tileW * pic.height / tileH;
    
    particles.clear();
    
    boxShape = ofBtGetBoxCollisionShape(tileW, tileH, tileW);
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
            particleIndex++;
            
            //crop image for texture on particle
			ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            
            
            
            //setup particle - takes ofPoint pos, cropped ofImage, tileW and tileH globals
//			Particle p;
//            Particle::keyframe pixelPos;
//            Particle::keyframe startPos;
//            pixelPos.pos=ofPoint(x*tileW, y*tileH,0);
//            p.setup(&world, particleKeyframes[particleKeyframe], pixelPos, tileImage, tileW, tileH, particleIndex/particleCount);
//			particles.push_back(p);
		}
	}
    
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
    
    
    // For "bird cloud/leave" exit
    // This currently needs some hardcoded stuff Particle::goToPostion so leave it out for now
    //    Particle::keyframe exit;
    //    exit.type = ofPoint(PARTICLE_POS_ABS, PARTICLE_POS_ABS, PARTICLE_POS_ABS);
    //    exit.duration = 2000;
    //    exit.durationMin = 0;
    //    exit.durationMax = 5000;
    //    exit.posMin.x = -200;
    //    exit.posMax.x = -400;
    //    exit.pos.y = -100;
    //    exit.posMin.z = -200;
    //    exit.posMax.z = 200;
    //    exit.interpolation = PARTICLE_INTERPOLATE_EXPO_EASE_OUT;
    //    particleKeyframes.push_back(exit);
    
    
}

void testApp::loadCamKeyframes(){
    
    //sequentially loads camera positions, rate, and reached into vector of camPoint objects
    
    camPoint newCamPoint;
    newCamPoint.pos=ofPoint(200,400,800);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(200,400,0);
    camKeyframes.push_back(newCamPoint);
    
    newCamPoint.pos=ofPoint(1000,600,800);
    newCamPoint.rate=.5;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(0,0,0);
    camKeyframes.push_back(newCamPoint);
    
//    newCamPoint.pos=ofPoint(1200,200,1000);
//    newCamPoint.rate=1;
//    newCamPoint.reached=false;
//    newCamPoint.lookAt=ofPoint(ofGetWidth()/2,ofGetHeight()/2,0);
//    camKeyframes.push_back(newCamPoint);
    
//    newCamPoint.pos=ofPoint(pic.width/2,pic.height/2,700);
//    newCamPoint.rate=1;
//    newCamPoint.reached=false;
//    newCamPoint.lookAt=ofPoint(pic.width/2,pic.height/2,0);
//    camKeyframes.push_back(newCamPoint);
    
}

//--------------------------------------------------------------
void testApp::setupGL(){
    
    ofSetSmoothLighting(true);
    
    //set up GL lighting and material
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setSpotlight();
    light.setPosition(100, 0, 300);
    light.lookAt(ofPoint(-50,550,0));
    
    light2.setDiffuseColor(ofColor(255,255,255));
    light2.setSpecularColor(ofColor(255,255,255));
    light2.setSpotlight();
    light2.setPosition(700, 0, 200);
    light2.lookAt(ofPoint(850,600,0));
    
    material.setShininess(100);
    material.setSpecularColor(ofColor(255,255,100));
//        material.setEmissiveColor(ofColor(255,0,0,30));
    
    whiteMat.setShininess(1000);
    whiteMat.setSpecularColor(ofColor(255,255,255));
    
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
    
}

void testApp::mouseDragged(int x, int y, int button) {
	printf("Position: %f, %f, %f\n", camera.getX(), camera.getY(), camera.getZ());
	printf("Target: %f, %f, %f\n", camera.getTarget().getX(), camera.getTarget().getY(), camera.getTarget().getZ());
	printf("Roll: %f\n", camera.getRoll());
	printf("Distance: %f\n", camera.getDistance());
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	world.setGravity(ofVec3f(0, 0, 0));
    drawPhysics=false;
    bSwitchPhysics=true;
    
    
}
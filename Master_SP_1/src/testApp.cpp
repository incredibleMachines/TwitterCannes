#include "testApp.h"

#define tileW 10
#define tileH 10

#define USE_DOF true
#define CAM_MOVE true
#define CAM_MOUSE false
#define FACES false

#define CAMERA_PATH_LINE 0
#define CAMERA_PATH_CURVE 1
#define CAMERA_RATE_LINEAR 0
#define CAMERA_RATE_EASE 1
#define CAMERA_RATE_SPEED 2


//--------------------------------------------------------------
void testApp::setup(){
    
    glEnable(GL_DEPTH_TEST);
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
    ofSetSmoothLighting(true);
    
	ofBackground(255);

    //imgCount cycles through the images found in specified image directory
    imgCount=0;
    
    //camCount cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    camKeyframe=0;
    
    //populates vector of images off of specified director and loads camera positions into vector
    loadParticleKeyframes();
    loadDir();
    loadImage();
    loadCamKeyframes();
    
    //sets up GL lighting and materials
    setupGL();
	
    //load init camera position
    camPos.x=camKeyframes[0].pos.x;
    camPos.y=camKeyframes[0].pos.y;
    camPos.z=camKeyframes[0].pos.z;
	camera.setPosition(camPos);
	camera.lookAt(ofPoint(pic.width/2,pic.height/2, 0));
    
    if (USE_DOF) {
		dof.setup(ofGetWidth(), ofGetHeight());
		dof.setFocalRange(100);
        dof.setBlurAmount(1);
	}
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
    }
    
    camKeyframe++;
    particleKeyframe=0;
    
}

//--------------------------------------------------------------
void testApp::update(){
    // Set this flag to true and flip it to false if any of the particles haven't arrived
    bool allParticlesArrived = true;
	
    // Loop through and update all particles
    // Check if all have reached their target for the current keyframe
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
		if (!particles[i].targetReached) {
            allParticlesArrived = false;
		}
    }
    
    // Advance the keyframe counter and set the next position for each particle
    if(allParticlesArrived){
        particleKeyframe++;
        for(int i=0; i<particles.size(); i++){
            particles[i].goToPosition(particleKeyframes[particleKeyframe]);
        }
        
    }

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
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //GL
    
   ofEnableLighting();
    
	if (USE_DOF) {
		dof.begin();
		camera.begin(dof.getDimensions());
	} else {
		camera.begin();
	}

        light.enable();
	
    material.begin();
	ofSetColor(255);
	
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
    
   material.end();
	
	camera.end();
    ofDisableLighting();
	if (USE_DOF) {
        dof.end();
        //fbo of dof for drawing
        ofFbo temp=dof.getFbo();
        temp.draw(0,0,ofGetWidth(),ofGetHeight());

	}
	
	
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

    createParticles();
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

    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
            particleIndex++;
            
            //crop image for texture on particle
			ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            
            //setup particle - takes ofPoint pos, cropped ofImage, tileW and tileH globals
			Particle p;
            Particle::keyframe pixelPos;
            Particle::keyframe startPos;
            pixelPos.pos=ofPoint(x*tileW, y*tileH,0);
            p.setup(particleKeyframes[particleKeyframe], pixelPos, tileImage, tileW, tileH, particleIndex/particleCount);
			particles.push_back(p);
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
    newCamPoint.pos=ofPoint(pic.width/2,pic.width/2,500);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(pic.width/2,pic.width/2,0);
    camKeyframes.push_back(newCamPoint);

    newCamPoint.pos=ofPoint(pic.width,pic.height,250);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(pic.width/2,pic.height/2,0);
    camKeyframes.push_back(newCamPoint);

    newCamPoint.pos=ofPoint(pic.width/2,pic.height/2,700);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(pic.width/2,pic.height/2,0);    
    camKeyframes.push_back(newCamPoint);

}

//--------------------------------------------------------------
void testApp::setupGL(){
    //set up GL lighting and material
    
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setPointLight();
    light.setPosition(0, -pic.height/2, 500);
    
    material.setShininess(100);
    material.setSpecularColor(ofColor(255,255,100));
    //    material.setEmissiveColor(ofColor(255,0,0,30));
    
}

void testApp::mouseDragged(int x, int y, int button) {
	printf("Position: %f, %f, %f\n", camera.getX(), camera.getY(), camera.getZ());
	printf("Target: %f, %f, %f\n", camera.getTarget().getX(), camera.getTarget().getY(), camera.getTarget().getZ());
	printf("Roll: %f\n", camera.getRoll());
	printf("Distance: %f\n", camera.getDistance());
}
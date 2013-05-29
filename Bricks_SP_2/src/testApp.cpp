#include "testApp.h"

#define tileW 10
#define tileH 10

#define USE_DOF true
#define CAM_MOVE true
#define CAM_MOUSE false
#define FACES false

//--------------------------------------------------------------
void testApp::setup(){
    
    glEnable(GL_DEPTH_TEST);
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
//	ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetSmoothLighting(true);
    
	ofBackground(255);

    //imgCount cycles through the images found in specified image directory
    imgCount=0;
    
    //camCount cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    camCount=0;
    
    //populates vector of images off of specified director and loads camera positions into vector
    loadDir();
    loadImage();
    loadCams();
    
    //sets up GL lighting and materials
    setupGL();
	

    
    camPos.x=camPoints[0].pos.x;
    camPos.y=camPoints[0].pos.y;
    camPos.z=camPoints[0].pos.z;
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
    
    camCount++;
    animateCount=0;
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    int count=0;
	
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
		if (!particles[i].targetReached) {
			        count++;
		}
    }
    
    if(count<1){
        cout<<animateCount<<endl;
        if(animateCount<1){
            for(int i=0; i<particles.size(); i++){
                particles[i].setVector();
            }
            animateCount++;
            count=0;
        }
        else{
            animateCount=0;
            imgCount++;
            loadImage();
        }
    }

    if(CAM_MOVE) {

        if(camPos.distance(camPoints[camCount].pos)<2){
            camCount++;
        }
        if(camCount>camPoints.size()-1){
            camCount=0;
            
        }
        if (camCount==0){
            ofPoint move=camPoints[camCount].pos-camPoints[camPoints.size()-1].pos;
            move.normalize();
            camPos=camPos+camPoints[camCount].rate*move;
        }
        else{
            ofPoint move=camPoints[camCount].pos-camPoints[camCount-1].pos;
            move.normalize();
            camPos=camPos+camPoints[camCount].rate*move;
        }
        camera.setPosition(camPos);
        camera.lookAt(camPoints[camCount].lookAt);
        ofPoint temp=ofPoint(camPoints[camCount].lookAt-camPos);;
        dof.setFocalDistance((temp.length())/4);
		dof.setFocalRange(100);

        cout<<"cam "<<camCount<<":"<<temp.length()<<endl;
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
    pic.loadImage(images[imgCount]);
    pic.setImageType(OF_IMAGE_COLOR);
    pic.resize(pic.width/2, pic.height/2);
    pic.mirror(false, false);
    populatePixels();
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


void testApp::populatePixels(){
    particles.clear();
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
            
            //crop image for texture on particle
			ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            
            //setup particle - takes ofPoint pos, cropped ofImage, tileW and tileH globals
			Particle p;
            p.setup(ofPoint(x*tileW, y*tileH,0), tileImage, tileW, tileH);
			particles.push_back(p);
		}
	}
}

void testApp::loadCams(){
    
    //sequentially loads camera positions, rate, and reached into vector of camPoint objects
    
    camPoint newCamPoint;
    newCamPoint.pos=ofPoint(0,0,500);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(pic.width/2,pic.height/2,0);
    camPoints.push_back(newCamPoint);
    newCamPoint.pos=ofPoint(pic.width,pic.height,250);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(pic.width/2,pic.height/2,0);
    camPoints.push_back(newCamPoint);
    newCamPoint.pos=ofPoint(pic.width/2,pic.height/2,700);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(pic.width/2,pic.height/2,0);    
    camPoints.push_back(newCamPoint);

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
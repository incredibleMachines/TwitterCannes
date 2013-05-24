#include "testApp.h"

#define tileW 10
#define tileH 10

#define USE_DOF true
#define CAM_MOVE true

//--------------------------------------------------------------
void testApp::setup(){
    	glEnable(GL_DEPTH_TEST);
    
    
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
    
    bool bZ=true;
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	//ofEnableSmoothing();
	ofSetRectMode(OF_RECTMODE_CENTER);
    
        ofSetSmoothLighting(true);
    
	//ofSetCircleResolution(10);
    
    pic.loadImage("image.jpg");
    pic.setImageType(OF_IMAGE_COLOR);
	pic.resize(pic.width/2, pic.height/2);
	pic.mirror(true, true);
    
    populatePixels();
    //createParticles();
	
	ofBackground(255);
	ofSetColor(255);
	
	
	if (USE_DOF) {
		dof.setup(ofGetWidth(), ofGetHeight());
		dof.setFocalDistance(175);
		dof.setFocalRange(100);
        dof.setBlurAmount(1);
	}

//
//	camera.setDistance(1000);
//	camera.setFov(100);
	


//	camera.rotateAround(90, ofPoint(0, 0, 1), camera.getPosition());
//	camera.roll(30);
    
    camera.disableMouseInput();
    camPos.x=pic.width;
    camPos.y=0;
    camPos.z=500;
    
//	camera.setDistance(100);
	camera.setPosition(camPos.x, camPos.y, camPos.z);
	camera.lookAt(ofPoint(pic.width/2, pic.height/2, 0));
    
    bZ=true;
    bX=false;
    
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setPointLight();
    light.setPosition(0, -pic.height/2, 500);
    
    material.setShininess(100);
    material.setSpecularColor(ofColor(255,255,100));
//    material.setEmissiveColor(ofColor(255,0,0,30));
    
    bDirection=true;
    imgCount=0;
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    int count=0;
	
    for(int i=0; i<particles.size(); i++){
        particles[i].update();

        if(i==0){
//            cout<<particles[i].targetReached<<endl;
        }
		
		if (!particles[i].targetReached) {
			        count++;
		}
    }
	

        if(bDirection==true){
        if (count<100) {
		for(int i=0; i<particles.size(); i++){
			particles[i].flipTarget();
		}
            bDirection=false;
        }
        }
        else{
            if (count<400) {
            pic.loadImage(images[imgCount]);
            pic.setImageType(OF_IMAGE_COLOR);
            pic.resize(pic.width/2, pic.height/2);
            pic.mirror(true, true);
            
            populatePixels();
            bDirection=true;
            imgCount++;
            }
        }


    if(bZ==true){
        camPos.x-=.5;
        camPos.y+=.5;
    }
    else{
        camPos.x+=.5;
        camPos.y-=.5;
    }

    if(camPos.x<0){
        bZ=false;
    }
    if(camPos.z>pic.width){
        bZ=true;
    }
    
    if(CAM_MOVE){
        camera.setPosition(camPos.x, camPos.y, camPos.z);
        	camera.lookAt(ofPoint(pic.width/2, pic.height/2, 0));
    }
    
//    cout<<camPos<<endl;
}

//--------------------------------------------------------------
void testApp::draw(){
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
//	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
    
   material.end();
	
	camera.end();
    ofDisableLighting();
	if (USE_DOF) {
		dof.end();
        ofFbo temp=dof.getFbo();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotate(180);
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
        ofPopMatrix();
	}
	
	
}

void testApp::populatePixels(){
    //imagePixels = pic.getPixels();
    particles.clear();
	
	int total = pic.width/tileW * pic.height/tileH;
	
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
			
			
			ofImage tileImage;
			//tileImage.clone(pic);
			//tileImage.crop(x*tileW, y*tileH, tileW, tileH);
			tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
			
			Particle p;
            p.setup(ofPoint(x*tileW, y*tileH), tileImage, pic.width, pic.height);
			particles.push_back(p);
		}
	}
}

void testApp::createParticles(){
//    for(int i=0;i<pixels.size();i++){
//        Particle newParticle;
//        newParticle.setup(pixels[i].pos, pixels[i].r, pixels[i].g, pixels[i].b, pic.getPixels(), pic.width, pic.height);
//        particles.push_back(newParticle);
//    }
}

void testApp::mouseDragged(int x, int y, int button) {
	printf("Position: %f, %f, %f\n", camera.getX(), camera.getY(), camera.getZ());
	printf("Target: %f, %f, %f\n", camera.getTarget().getX(), camera.getTarget().getY(), camera.getTarget().getZ());
	printf("Roll: %f\n", camera.getRoll());
	printf("Distance: %f\n", camera.getDistance());
}
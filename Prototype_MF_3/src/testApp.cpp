#include "testApp.h"

#define tileW 19
#define tileH 19

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	//ofEnableSmoothing();
    
	ofSetCircleResolution(10);
    
    pic.loadImage("image.jpg");
    pic.setImageType(OF_IMAGE_COLOR);
	pic.resize(pic.width/2, pic.height/2);
    
    populatePixels();
    //createParticles();
	
	ofBackground(255);
	ofSetColor(255);
	
//	ofSetRectMode(OF_RECTMODE_CENTER);
    
}

//--------------------------------------------------------------
void testApp::update(){
	bool flipTarget = true;
	
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
		
		if (!particles[i].targetReached) {
			flipTarget = false;
		}
    }
	
	if (flipTarget) {
		for(int i=0; i<particles.size(); i++){
			particles[i].flipTarget();
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
	
}

void testApp::populatePixels(){
    //imagePixels = pic.getPixels();
	
	int counter = 0;
	int total = pic.width/tileW * pic.height/tileH;
	
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
			
			counter++;
			
			ofImage tileImage;
			//tileImage.clone(pic);
			//tileImage.crop(x*tileW, y*tileH, tileW, tileH);
			tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
			
			Particle p;
            p.setup(ofPoint(x*tileW, y*tileH), tileImage, pic.width, pic.height, counter, total);
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
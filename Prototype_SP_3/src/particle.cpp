//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

#define tileW 10
#define tileH 10

void Particle::setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight) {
    	glEnable(GL_DEPTH_TEST);
    
//    int startR=ofRandom(0,100);
//    if (startR<25){
//        start.x=ofRandom(0,ofGetWidth());
//        start.y=-10;
//        start.z=1000;
//    }
//    else if (startR<50){
//        start.x=ofRandom(0,ofGetWidth());
//        start.y=ofGetHeight()+10;
//        start.z=1000;
//    }
//    else if(startR<75){
//        start.x=-10;
//        start.y=ofRandom(0,ofGetHeight());
//        start.z=1000;
//    }
//    else{
//        start.x=ofGetWidth()+10;
//        start.y=ofRandom(0,ofGetHeight());
//        start.z=1000;
//    }
    
    start.x=pixel.x;
    start.y=pixel.y;
    start.z=ofRandom(1000,1500);
    
	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
	
	pixelPos.x = pixel.x;
	pixelPos.y = pixel.y;
    pos.x=start.x;
    pos.y=start.y;
    pos.z=start.z;
	
	targetReached = false;
    
    startvector=pixelPos-start;
	
	offset = ofRandom(30, 50);
    percentage=ofRandom(.1,.02);
    
    bDirection=true;
    
    image.mirror(false,true);
    ofEnableNormalizedTexCoords();
    face.allocate(imageW,imageH, GL_RGB,true);
    face=image.getTextureReference();

    
//    cube.loadFromOfImages(image, image, image, image, image, image);

}

void Particle::update(){
    if(targetReached==false){
//        percentage=.01;
        if(bDirection==true){
            pos=pos+(startvector*percentage*(pos.z/1000));
        }
        else{
            pos=pos+(startvector*percentage*((pos.z-target.z)/1000));
        }
    }
    
//	pos.x = target.x + sin(t/2 + offset) * 4 * t;
//	pos.y = target.y + cos(t/2 + offset * 2) * 5 * t;
//	pos.z = target.z + powf(t, 1.75) * offset / 40;

	
	// Reached destination
	// Takes a few seconds for this condition to become true after
	// image appears complete
    
    if(bDirection==true){
        if (pos.z<1) {
            targetReached = true;
            pos.x=pixelPos.x;
            pos.y=pixelPos.y;
            pos.z=0;
        }
    }
    else{
        if(pos.z>800){
            targetReached = true;
            pos.x=target.x;
            pos.y=target.y;
            pos.z=target.z;
        }
    }
	
	prevPos = pos;
}

void Particle::draw(){


	ofPushMatrix();
	ofTranslate(pos);
//    ofRotateX(offset * t / 50 * RAD_TO_DEG);
//    ofRotateY(offset * t / 40 * RAD_TO_DEG);
//    ofRotateZ(offset * t / 30 * RAD_TO_DEG);
//	image.draw(0, 0, tileW, tileH);
        face.bind();
    ofBox(0, 0, 0, tileW);
        face.unbind();
    
    
	ofPopMatrix();

}

void Particle::flipTarget() {

        target.x=pixelPos.x;
        target.y=pixelPos.y;
        target.z=ofRandom(800,1200);
        targetReached=false;
    if(bDirection==true){
        startvector=pos-target;
    }
    else{
        startvector=pixelPos-pos;
    }
    bDirection=!bDirection;
    percentage=ofRandom(.1,.02);
	
}
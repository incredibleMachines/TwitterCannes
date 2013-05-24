//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

#define tileW 20
#define tileH 20

void Particle::setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight) {
    
    
    int startR=ofRandom(0,100);
    if (startR<25){
        start.x=ofRandom(0,ofGetWidth());
        start.y=-10;
        start.z=1000;
    }
    else if (startR<50){
        start.x=ofRandom(0,ofGetWidth());
        start.y=ofGetHeight()+10;
        start.z=1000;
    }
    else if(startR<75){
        start.x=-10;
        start.y=ofRandom(0,ofGetHeight());
        start.z=1000;
    }
    else{
        start.x=ofGetWidth()+10;
        start.y=ofRandom(0,ofGetHeight());
        start.z=1000;
    }
    
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
    percentage=ofRandom(.009,.03);
    
    bDirection=true;
	
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
        if(pos.z>999){
            targetReached = true;
            pos.x=target.x;
            pos.y=target.y;
            pos.z=1000;
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
	image.draw(0, 0, tileW, tileH);
	ofPopMatrix();
	//ofCircle(pos, 2);
}

void Particle::flipTarget() {

        target.x=ofRandom(0,ofGetWidth());
        target.y=ofRandom(0,ofGetHeight());
        target.z=1000;
        targetReached=false;
    if(bDirection==true){
        startvector=pos-target;
    }
    else{
        startvector=pixelPos-pos;
    }
    bDirection=!bDirection;
    percentage=ofRandom(.005,.02);
	
}
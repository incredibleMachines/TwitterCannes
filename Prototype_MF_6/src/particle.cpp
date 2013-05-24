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

void Particle::setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight, float i, float total) {

	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
	
	pixelPos.x = pixel.x;
	pixelPos.y = pixel.y;
	
	target.x = pixelPos.x + ((ofGetWidth() - imageW) / 2);
	target.y = pixelPos.y + ((ofGetHeight() - imageH) / 2);
	target.z = 0;
	
	targetReached = false;
	
	// store which tile this is relative to others, so we can make some
	// come in later than others, for example
	myIndex = i / total;
	
	// for diversity in starting positions
	offset = ofRandom(30, 50);
    timeOffset = ofRandom(0, 1);
//    timeOffset = 0;
    animateOutOffset = 0;
	
	// Generate a random rotation type 0..2 which will determine
	// different values for axes of rotation
	rotationType = ofRandom(8);
	
}

void Particle::update(){
//	float time = MIN(ofGetElapsedTimef(), PI*9/8);
	float time = ofGetElapsedTimef();
	float t = ofMap(sin(time+PI/2-timeOffset), -1, 1, 0, 50);
//	float t = 0;
    
    if (!targetReached) {
        pos.x = target.x + sin(t/4 + offset) * 4 * t;
        pos.y = target.y + cos(t/4 + offset * 2) * 5 * t;
        pos.z = target.z + powf(t, 1.75) * offset / 40;
    }

	
	// Reached destination
	// Takes a few seconds for this condition to become true after
	// image appears complete
	
	float xDiff = abs(pos.x - target.x);
	float yDiff = abs(pos.y - target.y);
    float zDiff = abs(pos.z - target.z);
	
	if (xDiff < 0.01 && yDiff < 0.01) {
//    if (!xDiff && !yDiff && !zDiff) {
		targetReached = true;
        pos = target;
	}
	
	prevPos = pos;
}

void Particle::draw(){
//	float time = MIN(ofGetElapsedTimef(), PI*9/8);
	float time = ofGetElapsedTimef();
	float t = ofMap(sin(time+PI/2-timeOffset), -1, 1, 0, 25);
//	float t = 0;

	float rotateX, rotateY, rotateZ;
	
	switch (rotationType) {
		case 0:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG;
			rotateZ = offset * t / 30 * RAD_TO_DEG;
			break;
			
		case 1:
			rotateX = offset * t / 50 * RAD_TO_DEG * -1;
			rotateY = offset * t / 40 * RAD_TO_DEG;
			rotateZ = offset * t / 30 * RAD_TO_DEG;
			break;
			
		case 2:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG * -1;
			rotateZ = offset * t / 30 * RAD_TO_DEG;
			break;
			
		case 3:
			rotateX = offset * t / 50 * RAD_TO_DEG * -1;
			rotateY = offset * t / 40 * RAD_TO_DEG * -1;
			rotateZ = offset * t / 30 * RAD_TO_DEG;
			break;
			
		case 4:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG;
			rotateZ = offset * t / 30 * RAD_TO_DEG * -1;
			break;
			
		case 5:
			rotateX = offset * t / 50 * RAD_TO_DEG * -1;
			rotateY = offset * t / 40 * RAD_TO_DEG;
			rotateZ = offset * t / 30 * RAD_TO_DEG * -1;
			break;
			
		case 6:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG * -1;
			rotateZ = offset * t / 30 * RAD_TO_DEG * -1;
			break;
			
		case 7:
			rotateX = offset * t / 50 * RAD_TO_DEG * -1;
			rotateY = offset * t / 40 * RAD_TO_DEG * -1;
			rotateZ = offset * t / 30 * RAD_TO_DEG * -1;
			break;
	}
    
    if (targetReached) {
        rotateX = rotateY = rotateZ = 0;
    }
	
	ofPushMatrix();
	ofTranslate(pos);
    ofRotateX(rotateX);
    ofRotateY(rotateY);
    ofRotateZ(rotateZ);
	image.draw(0, 0, tileW, tileH);
	ofPopMatrix();
	//ofCircle(pos, 2);
}

void Particle::flipTarget() {
    targetReached = false;
//    animateOutOffset = ofGetElapsedTimef() + PI*3/2;
    timeOffset = 0 - ofGetElapsedTimef() + PI/2 + timeOffset;
}
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
	
	// Generate a random rotation type 0..2 which will determine
	// different values for axes of rotation
	rotationType = ofRandom(3);
	
}

void Particle::update(){
//	float time = MIN(ofGetElapsedTimef(), PI*3/2);
	float time = ofGetElapsedTimef();
	float t = ofMap(sin(time), -1, 1, 0, 50);
//	float t = 0;
	
	pos.x = target.x + sin(t/2 + offset) * 4 * t;
	pos.y = target.y + cos(t/2 + offset * 2) * 5 * t;
	pos.z = target.z + powf(t, 1.75) * offset / 40;

	
	// Reached destination
	// Takes a few seconds for this condition to become true after
	// image appears complete
	
	float xDiff = abs(pos.x - target.x);
	float yDiff = abs(pos.y - target.y);
	
	if (xDiff < 0.01 && yDiff < 0.01) {
		targetReached = true;
	}
	
	prevPos = pos;
}

void Particle::draw(){
//	float time = MIN(ofGetElapsedTimef(), PI*3/2);
	float time = ofGetElapsedTimef();
	float t = ofMap(sin(time), -1, 1, 0, 25);
//	float t = 0;

	float rotateX, rotateY, rotateZ;
	
	switch (rotationType) {
		case 0:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG;
			rotateZ = offset * t / 30 * RAD_TO_DEG;
			break;
			
		case 1:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG * -1;
			rotateZ = offset * t / 30 * RAD_TO_DEG;
			break;
			
		case 2:
			rotateX = offset * t / 50 * RAD_TO_DEG;
			rotateY = offset * t / 40 * RAD_TO_DEG;
			rotateZ = offset * t / 30 * RAD_TO_DEG * -1;
			break;
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


	
}
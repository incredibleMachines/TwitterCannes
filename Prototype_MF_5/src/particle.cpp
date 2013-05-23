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
	
	myIndex = i / total;
	offset = ofRandom(50);
	
}

void Particle::update(){
	
	float t = ofMap(sin(ofGetElapsedTimef()), -1, 1, 0, 50);
	
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
	float t = ofMap(sin(ofGetElapsedTimef()), -1, 1, 0, 25);

	ofPushMatrix();
	ofTranslate(pos);
    ofRotateX(offset * t / 50 * RAD_TO_DEG);
    ofRotateY(offset * t / 40 * RAD_TO_DEG);
    ofRotateZ(offset * t / 30 * RAD_TO_DEG);
	image.draw(0, 0, tileW, tileH);
	ofPopMatrix();
	//ofCircle(pos, 2);
}

void Particle::flipTarget() {


	
}
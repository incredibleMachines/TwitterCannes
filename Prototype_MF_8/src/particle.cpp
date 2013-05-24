//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

#define tileW 12
#define tileH 12

void Particle::setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight, float i, float
					 total) {

    myIndex = i / total;
    
    start.x = ofMap(myIndex, 0, 1, ofGetWidth(), ofGetWidth()*2);
//	start.x = ofRandom(-ofGetWidth(), ofGetWidth()*2);
//	start.x = ofMap(i, i, total, ofGetWidth()*2, ofGetWidth());

//	start.y = ofRandom(0, ofGetHeight());
//	start.y = start.x / ofGetWidth() * ofGetHeight() * -1;
	start.y = ofRandom(1) < 0.5 ? 0 : ofGetHeight();
//	start.y = 0;
	start.z = ofRandom(-200, 200);

    pos.x = start.x;
    pos.y = start.y;
	pos.z = start.z;

	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
	
	pixelPos.x = pixel.x;
	pixelPos.y = pixel.y;
	
	target.x = pixelPos.x + ((ofGetWidth() - imageW) / 2);
	target.y = pixelPos.y + ((ofGetHeight() - imageH) / 2);
	target.z = 0;

    percentage = 0;
	percentageAdder = 0.01;
	direction = false;
	
	targetReached = false;
	

	//cout << myIndex << endl;
}

void Particle::update(){
	
//	float pct = 0.02;
	float pct = ofMap(myIndex, 1, 0, 0.043, 0.08);
	
	pos.x = pos.x * (1-pct) + pct * target.x;
	pos.y = pos.y * (1-pct) + pct * target.y;
	pos.z = pos.z * (1-pct) + pct * target.z;

//	percentage += percentageAdder;
//	if (direction)
//		percentage -= percentageAdder;
//	else
//		percentage += percentageAdder;
//
//	float shapedPercent = powf(percentage, 4);
//	
//	pos.x = start.x * (1-shapedPercent) + shapedPercent * target.x;
//	pos.y = start.y * (1-shapedPercent) + shapedPercent * target.y;
//	pos.z = start.z * (1-shapedPercent) + shapedPercent * target.z;
	
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
	ofPushMatrix();
	ofTranslate(pos);
	ofRotateX((target.x - pos.x)*3);
//	ofRotateZ(atan2((prevPos-pos).y, -(prevPos-pos).x));
//	ofRotateY((target.x - pos.x)*0.1);
//	ofRotateZ((target.x - pos.x)*0.1);
    //ofRect(pos.x, pos.y, pos.z, 1, 1);
	image.draw(0, 0, tileW, tileH);
	ofPopMatrix();
	//ofCircle(pos, 2);
}

void Particle::flipTarget() {

//		target.x = ofRandom(0, ofGetWidth());
//		target.y = ofRandom(1) < 0.5 ? 0 : ofGetHeight();

	if (target.x != start.x && target.y != start.y ) {
		target.x = ofRandom(-200, -400);
		target.y = -100;
//		target.y = ofRandom(0, ofGetHeight()/8);
		target.z = start.z;
	} else {

		target.x = pixelPos.x;
		target.y = pixelPos.y;

		target.x += (ofGetWidth() - imageW) / 2;
		target.y += (ofGetHeight() - imageH) / 2;
		
		target.z = 0;
		
	}

//	ofPoint temp = start;
//	start = target;
//	target = temp;
	
	percentage = 0;
//	percentageAdder *= -1;
//	direction = !direction;
	targetReached = false;
	
}
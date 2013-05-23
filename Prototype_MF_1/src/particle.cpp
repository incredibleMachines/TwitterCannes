//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

void Particle::setup(ofVec2f pixel, int red, int green, int blue, int imageWidth, int imageHeight){

	start.x = ofRandom(0, ofGetWidth());
//	start.y = ofRandom(0, ofGetHeight());
	start.y = start.x / ofGetWidth() * ofGetHeight() * -1;
//	start.y = ofRandom(1) < 0.5 ? 0 : ofGetHeight();
//	start.y = 0;

    pos.x = start.x;
    pos.y = start.y;

	imageW = imageWidth;
	imageH = imageHeight;
	
	pixelPos.x = pixel.x;
	pixelPos.y = pixel.y;
	
	target.x = pixelPos.x;
	target.y = pixelPos.y;
	
	target.x += (ofGetWidth() - imageW) / 2;
	target.y += (ofGetHeight() - imageH) / 2;

    percentage=1;

    r=red;
    g=green;
    b=blue;
	
	targetReached = false;
}

void Particle::update(){
//	percentage-=.1*(startvector.length()/ofGetWidth());
//	pos=pixelpos-(startvector*percentage);
	
	float pct = 0.15;
	
	pos.x = pos.x * (1-pct) + pct * target.x;
	pos.y = pos.y * (1-pct) + pct * target.y;
	
	// Reached destination
	// Takes a few seconds for this condition to become true after
	// image appears complete
	
	float xDiff = abs(pos.x - target.x);
	float yDiff = abs(pos.y - target.y);
	
	if (xDiff < 0.01 && yDiff < 0.01) {
		targetReached = true;
	}
}

void Particle::draw(){
    ofSetColor(r,g,b);
    ofRect(pos.x,pos.y,1,1);
	//ofCircle(pos, 2);
}

void Particle::flipTarget() {

//		target.x = ofRandom(0, ofGetWidth());
//		target.y = ofRandom(1) < 0.5 ? 0 : ofGetHeight();

	if (target.x != start.x && target.y != start.y ) {
		target.x = start.x;
		target.y = start.y;
//		target.y = ofRandom(0, ofGetHeight()/8);
	} else {

		target.x = pixelPos.x;
		target.y = pixelPos.y;

		target.x += (ofGetWidth() - imageW) / 2;
		target.y += (ofGetHeight() - imageH) / 2;
		
	}
	
	targetReached = false;
	
}
/*
 *  Particle.h
 *  particleLetters
 *
 *  Created by Andrew Wallace on 8/31/11.
 *  Copyright 2011 Cool Town Inc. All rights reserved.
 *
 *  Modified by Matt Felsen for color images, Summer 2013
 *  INCREDIBLE MACHINES
 *
 */

#include "ofMain.h"

#pragma once

class Particle{
	public:
	
	ofColor c;
	
	ofPoint loc;	//x and y location
	ofPoint vel;	//velocity of the particle
	float ang;		//angle in radians
	float speed;
	
	bool starting;
	ofPoint start;	//where it should start
	ofPoint goal;	//where it should end
	int timer;
	
	
	unsigned char * textPix;	//array of pixels holding the outline of the letters
	int imgW;				//size of the image
	
	void setup(ofPoint startLoc, ofColor pixelColor, unsigned char * &_textPix, int _imgW, bool normalStart);
	void update();
	void draw();
	void bounce();
	bool checkBounds();	

};
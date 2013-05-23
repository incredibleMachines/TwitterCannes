/*
 *  LeavingParticle.h
 *  particleLetters
 *
 *  Created by Andrew Wallace on 8/31/11.
 *  Copyright 2011 Cool Town Inc. All rights reserved.
 *
 */


#include "ofMain.h"

#pragma once

class LeavingParticle{
public:
	
	ofPoint loc;	//x and y location
	ofPoint start;	//where it should start
	ofPoint goal;	//where it should end
	int timer;
	
	bool killMe;
	
	
	void setup(ofPoint startLoc);
	void update();
	void draw();
	
	
};
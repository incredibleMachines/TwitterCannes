/*
 *  LeavingParticle.cpp
 *  particleLetters
 *
 *  Created by Andrew Wallace on 8/31/11.
 *  Copyright 2011 Cool Town Inc. All rights reserved.
 *
 */

#include "LeavingParticle.h"

void LeavingParticle::setup(ofPoint startLoc){
	start=startLoc;
	loc=startLoc;
	
	float endY=-10;
	if (ofRandom(0,100)>50) endY=ofGetHeight()+10;
	goal.set(ofRandom(0,ofGetWidth()),endY);
	killMe=false;
	timer=0;
}
void LeavingParticle::update(){
	timer++;
	//what percentage of the way there is it?
	float prc=ofMap(timer,0,100,0,1,true);
	//set the lcoaiton based on the percentage
	loc.x=ofMap(powf(prc,3),0,1,start.x,goal.x);
	loc.y=ofMap(powf(prc,3),0,1,start.y,goal.y);
	//if time is up, go to normal mode
	if (timer==100)
		killMe=true;
	
}
void LeavingParticle::draw(){
	ofSetColor(0, 0, 0);
	ofRect(loc.x, loc.y, 1,1);
}
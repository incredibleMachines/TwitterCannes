/*
 *  Particle.cpp
 *  particleLetters
 *
 *  Created by Andrew Wallace on 8/31/11.
 *  Copyright 2011 Cool Town Inc. All rights reserved.
 *
 */

#include "Particle.h"

void Particle::setup(ofPoint _loc, unsigned char * &_textPix, int _imgW, bool normalStart){
	speed=.3;
	ang=ofRandom(0,TWO_PI);
	vel.x=speed*cos(ang);
	vel.y=speed*sin(ang);
	textPix=_textPix;
	imgW=_imgW;
	starting=normalStart;
	
	//set the starting location and velocity
	float startY=-10;
	if (ofRandom(0,100)>50)	startY=ofGetHeight()+10;
	start.set(ofRandom(0,ofGetWidth()),startY);
	goal=_loc;
	timer=0;
	
	if (starting)	loc=start;
	else			loc=_loc;
    
}

void Particle::update(){
	
	
	if (starting){
		timer++;
		//what percentage of the way there is it?
		float prc=ofMap(timer,0,100,0,1,true);
		//set the lcoaiton based on the percentage
		loc.x=ofMap(powf(prc,0.7),0,1,start.x,goal.x);
		loc.y=ofMap(powf(prc,0.7),0,1,start.y,goal.y);
		//if time is up, go to normal mode
		if (timer==100)
			starting=false;
	}
	
	else{
		loc+=vel;
		if (!checkBounds())
			bounce();
	}
	
}

void Particle::draw(){
	ofSetColor(0, 0, 0);
	ofRect(loc.x, loc.y, 1,1);
//    bird.draw(loc.x,loc.y,5,5);
}

void Particle::bounce(){
	//save the current location
	ofPoint tempLoc=loc;
	
	bool good=false;  //stays false until we find a new angle that works
	
	//loops until it works
	while (!good){
		ang=ofRandom(0,TWO_PI);  //get a new angle new angle
		//set the velocities
		vel.x=speed*cos(ang);
		vel.y=speed*sin(ang);
		
		//try moving the particle along those velocities and see if it's still out of bounds
		loc+=vel;
		if (!checkBounds()){  //is it still out of bounds
			//then set the x and y values back to what they were
			loc=tempLoc;
		}else{
			//the new direciton is good! Get out of the loop
			good=true;
		}
	}   
}

//check if the point is still within the range of black pixels
bool Particle::checkBounds(){
	if (textPix[(int)loc.y*imgW+(int)loc.x] < 10)
		return true;
	else 
		return false;
}

//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

void Particle::setup(ofVec2f pixel, int red, int green, int blue){
    int startR=ofRandom(0,100);
    if (startR<25){
        start.x=ofRandom(0,ofGetWidth());
        start.y=-10;
    }
    else if (startR<50){
        start.x=ofRandom(0,ofGetWidth());
        start.y=ofGetHeight()+10;
    }
    else if(startR<75){
        start.x=-10;
        start.y=ofRandom(0,ofGetHeight());
    }
    else{
        start.x=ofGetWidth()+10;
        start.y=ofRandom(0,ofGetHeight());
    }
    pos.x=start.x;
    pos.y=start.y;
    pixelpos.x=pixel.x;
    pixelpos.y=pixel.y;
    startvector=pixelpos-start;
    percentage=1;
    r=red;
    g=green;
    b=blue;
}

void Particle::update(){
        percentage-=.1*(startvector.length()/ofGetWidth());
        pos=pixelpos-(startvector*percentage);
}

void Particle::draw(){
    ofSetColor(r,g,b);
    ofRect(pos.x,pos.y,1,1);
}
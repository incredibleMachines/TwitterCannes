//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

#define tileW 10
#define tileH 10

void Particle::setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight) {
    	glEnable(GL_DEPTH_TEST);
    
    
    
	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
	
	pixelPos=pixel;
    
    startPos.min=pixel;
    startPos.max=pixel;
    
    startPos.min.z=600;
    startPos.max.z=800;
    
    rateMin=4;
    rateMax=20;
    
    startPos.pos=ofPoint(ofRandom(startPos.min.x,startPos.max.x),ofRandom(startPos.min.y,startPos.max.y),ofRandom(startPos.min.z,startPos.max.z));
    
    pos=startPos.pos;
    
    bStart=true;
    
    target=pixelPos;
    start=startPos.pos;
    targetReached=false;
    move=target-start;
    move.normalize();
    
    image.mirror(false,true);
    ofEnableNormalizedTexCoords();
    face.allocate(tileW,tileH, GL_RGB,true);
    face=image.getTextureReference();

    rate=ofRandom(rateMin,rateMax);
}

void Particle::update(){
    if (targetReached==false&&pos.distance(target)<rateMax) {
        targetReached = true;
        pos=target;
        bStart=!bStart;
    }
    else if(targetReached==false){
        pos=pos+rate*move;
    }
}

void Particle::draw(){
	ofPushMatrix();
	ofTranslate(pos);
    face.bind();
//    ofSphere(0, 0, 0, tileW);
    ofBox(0, 0, 0, tileW);
    face.unbind();
	ofPopMatrix();

}

void Particle::setVector() {

    if(bStart==true){
        target=pixelPos;
        start=startPos.pos;
    }
    else{
        target=startPos.pos;
        start=pixelPos;
    }
    targetReached=false;
    move=target-start;
    move.normalize();
}

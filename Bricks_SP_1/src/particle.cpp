//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

void Particle::setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight) {
    glEnable(GL_DEPTH_TEST);
    
    //USER EDITABLE -- set starting position, pixel is END position so set startPos to pixel on x,y,z to have it travel straigh along that axis, set .min and .max to adjust individual axis. RATE is pixel/frame
    
    startPos.min=pixel;
    startPos.max=pixel;
    startPos.min.z=600;
    startPos.max.z=800;
    
    rateMin=4;
    rateMax=20;

    //Load Image section for mapping
    
	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
	
    //load pixel and start position and rate to globals
	pixelPos=pixel;
    startPos.pos=ofPoint(ofRandom(startPos.min.x,startPos.max.x),ofRandom(startPos.min.y,startPos.max.y),ofRandom(startPos.min.z,startPos.max.z));
    rate=ofRandom(rateMin,rateMax);

    //set initial variables
    pos=startPos.pos;
    bStart=true;
    target=pixelPos;
    start=startPos.pos;
    targetReached=false;
    move=target-start;
    move.normalize();
    
    //load image texture
    image.mirror(false,true);
    ofEnableNormalizedTexCoords();
    face.allocate(imageW,imageH, GL_RGB,true);
    face=image.getTextureReference();
}

void Particle::update(){
    //distance check relative to target
    if (targetReached==false&&pos.distance(target)<rateMax) {
        targetReached = true;
        pos=target;
        bStart=!bStart;
    }
    //vector position change between start and target
    else if(targetReached==false){
        pos=pos+rate*move;
    }
}

void Particle::draw(){
	ofPushMatrix();
	ofTranslate(pos);
    face.bind();
//    ofSphere(0, 0, 0, imageW);
    ofBox(0, 0, 0, imageW);
    face.unbind();
	ofPopMatrix();

}

void Particle::setVector() {
    //moving in
    if(bStart==true){
        target=pixelPos;
        start=startPos.pos;
    }
    
    //moving out
    else{
        target=startPos.pos;
        start=pixelPos;
    }
    targetReached=false;
    
    //sets vector of travel
    move=target-start;
    move.normalize();
}

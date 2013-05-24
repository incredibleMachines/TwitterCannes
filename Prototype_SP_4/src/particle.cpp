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
    
//    int startR=ofRandom(0,100);
//    if (startR<25){
//        start.x=ofRandom(0,ofGetWidth());
//        start.y=-10;
//        start.z=1000;
//    }
//    else if (startR<50){
//        start.x=ofRandom(0,ofGetWidth());
//        start.y=ofGetHeight()+10;
//        start.z=1000;
//    }
//    else if(startR<75){
//        start.x=-10;
//        start.y=ofRandom(0,ofGetHeight());
//        start.z=1000;
//    }
//    else{
//        start.x=ofGetWidth()+10;
//        start.y=ofRandom(0,ofGetHeight());
//        start.z=1000;
//    }
    
    start.x=pixel.x;
    start.y=pixel.y;
    start.z=ofRandom(1000,1500);
    
	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
	
	pixelPos.x = pixel.x;
	pixelPos.y = pixel.y;
    pos.x=start.x;
    pos.y=start.y;
    pos.z=start.z;
	
	targetReached = false;
    
    startvector=pixelPos-start;
	
	offset = ofRandom(30, 50);
    percentage=ofRandom(.1,.02);
    
    bDirection=true;
    
    image.mirror(false,true);
//    ofEnableNormalizedTexCoords();
    face.allocate(imageW,imageH, GL_RGB,true);
    face=image.getTextureReference();
    

    

    
//    cube.loadFromOfImages(image, image, image, image, image, image);

}

void Particle::update(){
    if(targetReached==false){

        if(bDirection==true){
            pos=pos+(startvector*percentage*(pos.z/1000));
        }
        else{
            pos=pos+(startvector*percentage*((pos.z-target.z)/1000));
        }
    }
    

    
    if(bDirection==true){
	if (pos.z<1) {
		targetReached = true;
        pos.x=pixelPos.x;
        pos.y=pixelPos.y;
        pos.z=0;
	}
    }
    else{
        if(pos.z>target.z-1){
            targetReached = true;
            pos.x=target.x;
            pos.y=target.y;
            pos.z=target.z;
        }
    }
	
	prevPos = pos;
}

void Particle::draw(){

    face.bind();
	ofPushMatrix();
	ofTranslate(pos);


    ofBox(0, 0, 0, tileW);

    
//    vbo.draw(GL_QUADS,0,0);
	ofPopMatrix();
        face.unbind();

	//ofCircle(pos, 2);
}

void Particle::flipTarget() {

        target.x=pixelPos.x;
        target.y=pixelPos.y;
        target.z=ofRandom(500,1200);
        targetReached=false;
    if(bDirection==true){
        startvector=pos-target;
    }
    else{
        startvector=pixelPos-pos;
    }
    bDirection=!bDirection;
    percentage=ofRandom(.005,.02);
	
}
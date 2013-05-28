//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

void Particle::setup(particlePosition startPos, particlePosition pixelPos, ofImage image, float imageWidth, float imageHeight) {
    
    glEnable(GL_DEPTH_TEST);

    //Load Image section for mapping
	this->image = image;
	imageW = imageWidth;
	imageH = imageHeight;
    
    bInit=true;
	
    //load pixel and start position and rate to globals
	pixel=pixelPos;
    
    //load image texture
    image.mirror(false,true);
    ofEnableNormalizedTexCoords();
    face.allocate(imageW,imageH, GL_RGB,true);
    face=image.getTextureReference();
    goToPosition(startPos);
}

void Particle::update(){
    
    if(target.pos.distance(current.pos)<target.rate){
        targetReached=true;
    }
    
    else {
        float distance=current.pos.distance(start.pos);
        if(distance<current.startRange*current.rate){
            if(current.startRate==PARTICLE_RATE_EASE){
                if(current.rate<target.rate){
                    current.rate=target.rate*(distance/target.pos.distance(start.pos));
                }
            }
            
            else if(current.startRate==PARTICLE_RATE_SPEED){
                if(current.rate<target.rate){
                    current.rate=target.rate*(target.pos.distance(start.pos)/current.pos.distance(target.pos));
                }
            }
        }
    current.pos+=target.rate*move;
    }
}

void Particle::draw(){
	ofPushMatrix();
	ofTranslate(current.pos);
    face.bind();
    ofBox(0, 0, 0, imageW);
    face.unbind();
	ofPopMatrix();

}

void Particle::goToPosition(particlePosition goTo){
    
    if(goTo.type.x==PARTICLE_POS_PIXEL){
        goTo.pos.x=pixel.pos.x;
    }
    else if(goTo.posMin.x&&goTo.posMax.x){
        if(goTo.type.x==PARTICLE_POS_RELATIVE){
            goTo.pos.x=pixel.pos.x+ofRandom(goTo.posMin.x, goTo.posMax.x);
        }
        else{
            goTo.pos.x=ofRandom(goTo.posMin.x, goTo.posMax.x);
        }
    }
    else{
        if(goTo.type.x==PARTICLE_POS_RELATIVE){
            goTo.pos.x=pixel.pos.x+goTo.pos.x;
        }
    }
    
    if(goTo.type.y==PARTICLE_POS_PIXEL){
        goTo.pos.y=pixel.pos.y;
    }
    else if(goTo.posMin.y&&goTo.posMax.y){
        if(goTo.type.y==PARTICLE_POS_RELATIVE){
            goTo.pos.y=pixel.pos.y+ofRandom(goTo.posMin.y, goTo.posMax.y);
        }
        else{
            goTo.pos.y=ofRandom(goTo.posMin.y, goTo.posMax.y);
        }
    }
    else{
        if(goTo.type.y==PARTICLE_POS_RELATIVE){
            goTo.pos.y=pixel.pos.y+goTo.pos.y;
        }
        else{
            goTo.pos.y=goTo.pos.y;
        }
    }
    
    if(goTo.type.z==PARTICLE_POS_PIXEL){
        goTo.pos.z=pixel.pos.z;
    }
    else if(goTo.posMin.z&&goTo.posMax.z){
        if(goTo.type.z==PARTICLE_POS_RELATIVE){
            goTo.pos.z=pixel.pos.z+ofRandom(goTo.posMin.z, goTo.posMax.z);
        }
        else{
            goTo.pos.z=ofRandom(goTo.posMin.z, goTo.posMax.z);
        }
    }
    else{
        if(goTo.type.z==PARTICLE_POS_RELATIVE){
            goTo.pos.z=pixel.pos.z+goTo.pos.z;
        }
        else{
            goTo.pos.z=goTo.pos.z;
        }
    }
        
    if(goTo.rateMin!=0&&goTo.rateMax!=0){
        goTo.rate=ofRandom(goTo.rateMin,goTo.rateMax);
    }
    
    
    targetReached=false;
    
    if(bInit==true){
        current=goTo;
        bInit=false;
        targetReached=true;
    }
    
    start=current;
    target=goTo;
        
    move=target.pos-start.pos;
    move.normalize();

}

void Particle::goToPixels(){
    goToPosition(pixel);
}
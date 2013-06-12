//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

Particle::Keyframe::Keyframe(){}
Particle::Keyframe::~Keyframe(){}

void Particle::setup(Keyframe pixelPos, float index) {
    
	pixel=pixelPos;
    myIndex=index;
    targetReached=false;
    
}

void Particle::setup(Keyframe pixelPos, float index, int c, ofPoint _size) {
        
	pixel=pixelPos;
    myIndex=index;
    size=_size;
    letter=c;
    targetReached=false;
    
}

void Particle::update(){
    
    playhead = ofGetElapsedTimeMillis() - startTime;
    
    if (playhead >= target.duration) {
        targetReached=true;
        current.pos=target.pos;
        return;
    }
    
    float t = playhead;
    ofPoint b = start.pos;
    ofPoint c = target.pos - start.pos;
    float d = target.duration;
    
    if (target.path == "line") {
        
        if (target.interpolation == "linear")
            current.pos = Linear::easeNone(t, b, c, d);
        
        if (target.interpolation == "back_in")
            current.pos = Back::easeIn(t, b, c, d);
        if (target.interpolation == "back_out")
            current.pos = Back::easeOut(t, b, c, d);
        if (target.interpolation == "back_in_out")
            current.pos = Back::easeInOut(t, b, c, d);
        
        if (target.interpolation == "bounce_in")
            current.pos = Bounce::easeIn(t, b, c, d);
        if (target.interpolation == "bounce_out")
            current.pos = Bounce::easeOut(t, b, c, d);
        if (target.interpolation == "bounce_in_out")
            current.pos = Bounce::easeInOut(t, b, c, d);
        
        if (target.interpolation == "circ_in")
            current.pos = Circ::easeIn(t, b, c, d);
        if (target.interpolation == "circ_out")
            current.pos = Circ::easeOut(t, b, c, d);
        if (target.interpolation == "circ_in_out")
            current.pos = Circ::easeInOut(t, b, c, d);
        
        if (target.interpolation == "cubic_in")
            current.pos = Cubic::easeIn(t, b, c, d);
        if (target.interpolation == "cubic_out")
            current.pos = Cubic::easeInOut(t, b, c, d);
        if (target.interpolation == "cubic_in_out")
            current.pos = Cubic::easeInOut(t, b, c, d);
        
        if (target.interpolation == "elastic_in")
            current.pos = Elastic::easeIn(t, b, c, d);
        if (target.interpolation == "elastic_out")
            current.pos = Elastic::easeOut(t, b, c, d);
        if (target.interpolation == "elastic_in_out")
            current.pos = Elastic::easeInOut(t, b, c, d);
        
        if (target.interpolation == "expo_in")
            current.pos = Expo::easeIn(t, b, c, d);
        if (target.interpolation == "expo_out")
            current.pos = Expo::easeOut(t, b, c, d);
        if (target.interpolation == "expo_in_out")
            current.pos = Expo::easeInOut(t, b, c, d);
        
        if (target.interpolation == "quad_in")
            current.pos = Quad::easeIn(t, b, c, d);
        if (target.interpolation == "quad_out")
            current.pos = Quad::easeOut(t, b, c, d);
        if (target.interpolation == "quad_in_out")
            current.pos = Quad::easeInOut(t, b, c, d);
        
        if (target.interpolation == "quart_in")
            current.pos = Quart::easeIn(t, b, c, d);
        if (target.interpolation == "quart_out")
            current.pos = Quart::easeOut(t, b, c, d);
        if (target.interpolation == "quart_in_out")
            current.pos = Quart::easeInOut(t, b, c, d);
        
        if (target.interpolation == "quint_in")
            current.pos = Quint::easeIn(t, b, c, d);
        if (target.interpolation == "quint_out")
            current.pos = Quint::easeOut(t, b, c, d);
        if (target.interpolation == "quin_in_out")
            current.pos = Quint::easeInOut(t, b, c, d);
        
        if (target.interpolation == "sine_in")
            current.pos = Sine::easeIn(t, b, c, d);
        if (target.interpolation == "sine_out")
            current.pos = Sine::easeOut(t, b, c, d);
        if (target.interpolation == "sine_in_out")
            current.pos = Sine::easeInOut(t, b, c, d);
    }
    
}

void Particle::goToPosition(Keyframe goTo){
    calcPosition(goTo);
    targetReached=false;
    start=current;
    target=calc;
    startTime = ofGetElapsedTimeMillis();
    playhead = 0;
}

void Particle::calcPosition(Keyframe goTo){

    
    ofPoint min=goTo.posMin;
    ofPoint max=goTo.posMax;

    
    
    if(goTo.type.x=="pixel"){
        calc.pos.x=pixel.pos.x;
    }
    else if(goTo.type.x=="previous"){
        calc.pos.x=current.pos.x;
    }
    else if(goTo.posMin.x!=0||goTo.posMax.x!=0){
        if(goTo.type.x=="pixel_relative"){
            calc.pos.x=pixel.pos.x+ofRandom(goTo.posMin.x, goTo.posMax.x);
        }
        else if(goTo.type.x=="previous_relative"){
            calc.pos.x=current.pos.x+ofRandom(goTo.posMin.x, goTo.posMax.x);
        }
        else{
            calc.pos.x =ofRandom(min.x, max.x);
        }
    }
    else{
        if(goTo.type.x=="pixel_relative"){
            calc.pos.x=pixel.pos.x+goTo.pos.x;
        }
        else if(goTo.type.x=="previous_relative"){
            calc.pos.x=current.pos.x+goTo.pos.x;
        }
        else{
            calc.pos.x=goTo.pos.x;
        }
    }
    
    
    
    if(goTo.type.y=="pixel"){
        calc.pos.y=pixel.pos.y;
    }
    else if(goTo.type.y=="previous"){
        calc.pos.y=current.pos.y;
    }
    else if(goTo.posMin.y!=NULL&&goTo.posMax.y!=NULL){
        if(goTo.type.y=="pixel_relative"){
            calc.pos.y=pixel.pos.y+ofRandom(goTo.posMin.y, goTo.posMax.y);
        }
        else if(goTo.type.y=="previous_relative"){
            calc.pos.y=current.pos.y+ofRandom(goTo.posMin.y, goTo.posMax.y);
        }
        else{
            calc.pos.y=ofRandom(goTo.posMin.y, goTo.posMax.y);
        }
    }
    else{
        if(goTo.type.y=="pixel_relative"){
            calc.pos.y=pixel.pos.y+goTo.pos.y;
        }
        else if(goTo.type.y=="previous_relative"){
            calc.pos.y=current.pos.y+goTo.pos.y;
        }
        else{
            calc.pos.y=goTo.pos.y;
        }
    }
    
    if(goTo.type.z=="pixel"){
        calc.pos.z=pixel.pos.z;
    }
    else if(goTo.type.z=="previous"){
        calc.pos.z=current.pos.z;
    }
    else if(goTo.posMin.z!=NULL&&goTo.posMax.z!=NULL){
        if(goTo.type.z=="pixel_relative"){
            calc.pos.z=pixel.pos.z+ofRandom(goTo.posMin.z, goTo.posMax.z);
        }
        else if(goTo.type.z=="previous_relative"){
            calc.pos.z=current.pos.z+ofRandom(goTo.posMin.z, goTo.posMax.z);
        }
        else{
            calc.pos.z=ofRandom(goTo.posMin.z, goTo.posMax.z);
        }
    }
    else{
        if(goTo.type.z=="pixel_relative"){
            calc.pos.z=pixel.pos.z+goTo.pos.z;
        }
        else if(goTo.type.z=="previous_relative"){
            calc.pos.z=current.pos.z+goTo.pos.z;
        }
        else{
            calc.pos.z=goTo.pos.z;
        }
    }
    
    if(goTo.durationMin!=NULL&&goTo.durationMax!=NULL){
        calc.duration=ofRandom(goTo.durationMin,goTo.durationMax);
    }
    else{
        calc.duration=goTo.duration;
    }
    
    calc.interpolation=goTo.interpolation;
    
    calc.path=goTo.path;
}

void Particle::goToPixels(){
    goToPosition(pixel);
}
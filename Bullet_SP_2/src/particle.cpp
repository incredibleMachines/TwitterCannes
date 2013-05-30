//
//  particle.cpp
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "particle.h"

void Particle::setup(keyframe pixelPos, float index) {
    
	pixel=pixelPos;
    pixel.rotation=ofPoint(0,0,0);
    myIndex=index;
    
}

void Particle::update(){
    
    playhead = ofGetElapsedTimeMillis() - startTime;
    
    // Calculate increment for linear interpolation
    float increment=(ofGetFrameRate()/target.duration)*target.length;
    
    if (playhead >= target.duration) {
        targetReached=true;
        current.pos=target.pos;
        return;
    }

    float t = playhead;
    ofPoint b = start.pos;
    ofPoint c = target.pos - start.pos;
    float d = target.duration;
    
//    current.rotation=start.rotation;
    
    if (target.path == PARTICLE_PATH_LINE) {
        
        if (target.interpolation == PARTICLE_INTERPOLATE_LINEAR)
            current.pos = Linear::easeNone(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_BACK_EASE_IN)
            current.pos = Back::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_BACK_EASE_OUT)
            current.pos = Back::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_BACK_EASE_IN_OUT)
            current.pos = Back::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_BOUNCE_EASE_IN)
            current.pos = Bounce::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_BOUNCE_EASE_OUT)
            current.pos = Bounce::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_BOUNCE_EASE_IN_OUT)
            current.pos = Bounce::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_CIRC_EASE_IN)
            current.pos = Circ::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_CIRC_EASE_OUT)
            current.pos = Circ::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_CIRC_EASE_IN_OUT)
            current.pos = Circ::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_CUBIC_EASE_IN)
            current.pos = Cubic::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_CUBIC_EASE_OUT)
            current.pos = Cubic::easeInOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_CUBIC_EASE_IN_OUT)
            current.pos = Cubic::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_ELASTIC_EASE_IN)
            current.pos = Elastic::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_ELASTIC_EASE_OUT)
            current.pos = Elastic::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_ELASTIC_EASE_IN_OUT)
            current.pos = Elastic::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_EXPO_EASE_IN)
            current.pos = Expo::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_EXPO_EASE_OUT)
            current.pos = Expo::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_EXPO_EASE_IN_OUT)
            current.pos = Expo::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_QUAD_EASE_IN)
            current.pos = Quad::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_QUAD_EASE_OUT)
            current.pos = Quad::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_QUAD_EASE_IN_OUT)
            current.pos = Quad::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_QUART_EASE_IN)
            current.pos = Quart::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_QUART_EASE_OUT)
            current.pos = Quart::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_QUART_EASE_IN_OUT)
            current.pos = Quart::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_QUINT_EASE_IN)
            current.pos = Quint::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_QUINT_EASE_OUT)
            current.pos = Quint::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_QUINT_EASE_IN_OUT)
            current.pos = Quint::easeInOut(t, b, c, d);
        
        if (target.interpolation == PARTICLE_INTERPOLATE_SINE_EASE_IN)
            current.pos = Sine::easeIn(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_SINE_EASE_OUT)
            current.pos = Sine::easeOut(t, b, c, d);
        if (target.interpolation == PARTICLE_INTERPOLATE_SINE_EASE_IN_OUT)
            current.pos = Sine::easeInOut(t, b, c, d);
    }
    
}
//
//void Particle::draw(){
//	ofPushMatrix();
//	ofTranslate(current.pos);
//    face.bind();
//    
//    // Rotation of 3D cubes
////    ofRotateX(current.pos.distance(target.pos)*0.5*ofNoise(pixel.pos.x, pixel.pos.y));
////    ofRotateY(current.pos.distance(target.pos)*1.25*ofNoise(pixel.pos.x, pixel.pos.y));
////    ofRotateZ(current.pos.distance(target.pos)*0.75*ofNoise(pixel.pos.x, pixel.pos.y));
//    
////    ofBox(0, 0, 0, imageW);
//    
//    box->draw();
//    
//    face.unbind();
//    
////    image.draw(0, 0, 0, imageW, imageH);
//    
//	ofPopMatrix();
//
//}

void Particle::goToPosition(keyframe goTo){
//    
//    if(goTo.type.x==PARTICLE_POS_PIXEL){
//        goTo.pos.x=pixel.pos.x;
//    }
//    else if(goTo.posMin.x&&goTo.posMax.x){
//        if(goTo.type.x==PARTICLE_POS_RELATIVE){
//            goTo.pos.x=pixel.pos.x+ofRandom(goTo.posMin.x, goTo.posMax.x);
//        }
//        else{
//            goTo.pos.x=ofRandom(goTo.posMin.x, goTo.posMax.x);
//        }
//    }
//    else{
//        if(goTo.type.x==PARTICLE_POS_RELATIVE){
//            goTo.pos.x=pixel.pos.x+goTo.pos.x;
//        }
//    }
//    
//    if(goTo.type.y==PARTICLE_POS_PIXEL){
//        goTo.pos.y=pixel.pos.y;
//    }
//    else if(goTo.posMin.y&&goTo.posMax.y){
//        if(goTo.type.y==PARTICLE_POS_RELATIVE){
//            goTo.pos.y=pixel.pos.y+ofRandom(goTo.posMin.y, goTo.posMax.y);
//        }
//        else{
//            goTo.pos.y=ofRandom(goTo.posMin.y, goTo.posMax.y);
//        }
//    }
//    else{
//        if(goTo.type.y==PARTICLE_POS_RELATIVE){
//            goTo.pos.y=pixel.pos.y+goTo.pos.y;
//        }
//        else{
//            goTo.pos.y=goTo.pos.y;
//        }
//    }
//    
//    if(goTo.type.z==PARTICLE_POS_PIXEL){
//        goTo.pos.z=pixel.pos.z;
//    }
//    else if(goTo.posMin.z&&goTo.posMax.z){
//        if(goTo.type.z==PARTICLE_POS_RELATIVE){
//            goTo.pos.z=pixel.pos.z+ofRandom(goTo.posMin.z, goTo.posMax.z);
//        }
//        else{
//            goTo.pos.z=ofRandom(goTo.posMin.z, goTo.posMax.z);
//        }
//    }
//    else{
//        if(goTo.type.z==PARTICLE_POS_RELATIVE){
//            goTo.pos.z=pixel.pos.z+goTo.pos.z;
//        }
//        else{
//            goTo.pos.z=goTo.pos.z;
//        }
//    }
    
    if(goTo.durationMin!=0||goTo.durationMax!=0){
        goTo.duration+=ofRandom(goTo.durationMin,goTo.durationMax);
    }
    
    targetReached=false;
    
    start=current;
    target=goTo;
    
    startTime = ofGetElapsedTimeMillis();
    playhead = 0;

}

void Particle::goToPixels(){
    goToPosition(pixel);
}
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableSmoothing();
    
    
    pic.loadImage("images/image.png");
    pic.setImageType(OF_IMAGE_COLOR);
    
    populatePixels();
    createParticles();
    
}

//--------------------------------------------------------------
void testApp::update(){
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
        if(i==0){
            cout<<particles[i].pos<<endl;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
}

void testApp::populatePixels(){
    imagePixels=pic.getPixels();
    for (int x=0; x<pic.width; x++){
		for (int y=0; y<pic.height; y++){
            pixel newPixel;
            newPixel.r=imagePixels[y*3*pic.width+x*3];
            newPixel.g=imagePixels[y*3*pic.width+x*3+1];
            newPixel.b=imagePixels[y*3*pic.width+x*3+2];
            newPixel.pos.x=x;
            newPixel.pos.y=y;
            pixels.push_back(newPixel);
		}
	}
}

void testApp::createParticles(){
    for(int i=0;i<pixels.size();i++){
        Particle newParticle;
        newParticle.setup(pixels[i].pos, pixels[i].r, pixels[i].g, pixels[i].b);
        particles.push_back(newParticle);
    }
}
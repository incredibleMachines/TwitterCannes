#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableSmoothing();
    
    pic.loadImage("images/image.png");
    pic.setImageType(OF_IMAGE_COLOR);
    
    populatePixels();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    for(int i; i<pixels.size(); i++){
        ofSetColor(pixels[i].r, pixels[i].g, pixels[i].b);
        ofRect(pixels[i].pos.x+250, pixels[i].pos.y, 1,1);
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
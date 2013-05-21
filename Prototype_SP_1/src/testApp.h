#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
    
        int masterTimer;
        int outTimer;

        ofImage pic;
        unsigned char * imagePixels;
    
    class pixel{
        public:
            ofPoint pos;
            int r;
            int g;
            int b;
    };
    
    vector<pixel> pixels;
    
    void populatePixels();
};

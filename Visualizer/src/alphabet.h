//
//  alphabet.h
//  ofxBullet Empty Example
//
//  Created by IncredibleMachines on 6/7/13.
//
//

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Alphabet {
public:
    
    class Letter{
    public:
        int ascii;
        ofMesh mesh;
        ofPoint size;
    };
    
    void setup();
    
    vector<Letter*> letters;
    

    
    void draw(int c, ofPoint scale);
    
    ofPoint getSize(int c);
    ofMesh getMesh(int c);
    
    ofxAssimpModelLoader newModel;
    
    float maxY;

};

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
    
    Alphabet();
    
    class Letter{
    public:
        int ascii;
        ofMesh mesh;
        ofPoint size;
    };
    
    vector<Letter> letters;
    

    
    void draw(int c, ofPoint scale);
    
    ofPoint getSize(int c);
    ofMesh getMesh(int c);

};

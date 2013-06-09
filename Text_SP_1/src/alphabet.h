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
        Letter();
        ~Letter();
        int ascii;
        ofxAssimpModelLoader model;
    };
    
    vector<Letter*> letters;
    
    Alphabet();
    ~Alphabet();
    
    void draw(char c, ofPoint scale);
    
    ofPoint getSize(char c);
    ofMesh getMesh(char c);

};

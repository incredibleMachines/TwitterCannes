//
//  alphabet.cpp
//  ofxBullet Empty Example
//
//  Created by IncredibleMachines on 6/7/13.
//
//

#include "alphabet.h"

Alphabet::Alphabet(){
    for(int i=0;i<256;i++){
            Letter *newLetter=new Letter();
            newLetter->ascii=i;
        if(i>32){
            string modelPath="GothamAsciiMeshes/"+ofToString(i)+".obj";
            newLetter->model.loadModel(modelPath);
        }
            letters.push_back(newLetter);
    }
}

Alphabet::Letter::Letter(){}

Alphabet::~Alphabet(){}

void Alphabet::draw(int c, ofPoint scale){
    ofPushMatrix();
    ofScale(scale.x,scale.y,scale.z);
    letters[c]->model.getMesh(0).draw();
    ofPopMatrix();
}

ofPoint Alphabet::getSize(int c){
    if(c>32){
    vector<ofPoint> vertices=letters[c]->model.getMesh(0).getVertices();
    ofPoint max,min;
    for (int i=0;i<vertices.size();i++)
    {
        if(vertices[i].x>max.x){
            max.x=vertices[i].x;
        }
        if(vertices[i].y>max.y){
            max.y=vertices[i].y;
        }
        if(vertices[i].z>max.z){
            max.z=vertices[i].z;
        }
        if(vertices[i].x<min.x){
            min.x=vertices[i].x;
        }
        if(vertices[i].y<min.y){
            min.y=vertices[i].y;
        }
        if(vertices[i].z<min.z){
            min.z=vertices[i].z;
        }
    }
        ofPoint scale=letters[c]->model.getScale();
        ofPoint size=max-min;
        return max;
    }
    else{
        return ofPoint(0,0,0);
    }

    
//    ofPoint max=letters[c]->model.getSceneMax(true);
//    ofPoint min=letters[c]->model.getSceneMin(true);

}

ofMesh Alphabet::getMesh(int c){
    if(c>32){
    return letters[c]->model.getMesh(0);
    }
}

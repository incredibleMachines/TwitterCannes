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
        Letter newLetter;
        newLetter.ascii=i;
        if(i>32){
            string modelPath="GothamAsciiMeshes/"+ofToString(i)+".obj";
            ofxAssimpModelLoader newModel;
            newModel.loadModel(modelPath);
            newLetter.mesh=newModel.getMesh(0);
            newLetter.size=newModel.getSceneMax()-newModel.getSceneMin();
            newModel.clear();
        }
            letters.push_back(newLetter);
    }
}

void Alphabet::draw(int c, ofPoint scale){
    ofPushMatrix();
    ofScale(scale.x,scale.y,scale.z);
    letters[c].mesh.draw();
    ofPopMatrix();
}

ofPoint Alphabet::getSize(int c){
    if(c>32){
        return letters[c].size;
    }

    
//    ofPoint max=letters[c]->model.getSceneMax(true);
//    ofPoint min=letters[c]->model.getSceneMin(true);

}

ofMesh Alphabet::getMesh(int c){
    if(c>32){
    return letters[c].mesh;
    }
}

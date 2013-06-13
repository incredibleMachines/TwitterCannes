//
//  alphabet.cpp
//  ofxBullet Empty Example
//
//  Created by IncredibleMachines on 6/7/13.
//
//

#include "alphabet.h"

void Alphabet::setup(){
    maxY=0;
    int letter=0;
    cout<<ofGetFrameNum()<<endl;
    for(int i=0;i<256;i++){
        Letter* newLetter=new Letter();;
        newLetter->ascii=i;
        if(i>32){
            string modelPath="GothamAsciiMeshes/"+ofToString(i)+".obj";
            newModel.loadModel(modelPath,false);
            newLetter->mesh=newModel.getMesh(0);
            newLetter->size=newModel.getSceneMax()-newModel.getSceneMin();
            if (newLetter->size.y>maxY){
                maxY=newLetter->size.y;
                letter=i;
            }
            newModel.clear();
        }
        letters.push_back(newLetter);
    }
    cout<<maxY<<"  "<<letter<<endl;
}

void Alphabet::draw(int c, ofPoint scale){
    if(c>32&&c<256){
    ofPushMatrix();
    ofScale(scale.x,scale.y,scale.z);
    letters[c]->mesh.draw();
    ofPopMatrix();
    }
}

ofPoint Alphabet::getSize(int c){
    if(c>32&&c<256){
        return letters[c]->size;
    }

    
//    ofPoint max=letters[c]->model.getSceneMax(true);
//    ofPoint min=letters[c]->model.getSceneMin(true);

}

ofMesh Alphabet::getMesh(int c){
    if(c>32&&c<256){
    return letters[c]->mesh;
    }
}

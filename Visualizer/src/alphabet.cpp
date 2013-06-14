//
//  alphabet.cpp
//  ofxBullet Empty Example
//
//  Created by IncredibleMachines on 6/7/13.
//
//

#include "alphabet.h"

void Alphabet::setup(){
    int offsetUp[]={40, 41, 47, 59, 159, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 217, 218, 219, 220, 221, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 249, 250, 251, 252, 253};
    int offsetDown[]={43,46,61, 97, 99, 101, 109, 110, 111, 114, 115, 117, 118, 119, 120, 121, 122, 215};
    maxY=0;
    int letter=0;
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
            newLetter->offset=0.;
            for(int j=0; j<sizeof(offsetUp);j++){
                if (offsetUp[j]==i) newLetter->offset=.8;
            }
            for(int j=0; j<sizeof(offsetDown);j++){
                if (offsetDown[j]==i) newLetter->offset=-.65;
            }
            newModel.clear();
        }
        letters.push_back(newLetter);
    }
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
    if(c>32&&c<256) return letters[c]->size;
    

    
//    ofPoint max=letters[c]->model.getSceneMax(true);
//    ofPoint min=letters[c]->model.getSceneMin(true);

}

ofMesh Alphabet::getMesh(int c){
    if(c>32&&c<256) return letters[c]->mesh;
    
}

bool Alphabet::testForMesh(int c){
    if(c>32&&c<256) return true;
    else return false;
    
}

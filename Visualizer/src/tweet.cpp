//
//  tweet.cpp
//  twitterCannes
//
//  Created by IncredibleMachines on 6/10/13.
//
//

#include "tweet.h"

#define tileW 50
#define tileH 50
#define tileD 10

void Tweet::loadTweet(string _text, string _ID, string _img, string _username, string _handle, string _profileimage, ofxBulletWorldRigid* _world){
    keyframe=0;
    bNewKey=0;
    world=_world;
    
    boxShape = ofBtGetBoxCollisionShape(boxScale.x*tileW, boxScale.y*tileH, boxScale.z*tileD);
    ofxJSONElement animation;
    bool success=animation.open("keyframes/contentBlockKeyframes/test.json");
    cout<<success<<endl;
    int i=0;
    tweetIn="keyframes/particleKeyframes/in/"+animation["animations"][i]["tweet"]["in"].asString();
    tweetOut="keyframes/particleKeyframes/out/"+animation["animations"][i]["tweet"]["out"].asString();
    imgIn="keyframes/particleKeyframes/in/"+animation["animations"][i]["image"]["in"].asString();
    imgOut="keyframes/particleKeyframes/out/"+animation["animations"][i]["image"]["out"].asString();
    userIn="keyframes/particleKeyframes/in/"+animation["animations"][i]["user"]["in"].asString();
    userOut="keyframes/particleKeyframes/out/"+animation["animations"][i]["user"]["out"].asString();
    Tweet newTweet;
    newTweet.keyframe=0;
    newTweet.imageKeyframe=0;
    newTweet.userKeyframe=0;
    ofPoint pos=tweetPos;
    
    loadParticleKeyframes(tweetIn, 0);
    loadParticleKeyframes(tweetOut,0);
    
    
    ofUniString tweetText= ofTextConverter::toUTF32(newTweet.text);
    
    for (int i=0;i<tweetText.length();i++){
        
        if (tweetText[i]==32){
            pos.x+=10;
            if(pos.x>40){
                pos.y+=5;
                pos.x=-45;
            }
        }
        
        else{
            //setup position values for displaying whole image
            Particle::keyframe temp;
            temp.pos.x=pos.x;
            temp.pos.y=pos.y;
            temp.pos.z=pos.z;
            ofVec3f boxDim;
            boxDim=gotham.getSize(tweetText[i])*tweetScale;
            //create Bullet letters for image visualization
            newTweet.letters.push_back( new ofxBulletCustomShape() );
            ofQuaternion startRot=ofQuaternion(0,0,1,PI);
            newTweet.letters[newTweet.letters.size()-1]->addMesh(gotham.getMesh(tweetText[i]),tweetScale,true);
            newTweet.letters[newTweet.letters.size()-1]->create(_world->world,ofPoint(0,0,0),startRot,0.);
            newTweet.letters[newTweet.letters.size()-1]->add();
            newTweet.letters[newTweet.letters.size()-1]->setProperties(.1,.1);
            newTweet.letters[newTweet.letters.size()-1]->enableKinematic();
            
            //create particles for controlling static shape position/rotation
            Particle particle;
            particle.setup(temp,i,tweetText[i],boxDim);
            newTweet.particles.push_back(particle);
            pos.x+=boxDim.x+2;
        }
    }
    
    tweetScale=ofPoint(.1,.1,.1);
    imagePos=ofPoint(-10,-10,-55);
    boxScale=ofPoint(.05,.05,.05);
    tweetPos=ofPoint(-45,0,-60);
    gotham= *new Alphabet();
    
    //    if(_img!=" "){
    //        newTweet.image=loadImage(_img, _world);
    //    }
    //    newTweet.user=loadUser(_username, _handle, _profileimage, _world, gotham);
    //    return newTweet;
        world->destroy();
    
}

void Tweet::update(ofxBulletWorldRigid* world){
    
    //check whether individual particles have completed the duration of their transition
    int moving=0;
    for(int i=0;i<particles.size();i++){
        if(!particles[i].targetReached){
            moving++;
        }
    }
    
    //if all have reached, trigger next keyframe position
    if (moving==0){
        bNewKey=true;
        keyframe++;
        if(keyframe>tweetKeyframes.size()-1){
            keyframe=0;
        }
    }
    
    //set new particle destinations
    if(bNewKey==true){
        
        //Destroy particles action - triggers image switch and removes bullet objects/position vectors
        
        if(tweetKeyframes[keyframe].path=="destroy"){
            
        }
        
        else if(tweetKeyframes[keyframe].path!="gravity"){
            tweetToKinematic();
        }
        
        //if switching into physics state
        else{
            tweetToPhysics(*world);
        }
        bNewKey=false;
    }
    
    //if not new keyframe
    
    else{
        cout<<tweetKeyframes.size()<<endl;
        updateTweet();
    }
    cout<<keyframe<<endl;
}


void Tweet::draw(){
        for (int i=0;i<letters.size();i++){
            white.bind();
            //        letters[i]->draw();
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            glEnable(GL_NORMALIZE);
            glDisable(GL_CULL_FACE);
            btScalar	m[16];
            ofGetOpenGLMatrixFromRigidBody( letters[i]->getRigidBody(), m );
            glPushMatrix();
            glMultMatrixf( m );
            glTranslatef(-particles[i].size.x/2, -particles[i].size.y/2, -particles[i].size.z/2);
            gotham.draw(particles[i].letter,tweetScale);
            glPopMatrix();
            glPopClientAttrib();
            glPopAttrib();
            white.unbind();
    }
    
//    for (int i=0;i<image.shapes.size();i++){
//            image.face[i].bind();
//            image.shapes[i]->draw();
//            image.face[i].unbind();
//        }
    
//    for (int i=0;i<user.letters.size();i++){
//        white.bind();
//        //        letters[i]->draw();
//        glPushAttrib(GL_ALL_ATTRIB_BITS);
//        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
//        glEnable(GL_NORMALIZE);
//        glDisable(GL_CULL_FACE);
//        btScalar	m[16];
//        ofGetOpenGLMatrixFromRigidBody( user.letters[i]->getRigidBody(), m );
//        glPushMatrix();
//        glMultMatrixf( m );
//        glTranslatef(-user.particles[i].size.x/2, -user.particles[i].size.y/2, -user.particles[i].size.z/2);
////        gotham->draw(user.particles[i].letter,*userScale);
//        glPopMatrix();
//        glPopClientAttrib();
//        glPopAttrib();
//        white.unbind();
//    }
    
}

Tweet::tweetImage Tweet::loadImage(string _image, ofxBulletWorldRigid* _world){
    ofImage pic;
    Tweet::tweetImage newImage= *new tweetImage();
    newImage.location=_image;
    newImage.bNewKey=true;
    pic.loadImage(_image);
    pic.setImageType(OF_IMAGE_COLOR);
    pic.resize(pic.width/2, pic.height/2);
    pic.mirror(false, false);
    
    int i=0;

    loadParticleKeyframes(imgIn,1);
    loadParticleKeyframes(imgOut,1);
    
    
    for (int x = 0; x < pic.width / tileW; x++){
        for (int y = 0; y < pic.height / tileH; y++){
            
            //setup position values for displaying whole image
            Particle::keyframe temp;
            temp.pos.x=x*tileW*boxScale.x+imagePos.x;
            temp.pos.y=y*tileH*boxScale.y+imagePos.y;
            temp.pos.z=imagePos.z;
            
            //create Bullet shapes for image visualization
            newImage.shapes.push_back( new ofxBulletBox() );
            
            ((ofxBulletBox*)newImage.shapes[newImage.shapes.size()-1])->init(boxShape);
            ((ofxBulletBox*)newImage.shapes[newImage.shapes.size()-1])->create(_world->world,ofPoint(0,0,0),0.);
            newImage.shapes[newImage.shapes.size()-1]->setProperties(.1,1);
            newImage.shapes[newImage.shapes.size()-1]->add();
            newImage.shapes[newImage.shapes.size()-1]->enableKinematic();
            
            //crop and allocate image boxes to tiles
            ofTexture newFace;
            ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
            tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            tileImage.mirror(false,true);
            ofEnableNormalizedTexCoords();
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            newImage.face.push_back(newFace);
            
            //create particles for controlling static shape position/rotation
            
            Particle particle;
            particle.setup(temp,x+x*y);
            newImage.particles.push_back(particle);
        }
    }
    
    for(int i=0;i<newImage.particles.size();i++){
        newImage.particles[i].goToPosition(imageKeyframes[imageKeyframe]);
        newImage.particles[i].start.pos=newImage.particles[i].target.pos;
    }
    return newImage;
    
}

Tweet::tweetUser Tweet::loadUser(string _username, string _handle, string _profileimage, ofxBulletWorldRigid* _world, Alphabet* gotham){
    tweetUser newUser = *new tweetUser();
    ofPoint pos=userPos;
    
    int i=0;
 
    loadParticleKeyframes(userIn,2);
    loadParticleKeyframes(userOut,2);
    
    ofUniString nameText= ofTextConverter::toUTF32(_username);
    ofUniString handleText= ofTextConverter::toUTF32(_handle);
    
    for (int i=0;i<nameText.length();i++){
        if(nameText[i]>32){
        //setup position values for displaying whole image
        Particle::keyframe temp;
        temp.pos.x=pos.x;
        temp.pos.y=pos.y;
        temp.pos.z=pos.z;
        ofVec3f boxDim;
        boxDim=gotham->getSize(nameText[i])*userScale;
        //create Bullet letters for image visualization
        newUser.letters.push_back( new ofxBulletCustomShape() );
        ofQuaternion startRot=ofQuaternion(0,0,1,PI);
        newUser.letters[newUser.letters.size()-1]->addMesh(gotham->getMesh(nameText[i]),userScale,true);
        newUser.letters[newUser.letters.size()-1]->create(_world->world,userKeyframes[userKeyframe].pos,startRot,10.);
        newUser.letters[newUser.letters.size()-1]->add();
        newUser.letters[newUser.letters.size()-1]->setProperties(.1,.1);
        newUser.letters[newUser.letters.size()-1]->enableKinematic();
        
        //create particles for controlling static shape position/rotation
        Particle particle;
        particle.setup(temp,i,nameText[i],boxDim);
        newUser.particles.push_back(particle);
        pos.x+=boxDim.x+2;
        }
    }
    for (int i=0;i<handleText.length();i++){

        if(handleText[i]>32){
        //setup position values for displaying whole image
        Particle::keyframe temp;
        temp.pos.x=pos.x;
        temp.pos.y=pos.y;
        temp.pos.z=pos.z;
        ofVec3f boxDim;
        boxDim=gotham->getSize(handleText[i])*handleScale;
        //create Bullet letters for image visualization
        newUser.letters.push_back( new ofxBulletCustomShape() );
        ofQuaternion startRot=ofQuaternion(0,0,1,PI);
        newUser.letters[newUser.letters.size()-1]->addMesh(gotham->getMesh(handleText[i]),handleScale,true);
        newUser.letters[newUser.letters.size()-1]->create(_world->world,userKeyframes[userKeyframe].pos,startRot,10.);
        newUser.letters[newUser.letters.size()-1]->add();
        newUser.letters[newUser.letters.size()-1]->setProperties(.1,.1);
        newUser.letters[newUser.letters.size()-1]->enableKinematic();
        
        //create particles for controlling static shape position/rotation
        Particle particle;
        particle.setup(temp,i,handleText[i],boxDim);
        newUser.particles.push_back(particle);
        pos.x+=boxDim.x+2;
        }
    }
    return newUser;
    
}

void Tweet::loadParticleKeyframes(string filePath, int which){
    ofxJSONElement json;
   bool success=json.open(filePath);
        for(int i = 0; i < json["particles"].size(); i++){
            Particle::keyframe temp;
            temp.path=json["particles"][i]["path"].asString();
            
            cout<<temp.path<<endl;
            temp.type.x=json["particles"][i]["type"]["x"].asString();
            temp.type.y=json["particles"][i]["type"]["y"].asString();
            temp.type.z=json["particles"][i]["type"]["z"].asString();
            string stringNum;
            stringNum=json["particles"][i]["pos"]["set"]["x"].asString();
            if(stringNum!=""){
                temp.pos.x=ofToInt(stringNum);
            }
            stringNum=json["particles"][i]["pos"]["set"]["y"].asString();
            if(stringNum!=""){
                temp.pos.y=ofToInt(stringNum);
            }
            stringNum=json["particles"][i]["pos"]["set"]["z"].asString();
            if(stringNum!=""){
                temp.pos.z=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["pos"]["min"]["x"].asString();
            if(stringNum!=""){
                temp.posMin.x=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["pos"]["min"]["y"].asString();
            if(stringNum!=""){
                temp.posMin.y=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["pos"]["min"]["z"].asString();
            if(stringNum!=""){
                temp.posMin.z=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["pos"]["max"]["x"].asString();
            if(stringNum!=""){
                temp.posMax.x=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["pos"]["max"]["y"].asString();
            if(stringNum!=""){
                temp.posMax.y=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["pos"]["max"]["z"].asString();
            if(stringNum!=""){
                temp.posMax.z=ofToInt(stringNum);
            }
            stringNum= json["particles"][i]["duration"]["actual"].asString();
            if(stringNum!=""){
                temp.duration=ofToInt(stringNum);
            }
            stringNum= json["particles"][i]["duration"]["max"].asString();
            if(stringNum!=""){
                temp.durationMax=ofToInt(stringNum);
            }
            stringNum= json["particles"][i]["duration"]["min"].asString();
            if(stringNum!=""){
                temp.durationMin=ofToInt(stringNum);
            }
            for(int j=0;j<json["particles"][i]["interpolations"].size();j++){
                     temp.interpolations.push_back(json["particles"][i]["interpolations"][j]["interpolation"].asString());
            }
            
            stringNum = json["particles"][i]["gravity"]["x"].asString();
            if(stringNum!=""){
                temp.gravity.x=ofToInt(stringNum);
            }
            stringNum = json["particles"][i]["gravity"]["y"].asString();
            if(stringNum!=""){
                temp.gravity.y=ofToInt(stringNum);
            }
            stringNum= json["particles"][i]["gravity"]["z"].asString();
            if(stringNum!=""){
                temp.gravity.z=ofToInt(stringNum);
            }
            
            if(which==0){
                tweetKeyframes.push_back(temp);
            }
            else if(which==1){
                imageKeyframes.push_back(temp);
            }
            else if(which==2){
                userKeyframes.push_back(temp);
            }
        }
    
    }

    
    


void Tweet::tweetToKinematic(){
    for (int i=0;i<letters.size();i++){
        particles[i].update();
        
        //set vector position/rotation to bullet object position/rotation and start rotation transition
        btTransform trans;
        ofPoint temp=letters[i]->getPosition();
        if(keyframe!=0){
            if(tweetKeyframes[keyframe-1].path=="gravity"){
                particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(keyframe!=0){
            if(tweetKeyframes[keyframe-1].path=="destroy"){
                particles[i].bRotate=true;
                particles[i].current.quat=rotQuat;
            }
        }
        letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        letters[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        particles[i].goToPosition(tweetKeyframes[keyframe]);
        letters[i]->enableKinematic();
    }
}

void Tweet::imgToKinematic(){
    for (int i=0;i<image.shapes.size();i++){
        image.particles[i].update();
        btTransform trans;
        ofPoint temp;
        if(imageKeyframe!=0){
            if(imageKeyframes[keyframe-1].path=="gravity"){
                particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = image.shapes[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(imageKeyframe!=0){
            if(imageKeyframes[keyframe-1].path=="gravity"){
                particles[i].bRotate=true;
                particles[i].current.quat=rotQuat;
            }
        }
        image.shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        image.shapes[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        image.particles[i].goToPosition(imageKeyframes[imageKeyframe]);
        image.shapes[i]->enableKinematic();
    }
}

void Tweet::userToKinematic(){
    for (int i=0;i<user.letters.size();i++){
        user.particles[i].update();
        
        //set vector position/rotation to bullet object position/rotation and start rotation transition
        btTransform trans;
        ofPoint temp=user.letters[i]->getPosition();
        if(keyframe!=0){
            if(userKeyframes[userKeyframe-1].path=="gravity"){
                user.particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = user.letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(userKeyframe!=0){
            if(userKeyframes[userKeyframe-1].path=="destroy"){
                user.particles[i].bRotate=true;
                user.particles[i].current.quat=rotQuat;
            }
        }
        user.letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        user.letters[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        user.particles[i].goToPosition(tweetKeyframes[keyframe]);
        user.letters[i]->enableKinematic();
    }
}

void Tweet::tweetToPhysics(ofxBulletWorldRigid world){
    world.setGravity(ofVec3f(tweetKeyframes[keyframe].gravity.x,tweetKeyframes[keyframe].gravity.y,tweetKeyframes[keyframe].gravity.z));
    for (int i=0;i<letters.size();i++){
        btTransform trans;
        ofPoint temp=letters[i]->getPosition();
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        letters[i]->remove();
        letters[i]->addMesh(gotham.getMesh(particles[i].letter),tweetScale,false);
        letters[i]->create(world.world,trans, 1.);
        letters[i]->add();
        letters[i]->setProperties(.1,1);
        particles[i].goToPosition(tweetKeyframes[keyframe]);
    }
}

void Tweet::imgToPhysics(){
    world->setGravity(ofVec3f(imageKeyframes[imageKeyframe].gravity.x,imageKeyframes[imageKeyframe].gravity.y,imageKeyframes[imageKeyframe].gravity.z));
    for (int i=0;i<image.shapes.size();i++){
        btTransform trans;
        ofPoint temp=image.shapes[i]->getPosition();
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = image.shapes[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        image.shapes[i]->remove();
        image.shapes[i]->init(boxShape);
        image.shapes[i]->create(world->world,trans, 10);
        image.shapes[i]->setProperties(.1,1);
        image.shapes[i]->add();
        image.particles[i].goToPosition(imageKeyframes[imageKeyframe]);
    }
    
}

void Tweet::userToPhysics(){
    world->setGravity(ofVec3f(userKeyframes[userKeyframe].gravity.x,userKeyframes[userKeyframe].gravity.y,userKeyframes[userKeyframe].gravity.z));
    for (int i=0;i<user.letters.size();i++){
        btTransform trans;
        ofPoint temp=user.letters[i]->getPosition();
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = user.letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        user.letters[i]->remove();
        user.letters[i]->addMesh(gotham.getMesh(particles[i].letter),tweetScale,false);
        user.letters[i]->create(world->world,trans, 1.);
        user.letters[i]->add();
        user.letters[i]->setProperties(.1,1);
        user.particles[i].goToPosition(userKeyframes[userKeyframe]);
    }
}

void Tweet::updateTweet(){
    cout<<tweetKeyframes.size()<<endl;
    if(tweetKeyframes[keyframe].path!="gravity"){
        for (int i=0;i<letters.size();i++){
            particles[i].update();
            btTransform trans;
            ofPoint temp=particles[i].current.pos;
            trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
            ofQuaternion rotQuat = letters[i]->getRotationQuat();
            float newRot = rotQuat.w();
            if(particles[i].bRotate==true){
                float newRot = rotQuat.w();
                newRot -= .05f;
                if (newRot<0){
                    newRot=0;
                    particles[i].bRotate=false;
                }
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
            }
            else{
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), 0) );
            }
            letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
            letters[i]->activate();
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        for (int i=0;i<letters.size();i++){
            particles[i].update();
        }
    }
}

void Tweet::updateImg(){
    if(imageKeyframes[imageKeyframe].path!="gravity"){
        for (int i=0;i<image.shapes.size();i++){
            image.particles[i].update();
            btTransform trans;
            ofPoint temp=particles[i].current.pos;
            trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
            ofQuaternion rotQuat = image.shapes[i]->getRotationQuat();
            float newRot = rotQuat.w();
            if(particles[i].bRotate==true){
                float newRot = rotQuat.w();
                newRot -= .05f;
                if (newRot<0){
                    newRot=0;
                    particles[i].bRotate=false;
                }
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
            }
            else{
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), 0) );
            }
            image.shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
            image.shapes[i]->activate();
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        for (int i=0;i<image.shapes.size();i++){
            image.particles[i].update();
        }
    }
    
}


void Tweet::updateUser(){
    if(userKeyframes[userKeyframe].path!="gravity"){
        for (int i=0;i<user.letters.size();i++){
            user.particles[i].update();
            btTransform trans;
            ofPoint temp=user.particles[i].current.pos;
            trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
            ofQuaternion rotQuat = letters[i]->getRotationQuat();
            float newRot = rotQuat.w();
            if(user.particles[i].bRotate==true){
                float newRot = rotQuat.w();
                newRot -= .05f;
                if (newRot<0){
                    newRot=0;
                    user.particles[i].bRotate=false;
                }
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
            }
            else{
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), 0) );
            }
            user.letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
            user.letters[i]->activate();
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        for (int i=0;i<letters.size();i++){
            user.particles[i].update();
        }
    }
}




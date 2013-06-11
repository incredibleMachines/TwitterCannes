//
//  tweet.cpp
//  twitterCannes
//
//  Created by IncredibleMachines on 6/10/13.
//
//

#include "tweet.h"

#define tileW 40
#define tileH 40
#define tileD 10

void Tweet::loadTweet(string _text, string _ID, string _img, string _username, string _handle, string _profileimage, ofxBulletWorldRigid* _world){
    
    tweetPos=ofPoint(-45,20,-55);
    boxScale=ofPoint(.1,.1,.1);
    imagePos=ofPoint(-10,-10,-60);
        tweetScale=ofPoint(.1,.1,.1);
    userPos=ofPoint(-80,-20,-25);
    userScale=ofPoint(.1,.1,.1);
    handleScale=ofPoint(.1,.08,.1);

    gotham= *new Alphabet();
    
    keyframe=0;
    bNewKey=true;
    world=_world;
    
    boxShape = ofBtGetBoxCollisionShape(boxScale.x*tileW, boxScale.y*tileH, boxScale.z*tileD);
    ofxJSONElement animation;
    bool success=animation.open("keyframes/contentBlockKeyframes/test.json");
    int i=0;
    string speed, delay;
    
    tweetIn.animation="keyframes/particleKeyframes/in/"+animation["animations"][i]["tweet"]["in"]["animation"].asString();
    
    speed="keyframes/particleKeyframes/in/"+animation["animations"][i]["tweet"]["in"]["speed"].asString();
    if(speed!=""){
        tweetIn.speed=ofToInt(speed);
    }
    else{
        tweetIn.speed=1;
    }
    delay="keyframes/particleKeyframes/in/"+animation["animations"][i]["tweet"]["in"]["delay"].asString();
    if(delay!=""){
        tweetIn.delay=ofToInt(delay);
    }
    else{
        tweetIn.delay=1;
    }
    
    tweetOut.animation="keyframes/particleKeyframes/out/"+animation["animations"][i]["tweet"]["out"]["animation"].asString();
    
    speed="keyframes/particleKeyframes/in/"+animation["animations"][i]["tweet"]["out"]["speed"].asString();
    if(speed!=""){
        tweetOut.speed=ofToInt(speed);
    }
    else{
        tweetOut.speed=1;
    }
    delay="keyframes/particleKeyframes/in/"+animation["animations"][i]["tweet"]["out"]["delay"].asString();
    if(delay!=""){
        tweetOut.delay=ofToInt(delay);
    }
    else{
        tweetOut.delay=1;
    }
    
    imgIn.animation="keyframes/particleKeyframes/in/"+animation["animations"][i]["image"]["in"]["animation"].asString();
    speed="keyframes/particleKeyframes/in/"+animation["animations"][i]["image"]["in"]["speed"].asString();
    if(speed!=""){
        imgIn.speed=ofToInt(speed);
    }
    else{
        imgIn.speed=1;
    }
    delay="keyframes/particleKeyframes/in/"+animation["animations"][i]["image"]["in"]["delay"].asString();
    if(delay!=""){
        imgIn.delay=ofToInt(delay);
    }
    else{
        imgIn.delay=1;
    }
    
    imgOut.animation="keyframes/particleKeyframes/out/"+animation["animations"][i]["image"]["out"]["animation"].asString();
    speed="keyframes/particleKeyframes/in/"+animation["animations"][i]["image"]["out"]["speed"].asString();
    if(speed!=""){
        imgOut.speed=ofToInt(speed);
    }
    else{
        imgOut.speed=1;
    }
    delay="keyframes/particleKeyframes/in/"+animation["animations"][i]["image"]["out"]["delay"].asString();
    if(delay!=""){
        imgOut.delay=ofToInt(delay);
    }
    else{
        imgOut.delay=1;
    }
    
    userIn.animation="keyframes/particleKeyframes/in/"+animation["animations"][i]["user"]["in"]["animation"].asString();
    speed="keyframes/particleKeyframes/in/"+animation["animations"][i]["user"]["in"]["speed"].asString();
    if(speed!=""){
        userIn.speed=ofToInt(speed);
    }
    else{
        userIn.speed=1;
    }
    delay="keyframes/particleKeyframes/in/"+animation["animations"][i]["user"]["in"]["delay"].asString();
    if(delay!=""){
        userIn.delay=ofToInt(delay);
    }
    else{
        userIn.delay=1;
    }
    
    userOut.animation="keyframes/particleKeyframes/out/"+animation["animations"][i]["user"]["out"]["animation"].asString();
    speed="keyframes/particleKeyframes/in/"+animation["animations"][i]["user"]["out"]["speed"].asString();
    if(speed!=""){
        userOut.speed=ofToInt(speed);
    }
    else{
        userOut.speed=1;
    }
    delay="keyframes/particleKeyframes/in/"+animation["animations"][i]["user"]["out"]["delay"].asString();
    if(delay!=""){
        userOut.delay=ofToInt(delay);
    }
    else{
        userOut.delay=1;
    }
    
    ofPoint pos=tweetPos;
    
    loadParticleKeyframes(tweetIn, 0);
    loadParticleKeyframes(tweetOut,0);
    
    
    ofUniString tweetText= ofTextConverter::toUTF32(_text);
    
    for (int i=0;i<tweetText.length();i++){
        
        if (tweetText[i]==32){
            pos.x+=10;
            if(pos.x>40){
                pos.y-=5;
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
            letters.push_back( new ofxBulletCustomShape() );
            ofQuaternion startRot=ofQuaternion(0,0,1,PI);
            letters[letters.size()-1]->addMesh(gotham.getMesh(tweetText[i]),tweetScale,true);
            letters[letters.size()-1]->create(world->world,tweetKeyframes[keyframe].pos,startRot,0.);
            letters[letters.size()-1]->add();
            letters[letters.size()-1]->setProperties(.1,.1);
            letters[letters.size()-1]->enableKinematic();
            
            //create particles for controlling static shape position/rotation
            Particle particle;
            particle.setup(temp,i,tweetText[i],gotham.getSize(tweetText[i]));
            particles.push_back(particle);
            pos.x+=boxDim.x+2;
            particle.goToPosition(tweetKeyframes[keyframe]);
        }
    }
    
        if(_img!=" "){
            loadImage(_img);
            cout<<"load"<<endl;
        }
    loadUser(_username, _handle, _profileimage);
    
}

void Tweet::update(){
    
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
    
    moving=0;
    
    for(int i=0;i<image.particles.size();i++){
        if(!image.particles[i].targetReached){
            moving++;
        }
    }
    
    //if all have reached, trigger next keyframe position
    if (moving==0){
        image.bNewKey=true;
        imageKeyframe++;
        if(imageKeyframe>imageKeyframes.size()-1){
            imageKeyframe=0;
        }
    }
    
    moving=0;
    
    for(int i=0;i<user.particles.size();i++){
        if(!user.particles[i].targetReached){
            moving++;
        }
    }
    
    //if all have reached, trigger next keyframe position
    if (moving==0){
        user.bNewKey=true;
        userKeyframe++;
        if(userKeyframe>userKeyframes.size()-1){
            userKeyframe=0;
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
            tweetToPhysics();
        }
        
        bNewKey=false;
    }
    
    //if not new keyframe
    
    else{
        updateTweet();
    }
    
    if (image.bNewKey==true){
        if(imageKeyframes[imageKeyframe].path=="destroy"){
            
        }
        
        else if(imageKeyframes[imageKeyframe].path!="gravity"){
            imgToKinematic();
        }
        
        //if switching into physics state
        else{
            imgToPhysics();
        }
        
        image.bNewKey=false;
    }
    
    else{
        updateImg();
    }
    
    if (user.bNewKey==true){
        if(userKeyframes[userKeyframe].path=="destroy"){
            
        }
        
        else if(userKeyframes[userKeyframe].path!="gravity"){
            userToKinematic();
        }
        
        //if switching into physics state
        else{
            userToPhysics();
        }
        
        user.bNewKey=false;
    }
    
    else{
        updateUser();
    }
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
            
            cout<<particles[i].size.x<<endl;
            glTranslatef((-particles[i].size.x)/2*tweetScale.x, (-particles[i].size.y)/2*tweetScale.y, (-particles[i].size.z)/2*tweetScale.z);
            gotham.draw(particles[i].letter,tweetScale);
            glPopMatrix();
            glPopAttrib();
            white.unbind();
    }
    
    for (int i=0;i<image.shapes.size();i++){
            image.face[i].bind();
            image.shapes[i]->draw();
            image.face[i].unbind();
        }
    
    for (int i=0;i<user.letters.size();i++){
        white.bind();
        //        letters[i]->draw();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        glDisable(GL_CULL_FACE);
        btScalar	m[16];
        ofGetOpenGLMatrixFromRigidBody(user.letters[i]->getRigidBody(), m );
        glPushMatrix();
        glMultMatrixf( m );
        glTranslatef(-user.particles[i].size.x/2, -user.particles[i].size.y/2, -user.particles[i].size.z/2);
        gotham.draw(user.particles[i].letter,userScale);
        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();
        white.unbind();
    }
    
}

void Tweet::loadImage(string _image){
    ofImage pic;
    image.location=_image;
    image.bNewKey=true;
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
            image.shapes.push_back( new ofxBulletBox() );
            
            ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->init(boxShape);
            ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->create(world->world,ofPoint(0,0,0),0.);
            image.shapes[image.shapes.size()-1]->setProperties(.1,1);
            image.shapes[image.shapes.size()-1]->add();
            image.shapes[image.shapes.size()-1]->enableKinematic();
            
            //crop and allocate image boxes to tiles
            ofTexture newFace;
            ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
            tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            tileImage.mirror(false,true);
            ofEnableNormalizedTexCoords();
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            image.face.push_back(newFace);
            
            //create particles for controlling static shape position/rotation
            
            Particle particle;
            particle.setup(temp,x+x*y);
            image.particles.push_back(particle);
        }
    }
    
    for(int i=0;i<image.particles.size();i++){
        image.particles[i].goToPosition(imageKeyframes[imageKeyframe]);
        image.particles[i].start.pos=image.particles[i].target.pos;
    }
    
}

void Tweet::loadUser(string _username, string _handle, string _profileimage){
    ofPoint pos=userPos;
    
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
        boxDim=gotham.getSize(nameText[i])*userScale;
        //create Bullet letters for image visualization
        user.letters.push_back( new ofxBulletCustomShape() );
        ofQuaternion startRot=ofQuaternion(0,0,1,PI);
        user.letters[user.letters.size()-1]->addMesh(gotham.getMesh(nameText[i]),userScale,true);
        user.letters[user.letters.size()-1]->create(world->world,userKeyframes[userKeyframe].pos,startRot,10.);
        user.letters[user.letters.size()-1]->add();
        user.letters[user.letters.size()-1]->setProperties(.1,.1);
        user.letters[user.letters.size()-1]->enableKinematic();
        
        //create particles for controlling static shape position/rotation
        Particle particle;
        particle.setup(temp,i,nameText[i],boxDim);
        user.particles.push_back(particle);
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
        boxDim=gotham.getSize(handleText[i])*handleScale;
        //create Bullet letters for image visualization
        user.letters.push_back( new ofxBulletCustomShape() );
        ofQuaternion startRot=ofQuaternion(0,0,1,PI);
        user.letters[user.letters.size()-1]->addMesh(gotham.getMesh(handleText[i]),handleScale,true);
        user.letters[user.letters.size()-1]->create(world->world,userKeyframes[userKeyframe].pos,startRot,10.);
        user.letters[user.letters.size()-1]->add();
        user.letters[user.letters.size()-1]->setProperties(.1,.1);
        user.letters[user.letters.size()-1]->enableKinematic();
        
        //create particles for controlling static shape position/rotation
        Particle particle;
        particle.setup(temp,i,handleText[i],boxDim);
        user.particles.push_back(particle);
        pos.x+=boxDim.x+2;
        }
    }
}

void Tweet::loadParticleKeyframes(animation anim, int which){
    ofxJSONElement json;
   bool success=json.open(anim.animation);
        for(int i = 0; i < json["particles"].size(); i++){
            Particle::keyframe temp;
            temp.path=json["particles"][i]["path"].asString();
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
            if(tweetKeyframes[keyframe-1].path=="gravity"){
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
        ofPoint temp=image.shapes[i]->getPosition();
        if(imageKeyframe!=0){
            if(imageKeyframes[imageKeyframe-1].path=="gravity"){
                image.particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = image.shapes[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(imageKeyframe!=0){
            if(imageKeyframes[imageKeyframe-1].path=="gravity"){
                image.particles[i].bRotate=true;
                image.particles[i].current.quat=rotQuat;
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
        if(userKeyframe!=0){
            if(userKeyframes[userKeyframe-1].path=="gravity"){
                user.particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = user.letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(userKeyframe!=0){
            if(userKeyframes[userKeyframe-1].path=="gravity"){
                user.particles[i].bRotate=true;
                user.particles[i].current.quat=rotQuat;
            }
        }
        user.letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        user.letters[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        user.particles[i].goToPosition(userKeyframes[userKeyframe]);
        user.letters[i]->enableKinematic();
    }
}

void Tweet::tweetToPhysics(){
    world->setGravity(ofVec3f(tweetKeyframes[keyframe].gravity.x,tweetKeyframes[keyframe].gravity.y,tweetKeyframes[keyframe].gravity.z));
    for (int i=0;i<letters.size();i++){
        btTransform trans;
        ofPoint temp=letters[i]->getPosition();
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        letters[i]->remove();
        letters[i]->addMesh(gotham.getMesh(particles[i].letter),tweetScale,false);
        letters[i]->create(world->world,trans, 1.);
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
    if(tweetKeyframes[keyframe].path!="gravity"){
        for (int i=0;i<letters.size();i++){
            particles[i].update();
            btTransform trans;
            ofPoint temp=particles[i].current.pos;
//            if(i==0){
//                cout<<particles[i].current.pos;
//            }
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
            ofPoint temp=image.particles[i].current.pos;
            trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
            ofQuaternion rotQuat = image.shapes[i]->getRotationQuat();
            float newRot = rotQuat.w();
            if(image.particles[i].bRotate==true){
                float newRot = rotQuat.w();
                newRot -= .05f;
                if (newRot<0){
                    newRot=0;
                    image.particles[i].bRotate=false;
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
            ofQuaternion rotQuat = user.letters[i]->getRotationQuat();
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
        for (int i=0;i<user.letters.size();i++){
            user.particles[i].update();
        }
    }
}




//
//  tweet.cpp
//  twitterCannes
//
//  Created by IncredibleMachines on 6/10/13.
//
//

#include "tweet.h"

#define tileW 20
#define tileH 20
#define tileD 20

#define KEYFRAME_TWEET 0
#define KEYFRAME_IMAGE 1
#define KEYFRAME_USER 2


void Tweet::setup(ofPoint _hashMin, ofPoint _hashMax, ofxBulletWorldRigid *_world, Alphabet *_gotham){
    hashMin=_hashMin;
    hashMax=_hashMax;
    animationCount=0;
    
    world=_world;
    gotham=_gotham;
    
    masterDelay=4000;
    
}

void Tweet::loadTweet(db item){
    
    bMulti=false;
    
    tweetKeyframe=0;
    imageKeyframe=0;
    userKeyframe=0;
    
    tweetScale=ofPoint(.08,.08,.08);
    userScale=ofPoint(.1,.1,.1);
    handleScale=ofPoint(.07,.07,.07);
    imageScale=ofPoint(.15,.15,.02);
    profileScale=ofPoint(2,2,.1);
    
    tweetPos=ofPoint(hashMin.x,43,hashMax.z);
    imagePos=ofPoint(-10,-10,2);
    userPos=ofPoint(hashMin.x,50,hashMax.z);
    //    userPos=ofPoint(hashMin.x,-30,hashMax.z);
    
    boxShape = ofBtGetBoxCollisionShape(imageScale.x*tileW, imageScale.y*tileH, imageScale.z*tileD);
    profileBox = ofBtGetBoxCollisionShape(profileScale.x*10, profileScale.y*10, profileScale.z*10);
    
    
    
    
    bImage=false;
    
    bFinished=false;
    image.bFinished=false;
    user.bFinished=false;
    bNewKey=true;
    user.bNewKey=true;
    image.bNewKey=true;
    
    
    ofxJSONElement json;
    
    if(item.media_url!=""){
        bool success=pic.loadImage(item.media_url);
        if(success){
            bImage=true;
            cout<<item.media_url<<endl;
        }
        else{
            image.bFinished=true;
        }
    }
    
    else{
        image.bFinished=true;
    }
    
    bool success=json.open("keyframes/contentBlockKeyframes/good.json");
    int i=animationCount;
    
    string speed;
    string delay;
    
    ofxJSONElement animations=json["animations"];
    
    tweetIn.path="keyframes/particleKeyframes/in/"+json["animations"][i]["tweet"]["in"]["animation"].asString();
    
    speed=json["animations"][i]["tweet"]["in"]["speed"].asString();
    if(speed!="") tweetIn.speed=ofToInt(speed);
    else tweetIn.speed=1;
    
    
    delay=json["animations"][i]["tweet"]["in"]["delay"].asString();
    if(delay!="") tweetIn.delay=ofToInt(delay);
    else tweetIn.delay=0;
    
    
    tweetOut.path="keyframes/particleKeyframes/out/"+json["animations"][i]["tweet"]["out"]["animation"].asString();
    
    speed=json["animations"][i]["tweet"]["out"]["speed"].asString();
    if(speed!="") tweetOut.speed=ofToInt(speed);
    else tweetOut.speed=1;
    
    delay=json["animations"][i]["tweet"]["out"]["delay"].asString();
    if(delay!="") tweetOut.delay=ofToInt(delay)+masterDelay;
    else tweetOut.delay=masterDelay;
    
    if(bImage==true){
        imgIn.path="keyframes/particleKeyframes/in/"+json["animations"][i]["image"]["in"]["animation"].asString();
        speed=json["animations"][i]["image"]["in"]["speed"].asString();
        if(speed!="") imgIn.speed=ofToInt(speed);
        else imgIn.speed=1;
        
        delay=json["animations"][i]["image"]["in"]["delay"].asString();
        if(delay!="") imgIn.delay=ofToInt(delay);
        else imgIn.delay=0;
        
        
        imgOut.path="keyframes/particleKeyframes/out/"+json["animations"][i]["image"]["out"]["animation"].asString();
        speed=json["animations"][i]["image"]["out"]["speed"].asString();
        if(speed!="") imgOut.speed=ofToInt(speed);
        else imgOut.speed=1;
        
        delay=json["animations"][i]["image"]["out"]["delay"].asString();
        if(delay!="") imgOut.delay=ofToInt(delay)+masterDelay;
        else imgOut.delay=masterDelay;
    }
    
    
    userIn.path="keyframes/particleKeyframes/in/"+json["animations"][i]["user"]["in"]["animation"].asString();
    speed=json["animations"][i]["user"]["in"]["speed"].asString();
    if(speed!="") userIn.speed=ofToInt(speed);
    else userIn.speed=1;
    
    delay=json["animations"][i]["user"]["in"]["delay"].asString();
    if(delay!="") userIn.delay=ofToInt(delay);
    else userIn.delay=0;
    
    
    userOut.path="keyframes/particleKeyframes/out/"+json["animations"][i]["user"]["out"]["animation"].asString();
    speed=json["animations"][i]["user"]["out"]["speed"].asString();
    if(speed!="") userOut.speed=ofToInt(speed);
    else userOut.speed=1;
    
    delay=json["animations"][i]["user"]["out"]["delay"].asString();
    if(delay!="") userOut.delay=ofToInt(delay)+masterDelay;
    else userOut.delay=masterDelay;
    
    animationCount++;
    
    if (animationCount>animations.size()-1){
        animationCount=0;
    }
    
    loadParticleKeyframes(tweetIn, KEYFRAME_TWEET, true);
    loadParticleKeyframes(tweetOut,KEYFRAME_TWEET, false);
    
    if(bImage==true){
        loadParticleKeyframes(imgIn, KEYFRAME_IMAGE, true);
        loadParticleKeyframes(imgOut,KEYFRAME_IMAGE, false);
    }
    
    loadParticleKeyframes(userIn, KEYFRAME_USER, true);
    loadParticleKeyframes(userOut,KEYFRAME_USER, false);
    
    ofUniString tweetText= ofTextConverter::toUTF32(item.text);
    ofPoint pos=tweetPos;
    pos.x=tweetPos.x+(10*profileScale.x)/2+2;
    
    if(bImage==true){
        tweetPos=ofPoint(hashMin.x,-25,5);
        pos=tweetPos;
        pos.x=tweetPos.x+(10*profileScale.x)/2+1;
        //        tweetScale=ofPoint(tweetScale.x,tweetScale.y,tweetScale.z*.5);
    }
    
//    tweetBlock=new ofxBulletBox();
//    tweetBlock->init(300.,300.,240.);
//    tweetBlock->create(world->world,ofVec3f(-50,-50,-50), 0.);
//    tweetBlock->add();
    
    for (int i=0;i<tweetText.length();i++){
        
        if (tweetText[i]==32){
            pos.x+=3;
            if(pos.x>40){
                pos.y-=6;
                pos.x=tweetPos.x+(10*profileScale.x)/2+2;
            }
        }
        else if(tweetText[i]>32&&tweetText[i]<256){
            //setup position values for displaying whole image
            Particle::Keyframe temp;
            
            ofVec3f boxDim;
            boxDim=gotham->getSize(tweetText[i])*tweetScale;
            
            pos.x+=.5*boxDim.x+.5;
            
            temp.pos.x=pos.x;
            temp.pos.y=pos.y+gotham->letters[tweetText[i]]->offset;
            temp.pos.z=pos.z;
            
            //create Bullet letters for image visualization
            //create particles for controlling static shape position/rotation
            
            Particle particle;
            particle.setup(temp,i,tweetText[i],boxDim, hashMin, hashMax);
            particle.goToPosition(tweetKeyframes[tweetKeyframe]);
            particle.start.pos=particle.target.pos;
            particles.push_back(particle);
            bNewKey=false;
            
            if(tweetType!="gravity"){
                
                letters.push_back( new ofxBulletCustomShape() );
                letters[letters.size()-1]->addMesh(gotham->getMesh(tweetText[i]),tweetScale,true);
                letters[letters.size()-1]->create(world->world,ofPoint(0,0,1000),0.);
                letters[letters.size()-1]->add();
                letters[letters.size()-1]->setProperties(.1,.1);
                letters[letters.size()-1]->enableKinematic();
            }
            
            else{
                world->setGravity(ofVec3f(tweetKeyframes[tweetKeyframe].gravity.x,tweetKeyframes[tweetKeyframe].gravity.y,tweetKeyframes[tweetKeyframe].gravity.z));
                letters.push_back( new ofxBulletCustomShape() );
                letters[letters.size()-1]->addMesh(gotham->getMesh(tweetText[i]),tweetScale,true);
                letters[letters.size()-1]->create(world->world,particle.target.pos,10.);
                letters[letters.size()-1]->add();
                letters[letters.size()-1]->setProperties(.1,.1);
            }
            
            
            pos.x+=.5*boxDim.x+.5;
            
        }
    }

    
    tweetTimer=ofGetElapsedTimeMillis();
    tweetStart=false;

    
    imgTimer=ofGetElapsedTimeMillis();
    imgStart=false;

    
    userTimer=ofGetElapsedTimeMillis();
    userStart=false;

    
    
    
    if(bImage==true){
        loadImage(item.media_url);
    }
    loadUser(item.user_name, item.user_screen_name, item.user_image);
}

    
void Tweet::goTweets(){
//    tweetBlock->remove();
    if(tweetType!="gravity"){
        for(int i=0; i<particles.size();i++){
            particles[i].goToPosition(tweetKeyframes[tweetKeyframe]);
            particles[i].start.pos=particles[i].target.pos;
        }
    }
}

void Tweet::goUser(){
//    tweetBlock->remove();
    if(tweetType!="gravity"){
        for(int i=0; i<user.particles.size();i++){
            user.particles[i].goToPosition(tweetKeyframes[tweetKeyframe]);
            user.particles[i].start.pos=user.particles[i].target.pos;
        }
    }
}

void Tweet::goImg(){
    if(tweetType!="gravity"){
        for(int i=0; i<image.particles.size();i++){
            image.particles[i].goToPosition(tweetKeyframes[tweetKeyframe]);
            image.particles[i].start.pos=image.particles[i].target.pos;
        }
    }
}


void Tweet::update(){
    
    //set new particle destinations
    
    if(tweetStart==false){
        if(ofGetElapsedTimeMillis()-tweetTimer>tweetIn.delay){
            tweetStart=true;
            goTweets();
        }
    }
    
    else{
    if(bNewKey==true){
        if(bFinished==false){
            switchKey(tweetKeyframes[tweetKeyframe],KEYFRAME_TWEET);
        }
        bNewKey=false;
    }
    
    //if not new keyframe
    
    else{
        if(bFinished==false){
            updateTweet();
        }
    }
    }
    
    if(bImage==true){
        
        if(imgStart==false){
            if(ofGetElapsedTimeMillis()-imgTimer>imgIn.delay){
                imgStart=true;
                goImg();
            }
        }
        
        else{
        if(image.bNewKey==true){
            if(image.bFinished==false){
                switchKey(imageKeyframes[imageKeyframe],KEYFRAME_IMAGE);
            }
            image.bNewKey=false;
        }
        
        else{
            if(image.bFinished==false){
                updateImg();
            }
        }
        
    }
    }
    
    if(userStart==false){
        if(ofGetElapsedTimeMillis()-userTimer>userIn.delay){
            userStart=true;
            goUser();
        }
    }
    
    else{
    if (user.bNewKey==true){
        if(user.bFinished==false){
            switchKey(userKeyframes[userKeyframe],KEYFRAME_USER);
        }
        user.bNewKey=false;
    }
    
    else{
        if(user.bFinished==false){
            updateUser();
        }
    }
    }
    
}


void Tweet::draw(){
    
    drawLetters();
    drawImg();
    
}

void Tweet::drawImg(){
    user.profile.bind();
    user.shape->draw();
    user.profile.unbind();
    
    if(bImage==true){
        //        ofRotate(180,0,1,0);
        bool bBlack=false;
        for (int i=0;i<image.shapes.size();i++){
//            if(i<.40*(image.height*image.width)){
//                bBlack=true;
//            }
//            else if(i>.35*(image.height*image.width)&&i%image.width==0) bBlack=false;
//            if(bBlack==true){
//                ofSetColor(150,150,150);
//            }
            image.face[i].bind();
            image.shapes[i]->draw();
            image.face[i].unbind();
            
            ofSetColor(255,255,255);
        }
    }
    
}

void Tweet::drawLetters(){
    if(tweetStart==true){
    for (int i=0;i<letters.size();i++){
        //        letters[i]->draw();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        glDisable(GL_CULL_FACE);
        btScalar	m[16];
        ofGetOpenGLMatrixFromRigidBody( letters[i]->getRigidBody(), m );
        glPushMatrix();
        glMultMatrixf( m );
        glTranslatef((-particles[i].size.x)/2, (-particles[i].size.y)/2, (-particles[i].size.z)/2);
        gotham->draw(particles[i].letter,tweetScale);
        glPopMatrix();
        glPopAttrib();
    }
    }
    
    for (int i=1;i<user.letters.size();i++){
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
        if(user.particles[i].handle==true){
            gotham->draw(user.particles[i].letter,handleScale);
        }
        else{
            gotham->draw(user.particles[i].letter,userScale);
        }
        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();
    }
    
    
}

void Tweet::loadImage(string _image){
    image.bNewKey=true;
    
    pic.setImageType(OF_IMAGE_COLOR);
    pic.mirror(true, false);
    
    imageScale=ofPoint(140./pic.height,140./pic.height,.02);
    imagePos=ofPoint(-(pic.width*imageScale.x)/2+5,-(pic.height*imageScale.y)/2,0);
    
    boxShape = ofBtGetBoxCollisionShape(imageScale.x*tileW, imageScale.y*tileH, imageScale.z*tileD);
    
    image.width=pic.width/tileW;
    image.height=pic.height/tileH;
    
    for (int y = 0; y < pic.height / tileH; y++){
        for (int x = 0; x < pic.width / tileW; x++){
            
            
            //setup position values for displaying whole image
            Particle::Keyframe temp;
            temp.pos.x=x*tileW*imageScale.x+imagePos.x;
            temp.pos.y=y*tileH*imageScale.y+imagePos.y;
            temp.pos.z=imagePos.z;
            
            Particle particle;
            particle.setup(temp,x+x*y, hashMin, hashMax);
            particle.goToPosition(imageKeyframes[imageKeyframe]);
            particle.start.pos=particle.target.pos;
            image.particles.push_back(particle);
            
            //create Bullet shapes for image visualization
            image.shapes.push_back( new ofxBulletBox() );
            
            if(imageType!="gravity"){
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->init(boxShape);
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->create(world->world,ofPoint(0,0,1000),0.);
                image.shapes[image.shapes.size()-1]->add();
                image.shapes[image.shapes.size()-1]->setProperties(.1,0);
                image.shapes[image.shapes.size()-1]->enableKinematic();
            }
            
            else{
                world->setGravity(ofVec3f(imageKeyframes[imageKeyframe].gravity.x,imageKeyframes[imageKeyframe].gravity.y,imageKeyframes[imageKeyframe].gravity.z));
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->init(boxShape);
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->create(world->world,particle.target.pos,10.);
                image.shapes[image.shapes.size()-1]->add();
                image.shapes[image.shapes.size()-1]->setProperties(.1,1);
            }
            
            //            //crop and allocate image boxes to tiles
            
            ofEnableNormalizedTexCoords();
            ofTexture newFace;
            ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
            tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            tileImage.mirror(false,true);
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            image.face.push_back(newFace);
            
            //create particles for controlling static shape position/rotation
            
        }
    }
    
}

void Tweet::loadUser(string _username, string _handle, string _profileimage){
    ofPoint pos=userPos;
    
    if(bImage==true){
        pos=ofPoint(hashMin.x,-18,5);
        //        userScale=ofPoint(userScale.x,userScale.y,userScale.z*.5);
        //        handleScale=ofPoint(handleScale.x,handleScale.y,handleScale.z*.5);
        
    }
    
    _handle="@"+_handle;
    
    ofUniString nameText= ofTextConverter::toUTF32(_username);
    ofUniString handleText= ofTextConverter::toUTF32(_handle);
    
    cout<<"go"<<endl;
    ofEnableNormalizedTexCoords();
    Particle::Keyframe temp;
    ofImage profileImg;
    pic.setImageType(OF_IMAGE_COLOR);
    profileImg.loadImage(_profileimage);
    profileImg.mirror(true,true);
    user.profile.allocate(10*profileScale.x ,10*profileScale.y, GL_RGB,true);
    user.profile=profileImg.getTextureReference();
    
    temp.pos.x=pos.x;
    temp.pos.z=pos.z;
    temp.pos.y=pos.y-10*profileScale.y/2+3;
    
    Particle particle;
    particle.setup(temp,0, hashMin, hashMax);
    particle.goToPosition(userKeyframes[userKeyframe]);
    particle.start.pos=particle.target.pos;
    user.particles.push_back(particle);
    
    
    //create Bullet shapes for image visualization
    
    if(userType!="gravity"){
        user.shape=new ofxBulletBox();
        user.shape->init(profileBox);
        user.shape->create(world->world,particle.target.pos,0.);
        user.shape->add();
        user.shape->setProperties(.1,1);
        user.shape->enableKinematic();
    }
    
    else{
        if(tweetType!="gravity"){
        world->setGravity(ofVec3f(userKeyframes[userKeyframe].gravity.x,imageKeyframes[imageKeyframe].gravity.y,imageKeyframes[imageKeyframe].gravity.z));
        }
        user.shape=new ofxBulletBox();
        user.shape->init(profileBox);
        user.shape->create(world->world,particle.target.pos,6.);
        user.shape->add();
        user.shape->setProperties(.1,1);
    }
    pos.x+=(10*profileScale.x)/2+2;
    
    for (int i=0;i<nameText.length();i++){
        
        if(nameText[i]>32&&nameText[i]<256){
            //setup position values for displaying whole image
            Particle::Keyframe temp;
            ofVec3f boxDim;
            boxDim=gotham->getSize(nameText[i])*userScale;
            
            pos.x+=.5*boxDim.x+.5;
            
            temp.pos.x=pos.x;
            temp.pos.y=pos.y+gotham->letters[nameText[i]]->offset;
            temp.pos.z=pos.z;
            Particle particle;
            particle.setup(temp,i,nameText[i],boxDim, hashMin, hashMax);
            particle.goToPosition(userKeyframes[userKeyframe]);
            particle.start.pos=particle.target.pos;
            user.particles.push_back(particle);
            
            user.bNewKey=false;
            
            if(userType!="gravity"){
                user.letters.push_back( new ofxBulletCustomShape() );
                if(i==0){
                    user.letters.push_back( new ofxBulletCustomShape() );
                }
                user.letters[user.letters.size()-1]->addMesh(gotham->getMesh(nameText[i]),userScale,true);
                user.letters[user.letters.size()-1]->create(world->world,ofPoint(0,0,1000),0.);
                user.letters[user.letters.size()-1]->add();
                user.letters[user.letters.size()-1]->setProperties(.1,0);
                user.letters[user.letters.size()-1]->enableKinematic();
            }
            
            else{
                if(tweetType!="gravity"){
                    world->setGravity(ofVec3f(userKeyframes[userKeyframe].gravity.x,imageKeyframes[imageKeyframe].gravity.y,imageKeyframes[imageKeyframe].gravity.z));
                }
                user.letters.push_back( new ofxBulletCustomShape() );
                if(i==0){
                    user.letters.push_back( new ofxBulletCustomShape() );
                }
                user.letters[user.letters.size()-1]->addMesh(gotham->getMesh(nameText[i]),userScale,true);
                user.letters[user.letters.size()-1]->create(world->world,particle.target.pos,10.);
                user.letters[user.letters.size()-1]->add();
                user.letters[user.letters.size()-1]->setProperties(.1,1);
            }
            
            pos.x+=.5*boxDim.x+.5;
        }
    }
    
    for (int i=0;i<handleText.length();i++){
        if(handleText[i]>32&&handleText[i]<256){
            //setup position values for displaying whole image
            Particle::Keyframe temp;
            ofVec3f boxDim;
            boxDim=gotham->getSize(handleText[i])*handleScale;
            pos.x+=.5*boxDim.x+.5;
            
            temp.pos.x=pos.x;
            temp.pos.y=pos.y+gotham->letters[handleText[i]]->offset;;
            temp.pos.z=pos.z;
            
            
            Particle particle;
            particle.setup(temp,i,handleText[i],boxDim, hashMin, hashMax);
            particle.goToPosition(userKeyframes[userKeyframe]);
            particle.start.pos=particle.target.pos;
            particle.handle=true;
            user.particles.push_back(particle);
            user.bNewKey=false;
            
            if(userType!="gravity"){
                
                user.letters.push_back( new ofxBulletCustomShape() );
                user.letters[user.letters.size()-1]->addMesh(gotham->getMesh(handleText[i]),handleScale,true);
                user.letters[user.letters.size()-1]->create(world->world,ofPoint(0,0,1000),0.);
                user.letters[user.letters.size()-1]->add();
                user.letters[user.letters.size()-1]->setProperties(.1,1);
                user.letters[user.letters.size()-1]->enableKinematic();
            }
            
            else{
                world->setGravity(ofVec3f(tweetKeyframes[tweetKeyframe].gravity.x,tweetKeyframes[tweetKeyframe].gravity.y,tweetKeyframes[tweetKeyframe].gravity.z));
                user.letters.push_back( new ofxBulletCustomShape() );
                user.letters[user.letters.size()-1]->addMesh(gotham->getMesh(handleText[i]),handleScale,true);
                user.letters[user.letters.size()-1]->create(world->world,particle.target.pos,10.);
                user.letters[user.letters.size()-1]->add();
                user.letters[user.letters.size()-1]->setProperties(.1,1);
            }
            
            pos.x+=.5*boxDim.x+.5;
        }
    }
}

void Tweet::loadParticleKeyframes(Animation anim, int which, bool in){
    ofxJSONElement json;
    bool success=json.open(anim.path);
    for(int i = 0; i < json["particles"].size(); i++){
        Particle::Keyframe temp;
        
        temp.path=json["particles"][i]["path"].asString();
        
        temp.type.x=json["particles"][i]["type"]["x"].asString();
        temp.type.y=json["particles"][i]["type"]["y"].asString();
        temp.type.z=json["particles"][i]["type"]["z"].asString();
        string stringNum;
        
        
        stringNum=json["particles"][i]["pos"]["set"]["x"].asString();
        if(stringNum!="") temp.pos.x=ofToInt(stringNum);
        else temp.pos.x=NULL;
        
        stringNum=json["particles"][i]["pos"]["set"]["y"].asString();
        if(stringNum!="") temp.pos.y=ofToInt(stringNum);
        else temp.pos.y=NULL;
        
        stringNum=json["particles"][i]["pos"]["set"]["z"].asString();
        if(stringNum!="") temp.pos.z=ofToInt(stringNum);
        else temp.pos.z=NULL;
        
        
        stringNum = json["particles"][i]["pos"]["min"]["x"].asString();
        if(stringNum!="") temp.posMin.x=ofToInt(stringNum);
        else temp.posMin.x=NULL;
        stringNum = json["particles"][i]["pos"]["min"]["y"].asString();
        if(stringNum!="") temp.posMin.y=ofToInt(stringNum);
        else temp.posMin.y=NULL;
        stringNum = json["particles"][i]["pos"]["min"]["z"].asString();
        if(stringNum!="") temp.posMin.z=ofToInt(stringNum);
        else temp.posMin.z=NULL;
        
        
        
        stringNum = json["particles"][i]["pos"]["max"]["x"].asString();
        if(stringNum!=""){
            temp.posMax.x=ofToInt(stringNum);
        }
        else temp.posMax.x=NULL;
        stringNum = json["particles"][i]["pos"]["max"]["y"].asString();
        if(stringNum!=""){
            temp.posMax.y=ofToInt(stringNum);
        }
        else temp.posMax.y=NULL;
        stringNum = json["particles"][i]["pos"]["max"]["z"].asString();
        if(stringNum!=""){
            temp.posMax.z=ofToInt(stringNum);
        }
        else temp.posMax.z=NULL;
        
        
        
        stringNum= json["particles"][i]["duration"]["actual"].asString();
        if(stringNum!=""){
            temp.duration=anim.speed*ofToInt(stringNum);
            if(i==0){
                temp.duration+=anim.delay;
            }
        }
        else temp.duration=NULL;
        stringNum= json["particles"][i]["duration"]["max"].asString();
        if(stringNum!=""){
            temp.durationMax=anim.speed*ofToInt(stringNum);
            if(i==0){
                temp.duration+=anim.delay;
            }
        }
        else temp.durationMax=NULL;
        
        
        stringNum= json["particles"][i]["duration"]["min"].asString();
        
        if(stringNum!=""){
            temp.durationMin=anim.speed*ofToInt(stringNum);
            if(i==0){
                temp.durationMin+=anim.delay;
            }
        }
        else temp.durationMin=NULL;
        
        temp.interpolation=json["particles"][i]["interpolation"].asString();
        
        stringNum = json["particles"][i]["gravity"]["x"].asString();
        if(stringNum!=""){
            temp.gravity.x=ofToInt(stringNum);
        }
        else temp.gravity.x=NULL;
        
        stringNum = json["particles"][i]["gravity"]["y"].asString();
        if(stringNum!=""){
            temp.gravity.y=ofToInt(stringNum);
        }
        else temp.gravity.y=NULL;
        
        stringNum= json["particles"][i]["gravity"]["z"].asString();
        if(stringNum!=""){
            temp.gravity.z=ofToInt(stringNum);
        }
        else temp.gravity.z=NULL;
        
        if(which==0){
            tweetKeyframes.push_back(temp);
            if(i==0){
                tweetType=tweetKeyframes[0].path;
                
            }
        }
        
        else if(which==1){
            imageKeyframes.push_back(temp);
            if(i==0){
                imageType=imageKeyframes[0].path;
                
            }
        }
        
        else if(which==2){
            userKeyframes.push_back(temp);
            if(i==0){
                userType=userKeyframes[0].path;
                
            }
        }
    }
    
}

void Tweet::tweetToKinematic(){
    for (int i=0;i<letters.size();i++){
        particles[i].update();
        //set vector position/rotation to bullet object position/rotation and start rotation transition
        btTransform trans;
        ofPoint temp=letters[i]->getPosition();
        
        if(tweetKeyframe!=0){
            if(tweetKeyframes[tweetKeyframe-1].path=="gravity"){
                particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat = letters[i]->getRotationQuat();
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(tweetKeyframe!=0){
            if(tweetKeyframes[tweetKeyframe-1].path=="gravity"){
                particles[i].bRotate=true;
                particles[i].current.quat=rotQuat;
            }
        }
        letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
        letters[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
        particles[i].goToPosition(tweetKeyframes[tweetKeyframe]);
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
        ofPoint temp;

        if(i==0&&bMulti==false){
            temp=user.shape->getPosition();
        }
        else{
            temp=user.letters[i]->getPosition();
            
        }
        if(userKeyframe!=0){
            if(userKeyframes[userKeyframe-1].path=="gravity"){
                user.particles[i].current.pos=temp;
            }
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat;
        if(i==0&&bMulti==false){
            rotQuat= user.shape->getRotationQuat();
        }
        else{
            rotQuat= user.letters[i]->getRotationQuat();
        }
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(userKeyframe!=0){
            if(userKeyframes[userKeyframe-1].path=="gravity"){
                user.particles[i].bRotate=true;
                user.particles[i].current.quat=rotQuat;
            }
        }
        if(i==0&&bMulti==false){
            user.shape->getRigidBody()->getMotionState()->setWorldTransform( trans );
            user.shape->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
            user.particles[i].goToPosition(userKeyframes[userKeyframe]);
            user.shape->enableKinematic();
        }
        else{
            user.letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
            user.letters[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
            user.particles[i].goToPosition(userKeyframes[userKeyframe]);
            user.letters[i]->enableKinematic();
        }
    }
}

void Tweet::tweetToPhysics(){
    world->setGravity(ofVec3f(tweetKeyframes[tweetKeyframe].gravity.x,tweetKeyframes[tweetKeyframe].gravity.y,tweetKeyframes[tweetKeyframe].gravity.z));
    if(tweetKeyframe!=0){
        if(tweetKeyframes[tweetKeyframe-1].path!="gravity"){
            for (int i=0;i<letters.size();i++){
                btTransform trans;
                ofPoint temp=letters[i]->getPosition();
                trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                ofQuaternion rotQuat = letters[i]->getRotationQuat();
                float newRot = rotQuat.w();
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                letters[i]->remove();
                letters[i]->addMesh(gotham->letters[particles[i].letter]->mesh,tweetScale,false);
                letters[i]->create(world->world,trans, 10.);
                letters[i]->add();
                letters[i]->setProperties(.1,1);
                particles[i].goToPosition(tweetKeyframes[tweetKeyframe]);
            }
        }
    }
}

void Tweet::imgToPhysics(){
    world->setGravity(ofVec3f(imageKeyframes[imageKeyframe].gravity.x,imageKeyframes[imageKeyframe].gravity.y,imageKeyframes[imageKeyframe].gravity.z));
    if(tweetKeyframe!=0){
        if(tweetKeyframes[tweetKeyframe-1].path!="gravity"){
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
                image.shapes[i]->add();
                image.shapes[i]->setProperties(.1,1);
                image.particles[i].goToPosition(imageKeyframes[imageKeyframe]);
            }
        }
    }
    
}

void Tweet::userToPhysics(){
    world->setGravity(ofVec3f(userKeyframes[userKeyframe].gravity.x,userKeyframes[userKeyframe].gravity.y,userKeyframes[userKeyframe].gravity.z));
    for (int i=0;i<user.letters.size();i++){
        btTransform trans;
        ofPoint temp;
        if(i==0&&bMulti==false){
            temp=user.shape->getPosition();
        }
        else{
            temp=user.letters[i]->getPosition();
        }
        trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
        ofQuaternion rotQuat;
        
        if(i==0&&bMulti==false){
            rotQuat=user.shape->getRotationQuat();
        }
        else{
            rotQuat= user.letters[i]->getRotationQuat();
        }
        float newRot = rotQuat.w();
        trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
        if(i==0&&bMulti==false){
            user.shape->remove();
            user.shape->init(profileBox);
            user.shape->create(world->world,trans, 1.);
            user.shape->add();
            user.shape->setProperties(.1,1);
            user.particles[i].goToPosition(userKeyframes[userKeyframe]);
        }
        else{
            user.letters[i]->remove();
            user.letters[i]->addMesh(gotham->getMesh(user.particles[i].letter),tweetScale,false);
            user.letters[i]->create(world->world,trans, 1.);
            user.letters[i]->add();
            user.letters[i]->setProperties(.1,1);
            user.particles[i].goToPosition(userKeyframes[userKeyframe]);
        }
    }
}

void Tweet::updateTweet(){
    
    if(tweetKeyframes[tweetKeyframe].path!="gravity"){
        int moving=0;
        for (int i=0;i<letters.size();i++){
            particles[i].update();
            if(!particles[i].targetReached){
                moving++;
            }
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
        if (moving==0&&bFinished==false){
            bNewKey=true;
            tweetKeyframe++;
            if(tweetKeyframe>tweetKeyframes.size()-1){
                bFinished=true;
            }
        }

    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        int moving=0;
        for (int i=0;i<letters.size();i++){
            particles[i].update();
            if(!particles[i].targetReached){
                moving++;
            }
        }
        if (moving==0&&bFinished==false){
            bNewKey=true;
            tweetKeyframe++;
            if(tweetKeyframe>tweetKeyframes.size()-1){
                bFinished=true;
            }
        }
    }
}

void Tweet::updateImg(){
    if(imageKeyframes[imageKeyframe].path!="gravity"){
        int moving=0;
        for (int i=0;i<image.shapes.size();i++){
            image.particles[i].update();
            if(!image.particles[i].targetReached){
                moving++;
            }
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
        
        if (moving==0&&image.bFinished==false){
            image.bNewKey=true;
            imageKeyframe++;
            if(imageKeyframe>imageKeyframes.size()-1){
                image.bFinished=true;
            }
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        int moving=0;
        for (int i=0;i<image.shapes.size();i++){
            image.particles[i].update();
            if(!image.particles[i].targetReached){
                moving++;
            }
        }
        if (moving==0&&image.bFinished==false){
            image.bNewKey=true;
            imageKeyframe++;
            if(imageKeyframe>imageKeyframes.size()-1){
                image.bFinished=true;
            }
        }
        
    }
    
}


void Tweet::updateUser(){
    if(userKeyframes[userKeyframe].path!="gravity"){
        int moving=0;
        for (int i=0;i<user.particles.size();i++){
            if(i<user.letters.size()){
                user.particles[i].update();
                btTransform trans;
                ofPoint temp=user.particles[i].current.pos;
                trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                ofQuaternion rotQuat;
                
                if(bMulti==false){
                    if(i==0){
                        rotQuat = user.shape->getRotationQuat();
                    }
                    else{
                        rotQuat   = user.letters[i]->getRotationQuat();
                    }
                }
                
                else{
                    rotQuat   = user.letters[i]->getRotationQuat();

                }
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
                    if(bMulti==false) {
                    if(i==0){
                        user.shape->getRigidBody()->getMotionState()->setWorldTransform( trans );
                        user.shape->activate();
                    }
                    else{
                        user.letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                        user.letters[i]->activate();
                    }
                }
                    else{

                            user.letters[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
//                            user.letters[i]->activate();
                    }

                
                    
                
                if(!user.particles[i].targetReached){
                    moving++;
                }
            }
        }
        
        
        if (moving==0&&user.bFinished==false){
            user.bNewKey=true;
            userKeyframe++;
            if(userKeyframe>userKeyframes.size()-1){
                user.bFinished=true;
            }
        }
    }
    
    //if in physics state - do nothing other than keep track of duration
    
    else{
        int moving=0;
        
        for (int i=0;i<user.letters.size();i++){
            user.particles[i].update();
            if(!user.particles[i].targetReached){
                moving++;
            }
        }
        
        if (moving==0&&user.bFinished==false){
            user.bNewKey=true;
            userKeyframe++;
            if(userKeyframe>userKeyframes.size()-1){
                user.bFinished=true;
            }
        }
    }
}

void Tweet::destroy(){
    for (int i=0;i<letters.size();i++){
        letters[i]->remove();
    }
    letters.clear();
    particles.clear();
    
    for(int i=0; i<image.shapes.size();i++){
        image.shapes[i]->remove();
        image.face[i].clear();
    }
    image.shapes.clear();
    image.face.clear();
    image.particles.clear();
    
    if(bMulti==false){
    user.shape->remove();
    user.profile.clear();
    }
    
    for(int i=0;i<user.letters.size();i++){
        user.letters[i]->remove();
    }
    user.letters.clear();
    user.particles.clear();
    
    if(bMulti==true){
        multis.clear();
    }
    
    tweetKeyframes.clear();
    imageKeyframes.clear();
    userKeyframes.clear();
    
    
}

void Tweet::switchKey(Particle::Keyframe key, int which){
    
    if(key.path!="gravity"){
        if(which==0){
            tweetToKinematic();
        }
        else if(which==1){
            imgToKinematic();
        }
        else if(which==2){
            userToKinematic();
        }
    }
    //if switching into physics state
    else{
        if(which==0){
            tweetToPhysics();
        }
        else if(which==1){
            imgToPhysics();
        }
        else if(which==2){
            userToPhysics();
        }
        
    }
    
}

void Tweet::loadMulti(vector<db> items){
    
    bMulti=true;
    multis=items;
    
    
    cout<<"load"<<endl;
    
    tweetKeyframe=0;
    imageKeyframe=0;
    userKeyframe=0;
    
    userScale=ofPoint(.1,.1,.1);
    handleScale=ofPoint(.04,.04,.04);
    
    imageScale=ofPoint(40,40,.02);
    imagePos=ofPoint(-100, 42,0);
    
    ofPoint pos=imagePos;
    
    bFinished=true;
    
    bImage=false;
    
    image.bFinished=false;
    user.bFinished=false;
    
    user.bNewKey=true;

    ofxJSONElement json;
    
    bool success=json.open("keyframes/contentBlockKeyframes/multi.json");
    int i=0;
    
    string speed;
    string delay;
    
    
     for(int j=0;j<multis.size();j++){   
    if(multis[j].media_url!=""){
        bool success=pic.loadImage(multis[j].media_url);
        if(success){
            bImage=true;
            cout<<multis[j].media_url<<endl;
        }
        else{
            image.bFinished=true;
            user.bFinished=true;
        }
        pic.clear();
    }
    
    else{
        image.bFinished=true;
    }
    
    if(bImage==true){
        image.bNewKey=true;
    }
     }
    
    ofxJSONElement animations=json["animations"];
    
    tweetIn.path="keyframes/particleKeyframes/in/"+json["animations"][i]["tweet"]["in"]["animation"].asString();
    
    speed=json["animations"][i]["tweet"]["in"]["speed"].asString();
    if(speed!="") tweetIn.speed=ofToInt(speed);
    else tweetIn.speed=1;
    
    
    delay=json["animations"][i]["tweet"]["in"]["delay"].asString();
    if(delay!="") tweetIn.delay=ofToInt(delay);
    else tweetIn.delay=0;
    
    
    tweetOut.path="keyframes/particleKeyframes/out/"+json["animations"][i]["tweet"]["out"]["animation"].asString();
    
    speed=json["animations"][i]["tweet"]["out"]["speed"].asString();
    if(speed!="") tweetOut.speed=ofToInt(speed);
    else tweetOut.speed=1;
    
    delay=json["animations"][i]["tweet"]["out"]["delay"].asString();
    if(delay!="") tweetOut.delay=ofToInt(delay)+masterDelay;
    else tweetOut.delay=masterDelay;
    
    if(bImage==true){
        imgIn.path="keyframes/particleKeyframes/in/"+json["animations"][i]["image"]["in"]["animation"].asString();
        speed=json["animations"][i]["image"]["in"]["speed"].asString();
        if(speed!="") imgIn.speed=ofToInt(speed);
        else imgIn.speed=1;
        
        delay=json["animations"][i]["image"]["in"]["delay"].asString();
        if(delay!="") imgIn.delay=ofToInt(delay);
        else imgIn.delay=0;
        
        
        imgOut.path="keyframes/particleKeyframes/out/"+json["animations"][i]["image"]["out"]["animation"].asString();
        speed=json["animations"][i]["image"]["out"]["speed"].asString();
        if(speed!="") imgOut.speed=ofToInt(speed);
        else imgOut.speed=1;
        
        delay=json["animations"][i]["image"]["out"]["delay"].asString();
        if(delay!="") imgOut.delay=ofToInt(delay)+masterDelay;
        else imgOut.delay=masterDelay;
    }
    
    
    userIn.path="keyframes/particleKeyframes/in/"+json["animations"][i]["user"]["in"]["animation"].asString();
    speed=json["animations"][i]["user"]["in"]["speed"].asString();
    if(speed!="") userIn.speed=ofToInt(speed);
    else userIn.speed=1;
    
    delay=json["animations"][i]["user"]["in"]["delay"].asString();
    if(delay!="") userIn.delay=ofToInt(delay);
    else userIn.delay=0;
    
    
    userOut.path="keyframes/particleKeyframes/out/"+json["animations"][i]["user"]["out"]["animation"].asString();
    speed=json["animations"][i]["user"]["out"]["speed"].asString();
    if(speed!="") userOut.speed=ofToInt(speed);
    else userOut.speed=1;
    
    delay=json["animations"][i]["user"]["out"]["delay"].asString();
    if(delay!="") userOut.delay=ofToInt(delay)+masterDelay;
    else userOut.delay=masterDelay;
    
    loadParticleKeyframes(tweetIn, KEYFRAME_TWEET, true);
    loadParticleKeyframes(tweetOut,KEYFRAME_TWEET,false);
    
    if(bImage==true){
        loadParticleKeyframes(imgIn, KEYFRAME_IMAGE, true);
        loadParticleKeyframes(imgOut,KEYFRAME_IMAGE, false);
    }
    
    loadParticleKeyframes(userIn, KEYFRAME_USER, true);
    loadParticleKeyframes(userOut,KEYFRAME_USER, false);
    


    

    multis[0].user_screen_name="@"+multis[0].user_screen_name;
    
    for(int j=0;j<multis.size();j++){
        multis[j].user_screen_name="@"+multis[j].user_screen_name;
        
        if(bImage==true){
            
            boxShape = ofBtGetBoxCollisionShape(imageScale.x, imageScale.y, imageScale.z);
            
            //setup position values for displaying whole image
            Particle::Keyframe temp;
            temp.pos.x=pos.x;
            temp.pos.y=pos.y;
            temp.pos.z=pos.z;
            
            Particle particle;
            particle.setup(temp,j, hashMin, hashMax);
            particle.goToPosition(imageKeyframes[imageKeyframe]);
            particle.start.pos=particle.target.pos;
            image.particles.push_back(particle);
            
            //create Bullet shapes for image visualization
            image.shapes.push_back( new ofxBulletBox() );
            
            if(imageType!="gravity"){
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->init(boxShape);
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->create(world->world,particle.target.pos,0.);
                image.shapes[image.shapes.size()-1]->add();
                image.shapes[image.shapes.size()-1]->setProperties(.1,1);
                image.shapes[image.shapes.size()-1]->enableKinematic();
            }
            
            else{
                world->setGravity(ofVec3f(imageKeyframes[imageKeyframe].gravity.x,imageKeyframes[imageKeyframe].gravity.y,imageKeyframes[imageKeyframe].gravity.z));
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->init(boxShape);
                ((ofxBulletBox*)image.shapes[image.shapes.size()-1])->create(world->world,particle.target.pos,10.);
                image.shapes[image.shapes.size()-1]->add();
                image.shapes[image.shapes.size()-1]->setProperties(.1,1);
            }
            
            //            //crop and allocate image boxes to tiles
            
            ofEnableNormalizedTexCoords();
            ofTexture newFace;
            ofImage tileImage;
            tileImage.allocate(imageScale.x,imageScale.y, OF_IMAGE_COLOR);
            tileImage.loadImage(multis[j].media_url);
            tileImage.mirror(true,true);
            newFace.allocate(imageScale.x,imageScale.y, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            image.face.push_back(newFace);
            tileImage.clear();
            newFace.clear();
            
            
            //create particles for controlling static shape position/rotation
            
        }
        
        ofUniString handleText= ofTextConverter::toUTF32(multis[j].user_screen_name);
        
        mUserPos=pos;
        mUserPos.x-=.45*imageScale.x;
        mUserPos.y-=.4*imageScale.y;
        
        for (int i=0;i<handleText.length();i++){
            if(handleText[i]>32&&handleText[i]<256){
                //setup position values for displaying whole image
                Particle::Keyframe temp;
                ofVec3f boxDim;
                boxDim=gotham->getSize(handleText[i])*handleScale;
                
                mUserPos.x+=.5*boxDim.x+.5;
                temp.pos.x=mUserPos.x;
                temp.pos.y=mUserPos.y+handleScale.y*gotham->letters[handleText[i]]->offset;
                temp.pos.z=mUserPos.z;
                
                
                Particle particle;
                particle.setup(temp,i,handleText[i],boxDim, hashMin, hashMax);
                particle.goToPosition(userKeyframes[userKeyframe]);
                particle.start.pos=particle.target.pos;
                particle.handle=true;
                user.particles.push_back(particle);
                user.bNewKey=false;
                
                if(userType!="gravity"){
                    
                    user.letters.push_back( new ofxBulletCustomShape() );
                    user.letters[user.letters.size()-1]->addMesh(gotham->getMesh(handleText[i]),handleScale,true);
                    user.letters[user.letters.size()-1]->create(world->world,particle.target.pos,0.);
                    user.letters[user.letters.size()-1]->add();
                    user.letters[user.letters.size()-1]->setProperties(.1,1);
                    user.letters[user.letters.size()-1]->enableKinematic();
                }
                
                else{
                    world->setGravity(ofVec3f(userKeyframes[userKeyframe].gravity.x,userKeyframes[userKeyframe].gravity.y,userKeyframes[userKeyframe].gravity.z));
                    user.letters.push_back( new ofxBulletCustomShape() );
                    user.letters[user.letters.size()-1]->addMesh(gotham->getMesh(handleText[i]),handleScale,true);
                    user.letters[user.letters.size()-1]->create(world->world,particle.target.pos,10.);
                    user.letters[user.letters.size()-1]->add();
                    user.letters[user.letters.size()-1]->setProperties(.1,1);

                }
                mUserPos.x+=.5*boxDim.x+.5;
            }
        }
        pos.x+=imageScale.x;
        if(pos.x>120){
            pos.x=imagePos.x;
            pos.y-=imageScale.y;
        }

    }
}

void Tweet::drawMultiImg(){
    //    user.profile.bind();
    //    user.shape->draw();
    //    user.profile.unbind();
    
    if(bImage==true){
        //        ofRotate(180,0,1,0);
        
        for (int i=0;i<image.shapes.size();i++){
            image.face[i].bind();
            image.shapes[i]->draw();
            image.face[i].unbind();
            
            ofSetColor(255,255,255);
        }
    }
    
}






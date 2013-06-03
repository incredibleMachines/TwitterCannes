#include "testApp.h"

#define tileW 40
#define tileH 40
#define tileD 10

#define USE_DOF true
#define CAM_MOVE false
#define CAM_MOUSE false
#define FACES false

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(255);
    
    loadHashtagGUI();
    hashtagGUI->loadSettings("GUI/hashtagGUI.xml");
    
    //camera, lighting dof setup
    setupGL();
    
    //camKeyframe cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    camKeyframe=0;
    particleKeyframe=0;
    loadParticleKeyframes();
    
    //populates vector of images off of specified directory
    imgCount=0;
    loadDir();
    loadImage();
}


//--------------------------------------------------------------
void testApp::update(){
    
    world.update();
    
    //check whether individual particles have reached their destination
    int moving=0;
    for(int i=0;i<particles.size();i++){
        if(!particles[i].targetReached){
            moving++;
        }
    }
    
    //if all have reached, trigger next keyframe position
    if (moving==0){
        bNewParticleKey=true;
        particleKeyframe++;
        if(particleKeyframe>particleKeyframes.size()-1){
            particleKeyframe=0;
        }
        cout<<"next"<<endl;
    }

    //set new particle destinations
    if(bNewParticleKey==true){
        
        //Destroy particles action - triggers image switch
        if(particleKeyframes[particleKeyframe].path==PARTICLE_PATH_DESTROY){
            particleKeyframe++;
            if(particleKeyframe>particleKeyframes.size()-1){
                particleKeyframe=0;
            }
            loadImage();
        }
        else if(particleKeyframes[particleKeyframe].path!=PARTICLE_PATH_PHYSICS){
            for (int i=0;i<shapes.size();i++){
                particles[i].update();
                shapes[i]->enableKinematic();
                btTransform trans;
                ofPoint temp=shapes[i]->getPosition();
                trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                float newRot = rotQuat.w();
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                shapes[i]->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
                particles[i].goToPosition(particleKeyframes[particleKeyframe]);
            }
        }
        else{
            cout<<"gravity"<<endl;
            world.setGravity(ofVec3f(particleKeyframes[particleKeyframe].gravity.x,particleKeyframes[particleKeyframe].gravity.y,particleKeyframes[particleKeyframe].gravity.z));
            world.enableCollisionEvents();
            cout<<world.getGravity()<<endl;
            for (int i=0;i<shapes.size();i++){
                btTransform trans;
                ofPoint temp=shapes[i]->getPosition();
                trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                float newRot = rotQuat.w();
                trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                tempShapes.push_back( new ofxBulletBox() );
                ((ofxBulletBox*)tempShapes[i])->init(boxShape);
                ((ofxBulletBox*)tempShapes[i])->create(world.world,temp,rotQuat,1.);
                tempShapes[i]->add();
                tempShapes[i]->activate();
                particles[i].goToPosition(particleKeyframes[particleKeyframe]);
            }
//            shapes.clear();
            shapes=tempShapes;
//            tempShapes.clear();

        }
        bNewParticleKey=false;
    }
    else{
        if(particleKeyframes[particleKeyframe].path!=PARTICLE_PATH_PHYSICS){
            for (int i=0;i<shapes.size();i++){
                particles[i].update();
                btTransform trans;
                ofPoint temp=particles[i].current.pos;
                trans.setOrigin( btVector3( btScalar(temp.x), btScalar(temp.y), btScalar(temp.z)) );
                ofQuaternion rotQuat = shapes[i]->getRotationQuat();
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
                shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                shapes[i]->activate();
            }
        }
        else{
            for (int i=0;i<shapes.size();i++){
                particles[i].update();
            }
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    

    glEnable(GL_DEPTH_TEST);
    //    ofEnableSmoothing();
    ofEnableLighting();
    ofEnableAlphaBlending();
    
    if (USE_DOF) {
        dof.begin();
        camera.begin(dof.getDimensions());
    } else {
        camera.begin();
    }
    
    
    light.enable();
    light2.enable();
    light.draw();
    
    //material for
    material.begin();
    drawHashtag(hash.pos);
    white.bind();
    background.draw();
    white.unbind();
    
    for (int i=0;i<shapes.size();i++){
        face[i].bind();
        if(bDrawPhysics==true){
            shapes[i]->draw();
        }
        else{
            ofPushMatrix();
//            ofTranslate(particles[i].current.pos);
            shapes[i]->draw();
            ofPopMatrix();
        }
        face[i].unbind();
    }
    
    material.end();
    
    camera.end();
    ofDisableLighting();
    ofDisableArbTex();
    ofDisableAlphaBlending();
    
    if (USE_DOF) {
        dof.end();
        //fbo of dof for drawing
        ofFbo temp=dof.getFbo();
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    glDisable(GL_DEPTH_TEST);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()),ofPoint(800,100,0));
    
    
}

void testApp::loadImage(){
    
    //go to next image in vector and create new particles
    if(imgCount>images.size()-1){
        imgCount=0;
    }
    particleKeyframe=0;
    pic.loadImage(images[imgCount]);
    pic.setImageType(OF_IMAGE_COLOR);
    pic.resize(pic.width/2, pic.height/2);
    pic.mirror(false, false);
    initParticles();
    imgCount++;
}

void testApp::loadDir(){
    
    //use ofDirectory to load image files into vector of strings then close dir
    string path = "images";
    ofDirectory dir(path);
    dir.allowExt("jpg");
    dir.allowExt("png");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++){
        string newImage=dir.getPath(i);
        images.push_back(newImage);
    }
    dir.close();
}

void testApp::loadParticleKeyframes(){
    
    //Populate particle behaviors off of XML documents
    particleXML.loadFile("keyframes/particleKeyframes/particles.xml");
    
    particleXML.pushTag("PARTICLES");
    int numParticles = particleXML.getNumTags("PARTICLE");
    for(int i = 0; i < numParticles; i++){
        particleXML.pushTag("PARTICLE", i);
        Particle::keyframe temp;
        temp.path=particleXML.getValue("PATH",0);
        
        particleXML.pushTag("TYPE");
        temp.type.x=particleXML.getValue("X",0);
        temp.type.y=particleXML.getValue("Y",0);
        temp.type.z=particleXML.getValue("Z",0);
        particleXML.popTag();
        
        particleXML.pushTag("POS");
        temp.pos.x = particleXML.getValue("X", 0);
        temp.pos.y = particleXML.getValue("Y", 0);
        temp.pos.z = particleXML.getValue("Z", 0);
        
        particleXML.pushTag("MIN");
        temp.posMin.x = particleXML.getValue("X", 0);
        temp.posMin.y = particleXML.getValue("Y", 0);
        temp.posMin.z = particleXML.getValue("Z", 0);
        particleXML.popTag();
        
        particleXML.pushTag("MAX");
        temp.posMax.x = particleXML.getValue("X", 0);
        temp.posMax.y = particleXML.getValue("Y", 0);
        temp.posMax.z = particleXML.getValue("Z", 0);
        particleXML.popTag();
        particleXML.popTag();
        
        particleXML.pushTag("DURATION");
        temp.duration= particleXML.getValue("ACTUAL", 0);
        temp.durationMax= particleXML.getValue("MAX", 0);
        temp.durationMin= particleXML.getValue("MIN", 0);
        particleXML.popTag();
        
        particleXML.pushTag("INTERPOLATIONS");
        int interpCount=particleXML.getNumTags("INTERPOLATION");
        for(int j = 0; j < interpCount; j++){
            temp.interpolations.push_back(particleXML.getValue("INTERPOLATION",0,j));
        }
        particleXML.popTag();
        
        particleXML.pushTag("GRAVITY");
        temp.gravity.x = particleXML.getValue("X", 0);
        temp.gravity.y = particleXML.getValue("Y", 0);
        temp.gravity.z = particleXML.getValue("Z", 0);
        particleXML.popTag();
        
        
        particleXML.popTag();
        particleKeyframes.push_back(temp);
    }
    
    particleXML.popTag();
    ofAddListener(hashtagGUI->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::loadCamKeyframes(){
        
    //sequentially loads camera positions, rate, and reached into vector of camPoint objects
    camPoint newCamPoint;
    newCamPoint.pos=ofPoint(600,400,5000);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(200,400,0);
    camKeyframes.push_back(newCamPoint);
    
    newCamPoint.pos=ofPoint(200,400,5000);
    newCamPoint.rate=1;
    newCamPoint.reached=false;
    newCamPoint.lookAt=ofPoint(200,400,0);
    camKeyframes.push_back(newCamPoint);
    
}

//--------------------------------------------------------------
void testApp::setupGL(){
    
    //loads camera keyframe positions into vector
    loadCamKeyframes();
    
    //set up camera
    camera.enableOrtho();
    camPos=ofPoint(600,400,5000);
    camera.setPosition(camPos);
    camera.lookAt(ofPoint(200,400,0));
    
    if (USE_DOF) {
        dof.setup(ofGetWidth(), ofGetHeight());
        dof.setFocalDistance(200);
        dof.setFocalRange(1000);
        dof.setBlurAmount(1);
    }
    
    if(!CAM_MOUSE){
        camera.disableMouseInput();
    }
    
    //set up GL lighting and material
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setSpotlight();
    light.setPosition(950, -100, 300);
    light.lookAt(ofPoint(800,550,-200));
    
    light2.setDiffuseColor(ofColor(255,255,255));
    light2.setSpecularColor(ofColor(255,255,255));
    light2.setSpotlight();
    light2.setPosition(700, 0, 200);
    light2.lookAt(ofPoint(850,600,0));
    
    material.setShininess(100);
    material.setSpecularColor(ofColor(255,255,100));

    //initialize Bullet Physics world
    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    
    //Create static collision objects
    background.create(world.world,ofVec3f(-2000.,-1000.,-250.),0.,10000.,10000.,10.);
    background.setProperties(1, 1);
    background.add();
    
    whiteImg.loadImage("white.jpg");
    white=whiteImg.getTextureReference();
}


//SETUP PARTICLES OFF IMAGE

void testApp::initParticles(){
    
    world.destroy();
    setupGL();
    
    cout<<tempShapes.size()<<endl;
    
    shapes.clear();
    tempShapes.clear();
    particles.clear();
    face.clear();
    
    boxShape = ofBtGetBoxCollisionShape(tileW, tileH, tileD);
    
    for (int x = 0; x < pic.width / tileW; x++){
        for (int y = 0; y < pic.height / tileH; y++){
            
            //setup position values for displaying whole image
            Particle::keyframe temp;
            temp.pos.x=x*tileW+hash.pos.x+pic.getWidth()/2;
            temp.pos.y=y*tileH+hash.pos.y-pic.getHeight()/2;
            temp.pos.z=0;
            
            //create Bullet shapes for image visualization
            shapes.push_back( new ofxBulletBox() );
            ((ofxBulletBox*)shapes[shapes.size()-1])->init(boxShape);
            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world,particleKeyframes[particleKeyframe].pos,0.);
            shapes[shapes.size()-1]->add();
            shapes[shapes.size()-1]->enableKinematic();
            
            //crop and allocate image boxes to tiles
            ofTexture newFace;
            ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
            tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            tileImage.mirror(false,true);
            ofEnableNormalizedTexCoords();
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            face.push_back(newFace);
            
            //create particles for controlling static shape position
            Particle particle;
            particle.setup(temp,x+x*y);
            particles.push_back(particle);
        }
    }
    
    for(int i=0;i<particles.size();i++){
        particles[i].goToPosition(particleKeyframes[particleKeyframe]);
        particles[i].start.pos=particles[i].target.pos;
    }
}


//DRAW HASHTAG LETTERS

void testApp::drawHashtag(ofPoint pos){
    
    ofPushMatrix();
    ofTranslate(pos);
    
    ofScale(1,1,3);
    
    for (int i=0;i<hashletters.size()-1;i++){
        ofPushMatrix();
        ofTranslate(hashletters[i].pos);
        ofRotate(180,1,0,0);
        tempMesh[i].draw();
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
}

void testApp::drawHashtag(int x, int y, int z){
    drawHashtag(ofPoint(x,y,z));
}


//GUI INIT CODE - LOADS MASTER SETTINGS DOCS AND POPULATES GUI OF THEIR CONTENT

void testApp::loadHashtagGUI()
{
    bSingle=true;
    float dim = 16;
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    settings.loadFile("settings.xml");
    
    hashtagGUI = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    settings.pushTag("HASHTAG");
    settings.pushTag("POS");
    hash.pos.x = settings.getValue("X", 0);
    hash.pos.y = settings.getValue("Y", 0);
    hash.pos.z = settings.getValue("Z", 0);
    settings.popTag();
    settings.popTag();
    hashtagGUI->addWidgetDown(new ofxUIToggle( dim, dim, false, "Entire Hashtag"))->setID(-1);
    
    settings.pushTag("LETTERS");
    int numberOfSavedPoints = settings.getNumTags("LETTER");
    int size=100;
    for(int i = 0; i < numberOfSavedPoints; i++){
        settings.pushTag("LETTER", i);
        settings.pushTag("POS");
        hashletter newHash;
        newHash.pos.x = settings.getValue("X", 0);
        newHash.pos.y = settings.getValue("Y", 0);
        newHash.pos.z = settings.getValue("Z", 0);
        settings.popTag();
        newHash.key = settings.getValue("NAME","0");
        string file=settings.getValue("MESH"," ");
        settings.popTag();;
        tempMesh[i].loadModel(file,size);
        hashletters.push_back(newHash);
        hashtagGUI->addWidgetDown(new ofxUIToggle( dim, dim, false, newHash.key))->setID(i);
    }
    settings.popTag();
    ofAddListener(hashtagGUI->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    int num=e.widget->getID();
    ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
    if(num!=-1){
        hashletters[num].active=toggle->getValue();
    }
    else{
        hash.active=toggle->getValue();
    }
}


//USER INPUT FUNCTIONS

void testApp::mousePressed(int x, int y, int button) {
    
}

void testApp::keyPressed(int key){
    
    switch(key){
            
        case 'f':
            bSingle=false;
            return;
            
        case 'h':
            hashtagGUI->toggleVisible();
            break;
            
        case ' ':
            settings.pushTag("LETTERS");
            for(int i = 0; i < hashletters.size(); i++){
                settings.pushTag("LETTER", i);
                settings.pushTag("POS");
                settings.setValue("X", hashletters[i].pos.x);
                settings.setValue("Y", hashletters[i].pos.y);
                settings.setValue("Z", hashletters[i].pos.z);
                settings.popTag();
                settings.popTag();
            }
            settings.popTag();
            
            settings.pushTag("HASHTAG");
            settings.pushTag("POS");
            settings.setValue("X", hash.pos.x);
            settings.setValue("Y", hash.pos.y);
            settings.setValue("Z", hash.pos.z);
            settings.popTag();
            settings.popTag();
            settings.saveFile("settings.xml");
            hashtagGUI->saveSettings("GUI/hashtagGUI.xml");
            return;
            
        case OF_KEY_UP:
            if(hash.active){
                if(!bSingle){
                    hash.pos.y-=10;
                }
                else{
                    hash.pos.y--;
                }
                
            }
            for(int i=0;i<hashletters.size();i++){
                if(hashletters[i].active){
                    if(!bSingle){
                        hashletters[i].pos.y-=10;
                    }
                    else{
                        hashletters[i].pos.y--;
                    }
                    
                }
            }
            return;
            
        case OF_KEY_DOWN:
            if(hash.active){
                if(!bSingle){
                    hash.pos.y+=10;
                }
                else{
                    hash.pos.y++;
                }
                
            }
            for(int i=0;i<hashletters.size();i++){
                if(hashletters[i].active){
                    if(!bSingle){
                        hashletters[i].pos.y+=10;
                    }
                    else{
                        hashletters[i].pos.y++;
                    }
                }
            }
            return;
            
        case OF_KEY_RIGHT:
            if(hash.active){
                if(!bSingle){
                    hash.pos.x+=10;
                }
                else{
                    hash.pos.x++;
                }
                
            }
            for(int i=0;i<hashletters.size();i++){
                if(hashletters[i].active){
                    if(!bSingle){
                        hashletters[i].pos.x+=10;
                    }
                    else{
                        hashletters[i].pos.x++;
                    }
                }
            }
            return;
            
        case OF_KEY_LEFT:
            if(hash.active){
                if(!bSingle){
                    hash.pos.x-=10;
                }
                else{
                    hash.pos.x--;
                }
                
            }
            for(int i=0;i<hashletters.size();i++){
                if(hashletters[i].active){
                    if(!bSingle){
                        hashletters[i].pos.x-=10;
                    }
                    else{
                        hashletters[i].pos.x--;
                    }
                }
            }
            return;
    }
}

void testApp::keyReleased(int key){
    switch(key){
        case 'f':
            bSingle=true;
            return;
            
    }
}

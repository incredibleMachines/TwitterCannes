#include "testApp.h"

#define tileW 40
#define tileH 40

#define USE_DOF true
#define CAM_MOVE false
#define CAM_MOUSE false
#define FACES false

#define CAMERA_PATH_LINE 0
#define CAMERA_PATH_CURVE 1
#define CAMERA_RATE_LINEAR 0
#define CAMERA_RATE_EASE 1
#define CAMERA_RATE_SPEED 2

//--------------------------------------------------------------
void testApp::setup(){
    
    pos=ofPoint(250,800,160);
    ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(255);
    settings.loadFile("settings.xml");
    
    setupGL();
    
    int w=50;
    int h=50;
    
    //imgCount cycles through the images found in specified image directory
    imgCount=0;
    
    //camKeyframe cycles through camPoints vector which includes x,y,z coords and rate of movement between them
    camKeyframe=0;
    particleKeyframe=0;

    //Loads Meshes of 3DS files for #CannesLions logo
    loadTextMeshes();
    loadParticleKeyframes();
    
    //populates vector of images off of specified directory
    loadDir();
    loadImage();
    
    //loads camera keyframe positions into vector
    loadCamKeyframes();
    
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
    
    whiteImg.loadImage("white.jpg");
    white=whiteImg.getTextureReference();
    

    
}
 

//--------------------------------------------------------------
void testApp::update(){
    
    world.update();
    
    if(CAM_MOVE) {
        if(camPos.distance(camKeyframes[camKeyframe].pos)<2){
            camKeyframe++;
        }
        if(camKeyframe>camKeyframes.size()-1){
            camKeyframe=0;
        }
        if (camKeyframe==0){
            ofPoint move=camKeyframes[camKeyframe].pos-camKeyframes[camKeyframes.size()-1].pos;
            move.normalize();
            camPos=camPos+camKeyframes[camKeyframe].rate*move;
        }
        else{
            ofPoint move=camKeyframes[camKeyframe].pos-camKeyframes[camKeyframe-1].pos;
            move.normalize();
            camPos=camPos+camKeyframes[camKeyframe].rate*move;
        }
        camera.setPosition(camPos);
        camera.lookAt(camKeyframes[camKeyframe].lookAt);
        ofPoint temp=ofPoint(camKeyframes[camKeyframe].lookAt-camPos);;
        if(USE_DOF){
            dof.setFocalDistance(200);
            dof.setFocalRange(1000);
        }
    }
    
    else if(bCamChange==true){
        camera.setPosition(camKeyframes[camKeyframe].pos);
        camera.lookAt(camKeyframes[camKeyframe].lookAt);
        ofPoint temp=ofPoint(camKeyframes[camKeyframe].lookAt-camPos);
        if(USE_DOF){
            dof.setFocalDistance(temp.distance(camKeyframes[camKeyframe].lookAt)/4);
            dof.setFocalRange(1000);
        }
        bCamChange=false;
    }
    
    
    //switch out of Bullet physics and into eased particle functions
    if(bDrawPhysics==false){
        if(bSwitchPhysics==true){
            for (int i=0;i<shapes.size();i++){
                particles[i].bullet.pos=shapes[i]->getPosition();
                
                if(i==0)
                {
                    cout<<"init: "<<shapes[i]->getRotation()<<endl;
                }
                
                particles[i].bullet.quat = shapes[i]->getRotationQuat();

                particles[i].pixel.pos=particles[i].pixel.pos-particles[i].bullet.pos;
                particles[i].goToPosition(particles[i].pixel);
                shapes[i]->enableKinematic();
            }
            bSwitchPhysics=false;
        }
        else{
            for (int i=0;i<shapes.size();i++){
                particles[i].update();
                
                if(particles[i].bRotate==true){
                    ofVec3f pos = shapes[i]->getPosition();
                    btTransform trans;
                    trans.setOrigin( btVector3( btScalar(pos.x), btScalar(pos.y), btScalar(pos.z)) );
                    ofQuaternion rotQuat = shapes[i]->getRotationQuat();
                    float newRot = rotQuat.w();
                    newRot -= .05f;
                    if (newRot<0){
                        newRot=0;
                        particles[i].bRotate=false;
                    }
                    trans.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), rotQuat.z()), newRot) );
                    shapes[i]->getRigidBody()->getMotionState()->setWorldTransform( trans );
                    shapes[i]->activate();
                }

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
    drawHashtag(pos);
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
            ofTranslate(particles[i].current.pos);
            shapes[i]->draw();
            ofPopMatrix();
        }
        face[i].unbind();
    }
    
    material.end();
	
	camera.end();
    ofDisableLighting();
    ofDisableArbTex();

	if (USE_DOF) {
        dof.end();
        //fbo of dof for drawing
        ofFbo temp=dof.getFbo();
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
	}
    glDisable(GL_DEPTH_TEST);	
	
}

void testApp::loadImage(){
    
    //go to next image in vector, create particles out of it
    
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
    
    //uses ofDirectory to load image files into vector of strings then closes dir
    
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
    
    Particle::keyframe start;
    start.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_ABS);
    start.posMin.z=600;
    start.posMax.z=800;
    particleKeyframes.push_back(start);
    
    Particle::keyframe imagePlane;
    imagePlane.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL);
    imagePlane.duration=4000;
    imagePlane.durationMin=0;
    imagePlane.durationMax=2500;
    imagePlane.interpolation = PARTICLE_INTERPOLATE_QUINT_EASE_OUT;
    particleKeyframes.push_back(imagePlane);
    
    Particle::keyframe holdImage;
    holdImage.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL);
    holdImage.duration=2000;
    particleKeyframes.push_back(holdImage);
    
    Particle::keyframe exit;
    exit.type=ofPoint(PARTICLE_POS_PIXEL, PARTICLE_POS_PIXEL, PARTICLE_POS_ABS);
    exit.pos.z=500;
    exit.posMin.z=0;
    exit.posMax.z=100;
    exit.duration=5000;
    exit.durationMin=0;
    exit.durationMax=500;
    exit.interpolation = PARTICLE_INTERPOLATE_ELASTIC_EASE_IN;
    particleKeyframes.push_back(exit);
}

void testApp::loadCamKeyframes(){
    
    bCamChange=false;
    
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
    
//    ofSetSmoothLighting(true);
    
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
    
    world.setup();
    world.enableCollisionEvents();
    world.setCamera(&camera);
    
    ground.create(world.world, pos, 0., -100, 28, 80 );
	ground.setProperties(1, 1);
	ground.add();
    groundLow.create(world.world, pos+ofPoint(600,0,0), 0., 500, 28, 80 );
	groundLow.setProperties(1, 1);
	groundLow.add();
    
    background.create(world.world,ofVec3f(-2000.,-1000.,-200.),0.,10000.,10000.,10.);
    background.setProperties(1, 1);
    background.add();
    
}

void testApp::loadTextMeshes(){
    float size=100;
    hashtag.loadModel("OBJ_letters/hashtag/hashtag.3ds",size);
    hashtag.setRotation(1, 180, 1, 0, 0);
    C_upper.loadModel("OBJ_letters/C_upper/C_upper.3ds",size);
    C_upper.setRotation(1, 180, 1, 0, 0);
    A_lower.loadModel("OBJ_letters/A_lower/A_lower.3ds",size);
    A_lower.setRotation(1, 180, 1, 0, 0);
    N_lower.loadModel("OBJ_letters/N_lower/N_lower.3ds",size);
    N_lower.setRotation(1, 180, 1, 0, 0);
    E_lower.loadModel("OBJ_letters/E_lower/E_lower.3ds",size);
    E_lower.setRotation(1, 180, 1, 0, 0);
    S_lower.loadModel("OBJ_letters/S_lower/S_lower.3ds",size);
    S_lower.setRotation(1, 180, 1, 0, 0);
    L_upper.loadModel("OBJ_letters/L_upper/L_upper.3ds",size);
    L_upper.setRotation(1, 180, 1, 0, 0);
    I_lower.loadModel("OBJ_letters/I_lower/I_lower.3ds",size);
    I_lower.setRotation(1, 180, 1, 0, 0);
    O_lower.loadModel("OBJ_letters/O_lower/O_lower.3ds",size);
    O_lower.setRotation(1, 180, 1, 0, 0);
    hash=ofPoint(settings.getValue("LETTERS:HASH:POS:X",0),settings.getValue("LETTERS:HASH:POS:Y",0),settings.getValue("LETTERS:HASH:POS:Z",0));
    C=ofPoint(settings.getValue("LETTERS:C:POS:X",0),settings.getValue("LETTERS:C:POS:Y",0),settings.getValue("LETTERS:C:POS:Z",0));
    a=ofPoint(settings.getValue("LETTERS:a:POS:X",0),settings.getValue("LETTERS:a:POS:Y",0),settings.getValue("LETTERS:a:POS:Z",0));
    n1=ofPoint(settings.getValue("LETTERS:n1:POS:X",0),settings.getValue("LETTERS:n1:POS:Y",0),settings.getValue("LETTERS:n1:POS:Z",0));
    n2=ofPoint(settings.getValue("LETTERS:n2:POS:X",0),settings.getValue("LETTERS:n2:POS:Y",0),settings.getValue("LETTERS:n2:POS:Z",0));
    e=ofPoint(settings.getValue("LETTERS:e:POS:X",0),settings.getValue("LETTERS:e:POS:Y",0),settings.getValue("LETTERS:e:POS:Z",0));
    s1=ofPoint(settings.getValue("LETTERS:s1:POS:X",0),settings.getValue("LETTERS:s1:POS:Y",0),settings.getValue("LETTERS:s1:POS:Z",0));
    l=ofPoint(settings.getValue("LETTERS:L:POS:X",0),settings.getValue("LETTERS:L:POS:Y",0),settings.getValue("LETTERS:L:POS:Z",0));
    i=ofPoint(settings.getValue("LETTERS:i:POS:X",0),settings.getValue("LETTERS:i:POS:Y",0),settings.getValue("LETTERS:i:POS:Z",0));
    o=ofPoint(settings.getValue("LETTERS:o:POS:X",0),settings.getValue("LETTERS:o:POS:Y",0),settings.getValue("LETTERS:o:POS:Z",0));
    n3=ofPoint(settings.getValue("LETTERS:n3:POS:X",0),settings.getValue("LETTERS:n3:POS:Y",0),settings.getValue("LETTERS:n3:POS:Z",0));
    s2=ofPoint(settings.getValue("LETTERS:s2:POS:X",0),settings.getValue("LETTERS:s2:POS:Y",0),settings.getValue("LETTERS:s2:POS:Z",0));
}

void testApp::initParticles(){
    
    shapes.clear();
    particles.clear();
    
    //particle bools
    bDrawPhysics=true;
    bSwitchPhysics=false;
    bStart=true;
    
    world.setGravity(ofVec3f(0,2000,0));
    
    boxShape = ofBtGetBoxCollisionShape(tileW, tileH, 2);
    
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
            //crop image for texture on particle
			ofImage tileImage;
            tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
            shapes.push_back( new ofxBulletBox() );
            ((ofxBulletBox*)shapes[shapes.size()-1])->init(boxShape);
            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world, ofVec3f(ofRandom(pos.x, pos.x+ofGetWidth()), ofRandom(-200,-600), ofRandom(pos.z-20,pos.z)), 10.);
            
//            ((ofxBulletBox*)shapes[shapes.size()-1])->create(world.world, ofVec3f(ofRandom(ofGetWidth(),ofGetWidth()+200), ofRandom(0,ofGetHeight()), ofRandom(150,160)), 1.);
            
            
            shapes[shapes.size()-1]->setActivationState( DISABLE_DEACTIVATION );
            shapes[shapes.size()-1]->add();
            tileImage.mirror(false,true);
            ofEnableNormalizedTexCoords();
            ofTexture newFace;
            newFace.allocate(tileW,tileH, GL_RGB,true);
            newFace=tileImage.getTextureReference();
            face.push_back(newFace);
            
            Particle::keyframe temp;
            temp.pos.x=x*tileW+pos.x+pic.getWidth()/2;
            temp.pos.y=y*tileH+pos.y-pic.getHeight()/2;
            temp.pos.z=0;
            temp.duration=10000;
            temp.interpolation=PARTICLE_INTERPOLATE_QUAD_EASE_OUT;
            Particle particle;
            particle.setup(temp,x+x*y);
            particles.push_back(particle);
        }
    }
}

void testApp::drawHashtag(ofPoint pos){
    
    ofPushMatrix();
    ofTranslate(pos);
    
//    ofScale(1,1,abs(sin(count)*3.2));
    ofScale(1,1,3);
//    count+=.01;

    
    ofPushMatrix();
    ofTranslate(hash);
    hashtag.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(C);
    C_upper.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(a);
    A_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(n1);
    N_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(n2);
    N_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(e);
    E_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(s1);
    S_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(l);
    L_upper.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(i);
    I_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(o);
    O_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(n3);
    N_lower.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(s2);
    S_lower.draw();
    ofPopMatrix();
    
    ofPopMatrix();
    
}

void testApp::drawHashtag(int x, int y, int z){
    drawHashtag(ofPoint(x,y,z));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	world.setGravity(ofVec3f(0, 0, 0));
    bDrawPhysics=false;
    if(bStart==true){
    bSwitchPhysics=true;
        bStart=false;
    }
}

void testApp::keyPressed(int key){

    switch(key){
        case OF_KEY_UP:
            bUp=true;
            cout<<"up"<<endl;
            return;
            
        case OF_KEY_DOWN:
            bDown=true;
            return;
            
        case OF_KEY_RIGHT:
            bRight=true;
            return;
            
        case OF_KEY_LEFT:
            bLeft=true;
            return;
            
        case '3':
            if(bUp){
                hash.y--;
                
            }
            else if(bDown){
                hash.y++;
            }
            else if(bLeft){
                hash.x--;
            }
            else if(bRight){
                hash.x++;
            }
            return;
            
        case 'c':
            cout<<"c"<<endl;
            if(bUp){
                C.y--;
                
            }
            else if(bDown){
                C.y++;
            }
            else if(bLeft){
                C.x--;
            }
            else if(bRight){
                C.x++;
            }
            return;
            
        case 'a':
            if(bUp){
                a.y--;
            }
            else if(bDown){
                a.y++;
            }
            else if(bLeft){
                a.x--;
            }
            else if(bRight){
                a.x++;
            }
            return;
            
        case 'n':
            if(bUp){
                n1.y--;
            }
            else if(bDown){
                n1.y++;
            }
            else if(bLeft){
                n1.x--;
            }
            else if(bRight){
                n1.x++;
            }
            return;
            
        case 'm':
            if(bUp){
                n2.y--;
            }
            else if(bDown){
                n2.y++;
            }
            else if(bLeft){
                n2.x--;
            }
            else if(bRight){
                n2.x++;
            }
            return;
            
        case 'e':
            if(bUp){
                e.y--;
            }
            else if(bDown){
                e.y++;
            }
            else if(bLeft){
                e.x--;
            }
            else if(bRight){
                e.x++;
            }
            return;
            
        case 's':
            if(bUp){
                s1.y--;
            }
            else if(bDown){
                s1.y++;
            }
            else if(bLeft){
                s1.x--;
            }
            else if(bRight){
                s1.x++;
            }
            return;
            
        case 'l':
            if(bUp){
                l.y--;
            }
            else if(bDown){
                l.y++;
            }
            else if(bLeft){
                l.x--;
            }
            else if(bRight){
                l.x++;
            }
            return;
            
        case 'i':
            if(bUp){
                i.y--;
            }
            else if(bDown){
                i.y++;
            }
            else if(bLeft){
                i.x--;
            }
            else if(bRight){
                i.x++;
            }
            return;
            
        case 'o':
            if(bUp){
                o.y--;
            }
            else if(bDown){
                o.y++;
            }
            else if(bLeft){
                o.x--;
            }
            else if(bRight){
                o.x++;
            }
            return;
            
        case ',':
            if(bUp){
                n3.y--;
            }
            else if(bDown){
                n3.y++;
            }
            else if(bLeft){
                n3.x--;
            }
            else if(bRight){
                n3.x++;
            }
            return;
            
        case 'd':
            if(bUp){
                s2.y--;
            }
            else if(bDown){
                s2.y++;
            }
            else if(bLeft){
                s2.x--;
            }
            else if(bRight){
                s2.x++;
            }
            return;

            
    }
    
}

void testApp::keyReleased(int key){
    switch(key){
        case ' ':
            settings.setValue("LETTERS:HASH:POS:X",hash.x);
            settings.setValue("LETTERS:HASH:POS:Y",hash.y);
            settings.setValue("LETTERS:HASH:POS:Z",hash.z);
            settings.setValue("LETTERS:C:POS:X",C.x);
            settings.setValue("LETTERS:C:POS:Y",C.y);
            settings.setValue("LETTERS:C:POS:Z",C.z);
            settings.setValue("LETTERS:a:POS:X",a.x);
            settings.setValue("LETTERS:a:POS:Y",a.y);
            settings.setValue("LETTERS:a:POS:Z",a.z);
            settings.setValue("LETTERS:n1:POS:X",n1.x);settings.setValue("LETTERS:n1:POS:Y",n1.y);settings.setValue("LETTERS:n1:POS:Z",n1.z);
            settings.setValue("LETTERS:n2:POS:X",n2.x);settings.setValue("LETTERS:n2:POS:Y",n2.y);settings.setValue("LETTERS:n2:POS:Z",n3.z);
            settings.setValue("LETTERS:e:POS:X",e.x);settings.setValue("LETTERS:e:POS:Y",e.y);settings.setValue("LETTERS:e:POS:Z",e.z);
            settings.setValue("LETTERS:s1:POS:X",s1.x);settings.setValue("LETTERS:s1:POS:Y",s1.y);settings.setValue("LETTERS:s1:POS:Z",s1.z);
            settings.setValue("LETTERS:L:POS:X",l.x);settings.setValue("LETTERS:L:POS:Y",l.y);settings.setValue("LETTERS:L:POS:Z",l.z);
            
            settings.setValue("LETTERS:i:POS:X",i.x);settings.setValue("LETTERS:i:POS:Y",i.y);settings.setValue("LETTERS:i:POS:Z",i.z);
            
            settings.setValue("LETTERS:o:POS:X",o.x);settings.setValue("LETTERS:o:POS:Y",o.y);settings.setValue("LETTERS:o:POS:Z",o.z);
            
            settings.setValue("LETTERS:n3:POS:X",n3.x);settings.setValue("LETTERS:n3:POS:Y",n3.y);settings.setValue("LETTERS:n3:POS:Z",n3.z);
            settings.setValue("LETTERS:s2:POS:X",s2.x);settings.setValue("LETTERS:s2:POS:Y",s2.y);settings.setValue("LETTERS:s2:POS:Z",s2.z);
            
            settings.saveFile("settings.xml");
            return;
            
        case OF_KEY_UP:
            bUp=false;
            return;
            
        case OF_KEY_DOWN:
            bDown=false;
            return;
            
        case OF_KEY_RIGHT:
            bRight=false;
            return;
        
        case OF_KEY_LEFT:
            bLeft=false;
            return;
            

    }
}
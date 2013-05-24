#include "testApp.h"

#define tileW 20
#define tileH 20

#define USE_DOF true

//--------------------------------------------------------------
void testApp::setup(){
    
    bool bZ=true;
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	//ofEnableSmoothing();
	ofSetRectMode(OF_RECTMODE_CENTER);
    
	//ofSetCircleResolution(10);
    
    pic.loadImage("image.jpg");
    pic.setImageType(OF_IMAGE_COLOR);
	pic.resize(pic.width/2, pic.height/2);
	pic.mirror(true, true);
    
    populatePixels();
    //createParticles();
	
	ofBackground(255);
	ofSetColor(255);
	
	
	if (USE_DOF) {
		dof.setup(ofGetWidth(), ofGetHeight());
		dof.setFocalDistance(320);
		dof.setFocalRange(200);
        dof.setBlurAmount(1);
	}

//
//	camera.setDistance(1000);
//	camera.setFov(100);
	


//	camera.rotateAround(90, ofPoint(0, 0, 1), camera.getPosition());
//	camera.roll(30);
    
    camera.disableMouseInput();
    camPos.x=pic.width/2;
    camPos.y=pic.height/2;
    camPos.z=1000;
    
//	camera.setDistance(100);
	camera.setPosition(camPos.x, camPos.y, camPos.z);
	camera.lookAt(ofPoint(pic.width/2, pic.height/2, 0));
    
    bZ=true;
    bX=false;
    
    light.enable();
    light.setDirectional();
    light.setPosition(0,0,150);
    
}

//--------------------------------------------------------------
void testApp::update(){
    int count=0;
	
    for(int i=0; i<particles.size(); i++){
        particles[i].update();

        if(i==0){
            cout<<particles[i].targetReached<<endl;
        }
		
		if (!particles[i].targetReached) {
			        count++;
		}
    }
	
	if (count<100) {
		for(int i=0; i<particles.size(); i++){
			particles[i].flipTarget();
            
            
                bZ=!bZ;

		}
	}
    
    if(bX==true){
        rotate+=.01;
    }
    else{
        rotate-=.01;
    }


    if(bZ==true){
        camPos.z+=.4;
    }
    else{
        camPos.z-=.4;
    }

//    if(camPos.z>999){
//        bZ=false;
//    }
//    if(camPos.z<200){
//        bZ=true;
//    }
    
    if(rotate<-60){
        bX=true;
    }
    else if(rotate>60){
        bX=false;
    }
    
	camera.setPosition(camPos.x, camPos.y, camPos.z);
    
    cout<<camPos<<endl;
}

//--------------------------------------------------------------
void testApp::draw(){
	if (USE_DOF) {
		dof.begin();
		camera.begin(dof.getDimensions());
	} else {
		camera.begin();
	}

	
	
	ofSetColor(255);
//	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
	
	camera.end();
	if (USE_DOF) {
		dof.end();
        ofFbo temp=dof.getFbo();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotate(180);
        temp.draw(0,0,ofGetWidth(),ofGetHeight());
        ofPopMatrix();
	}
	
	
}

void testApp::populatePixels(){
    //imagePixels = pic.getPixels();
	
	int total = pic.width/tileW * pic.height/tileH;
	
    for (int x = 0; x < pic.width / tileW; x++){
		for (int y = 0; y < pic.height / tileH; y++){
			
			
			ofImage tileImage;
			//tileImage.clone(pic);
			//tileImage.crop(x*tileW, y*tileH, tileW, tileH);
			tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(pic, x*tileW, y*tileH, tileW, tileH);
			
			Particle p;
            p.setup(ofPoint(x*tileW, y*tileH), tileImage, pic.width, pic.height);
			particles.push_back(p);
		}
	}
}

void testApp::createParticles(){
//    for(int i=0;i<pixels.size();i++){
//        Particle newParticle;
//        newParticle.setup(pixels[i].pos, pixels[i].r, pixels[i].g, pixels[i].b, pic.getPixels(), pic.width, pic.height);
//        particles.push_back(newParticle);
//    }
}

void testApp::mouseDragged(int x, int y, int button) {
	printf("Position: %f, %f, %f\n", camera.getX(), camera.getY(), camera.getZ());
	printf("Target: %f, %f, %f\n", camera.getTarget().getX(), camera.getTarget().getY(), camera.getTarget().getZ());
	printf("Roll: %f\n", camera.getRoll());
	printf("Distance: %f\n", camera.getDistance());
}
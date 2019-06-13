#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(10, 10, 10);
    
    // Set up Bullet Physics
    

    camera.setPosition(ofVec3f(0, -7.f, -10.f));
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    
    world.setup();
    
    world.enableGrabbing();
    world.enableCollisionEvents();
    ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
    world.enableDebugDraw();
    
    world.setCamera(&camera);
    
    world.setGravity(ofVec3f(0, 0, 0));
    
    sphere = new ofxBulletSphere();
    sphere->create(world.world, ofVec3f(7, 0, 0), 0.1, 0.25);
    sphere->add();
    
    ofVec3f startLoc;
    ofPoint dimens;
    boundsWidth = 30.;
    float hwidth = boundsWidth*.5;
    float depth = 2.;
    float hdepth = depth*.5;
    boundsShape = new ofxBulletCustomShape();
    boundsShape->create(world.world, ofVec3f(0, 0, 0), 10.);
    
    for(int i = 0; i < 6; i++) {
        bounds.push_back( new ofxBulletBox() );
        if(i == 0) { // ground //
            startLoc.set( 0., hwidth+hdepth, 0. );
            dimens.set(boundsWidth, depth, boundsWidth);
        } else if (i == 1) { // back wall //
            startLoc.set(0, 0, hwidth+hdepth);
            dimens.set(boundsWidth, boundsWidth, depth);
        } else if (i == 2) { // right wall //
            startLoc.set(hwidth+hdepth, 0, 0.);
            dimens.set(depth, boundsWidth, boundsWidth);
        } else if (i == 3) { // left wall //
            startLoc.set(-hwidth-hdepth, 0, 0.);
            dimens.set(depth, boundsWidth, boundsWidth);
        } else if (i == 4) { // ceiling //
            startLoc.set(0, -hwidth-hdepth, 0.);
            dimens.set(boundsWidth, depth, boundsWidth);
        } else if (i == 5) { // front wall //
            startLoc.set(0, 0, -hwidth-hdepth);
            dimens.set(boundsWidth, boundsWidth, depth);
        }
        btBoxShape* boxShape = ofBtGetBoxCollisionShape( dimens.x, dimens.y, dimens.z );
        boundsShape->addShape( boxShape, startLoc );
        
        bounds[i]->create( world.world, startLoc, 0., dimens.x, dimens.y, dimens.z );
        bounds[i]->setProperties(.25, .95);
        bounds[i]->add();
    }
    
    bDropBox = false;
    
    // Set up Audio Patch
    
    gate_ctrl.out_trig() >> env;
                            env >> amp.in_mod();
    
    pitch_ctrl >> osc.in_pitch();
                  osc >> amp * dB(-12.0f) >> engine.audio_out(0);
                         amp * dB(-12.0f) >> engine.audio_out(1);
    
    
    
    0.0f    >> env.in_attack();
//    50.0f   >> env.in_decay();
//    0.5f    >> env.in_sustain();
    500.0f  >> env.in_release();
    
    pitch_ctrl.set(40.0f);
    
    // Set up Audio
    engine.listDevices();
    engine.setDeviceID(1);
    
    engine.setup( 44100, 512, 3);
}

//--------------------------------------------------------------
void ofApp::update(){
    world.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    //std::cout << "Sphere Distance From Camera: " << glm::distance(camera.getPosition(), sphere->getPosition()) << std::endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    glEnable(GL_DEPTH_TEST);
    camera.begin();
    
    ofSetLineWidth(1.f);
    ofSetColor(255, 0, 200);
    world.drawDebug();
    
    ofSetColor(0, 0, 0);
    if(!bDropBox) {
        boundsMat.begin();
        for(int i = 0; i < bounds.size()-1; i++) {
            bounds[i]->draw();
        }
        boundsMat.end();
    } else {
        ofNoFill();
        boundsShape->transformGL();
        ofDrawBox(ofVec3f(0, 0,0), boundsWidth);
        boundsShape->restoreTransformGL();
        ofFill();
    }
    
    ofSetColor(255, 255, 255);
    sphere->draw();
    
    camera.end();
}

void ofApp::onCollision(ofxBulletCollisionData& cdata) {
    if(*sphere == cdata) {
        std::cout << "Collided!" << std::endl;
        gate_ctrl.trigger(1.0f);
        //pitch_ctrl.set(cdata.body1->getTotalForce().length());

        //gate_ctrl.off();
        bColliding = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //gate_ctrl.trigger(0.75f);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //gate_ctrl.off();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

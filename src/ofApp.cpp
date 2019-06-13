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
    
    ground.create(world.world, ofVec3f(0., 5.5, 0.), 10., 100.f, 1.f, 100.f);
    ground.setProperties(0.25, 0.95);
    ground.add();
    
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
    
    pitch_ctrl.set(72.0f);
    
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
    
    ofSetColor(100, 100, 100);
    ground.draw();
    
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

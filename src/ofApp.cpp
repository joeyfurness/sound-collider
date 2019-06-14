#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(10, 10, 10);
    
    // Set up Bullet Physics
    

    camera.setPosition(ofVec3f(0, -3.f, -40.f));
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    
    world.setup();
    
    world.enableGrabbing();
    world.enableCollisionEvents();
    ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
    world.enableDebugDraw();
    
    world.setCamera(&camera);
    
    world.setGravity(ofVec3f(0, 0, 0));
    
    sphere = new ofxBulletSphere();
    sphere->create(world.world, ofVec3f(0, 0, 0), 0.5, 2.5);
    sphere->setProperties(0.75, 0.5);
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
        bounds[i]->setProperties(0.75, 0.5);
        bounds[i]->add();
    }
    
    // Set up Audio Patch
    
    gate_ctrl.out_trig() >> env;
                            env >> amp.in_mod();
    
    pan_ctrl >> pan.in_pan();
    pan_ctrl.set(0.0f);
    
//    verb_amt >> verb.in_density();
//    verb_time >> verb.in_time();
//    verb_filter >> verb.in_hi_cut();
    
    pitch_ctrl >> osc.in_pitch();
                  //osc >> amp * dB(-12.0f) >> pan >> engine.audio_out(0);
                  //       amp * dB(-12.0f) >> pan >> engine.audio_out(1);
    osc >> amp * dB(-12.0f) >> pan;
    pan.out_L() >> engine.audio_out(0);
    pan.out_R() >> engine.audio_out(1);
    
    0.0f    >> env.in_attack();
//    50.0f   >> env.in_decay();
//    0.5f    >> env.in_sustain();
    500.0f  >> env.in_release();
    
    pitch_ctrl.set(40.0f);
    verbEnable = true;
    
    //Set up GUI !! This isnt working because of some OpenGL stuff
//    gui.setup();
//    verb_group.setName("reverb");
//    verb_group.add(verb_amt.set("density", 0.5, 0.0, 1.0));
//    verb_group.add(verb_time.set("time", 1.0, 0.0, 5.0));
//    verb_group.add(verb_filter.set("filter", 1.0, 0.0, 1.0));
//    gui.add(verb_group);
    
    ofBackground(0);
    
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
    //gui.draw();
    glEnable(GL_DEPTH_TEST);
    camera.begin();
    
    ofSetLineWidth(2.f);
    ofSetColor(255, 0, 200);
    world.drawDebug();
    
    ofNoFill();
    ofSetColor(0, 0, 0);
    boundsMat.begin();
    for(int i = 0; i < bounds.size()-1; i++) {
        bounds[i]->draw();
    }
    boundsMat.end();
    
    ofSetColor(255, 255, 255);
    sphere->draw();
    
    camera.end();
}

void ofApp::onCollision(ofxBulletCollisionData& cdata) {
    if(*sphere == cdata) {
        std::cout << "Collided!" << std::endl;
        std::cout << "Sphere Position (y): " << sphere->getPosition().y << std::endl;
        float mapPitch = ofMap(sphere->getPosition().y, -15.0f, 15.0f, 0.0f, 100.0f);
        float mapPan = ofMap(sphere->getPosition().x, -15.0f, 15.0f, -1.0f, 1.0f);
        pitch_ctrl.set(mapPitch);
        pan_ctrl.set(mapPan);
        gate_ctrl.trigger(1.0f);
        //pitch_ctrl.set(cdata.body1->getTotalForce().length());

        //gate_ctrl.off();
        bColliding = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        verbEnable = !verbEnable;
    }
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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	playerImage.load("/images/mask-head.png");

	gui.setup();
	gui.add(speed.setup("speed", 200, 50, 500));
	gui.add(rate.setup("rate", 1, 1, 10));
	gui.add(emitterVelocity.setup("Emitter Velocity", glm::vec3(1, 1, 0), glm::vec3(-2, -2, 0), glm::vec3(2, 2, 0)));
	universalSpriteSystem = SpriteSystem();

	/*e = EmittingSprite();
	e.setImage(playerImage);
	e.lifespan = -1;
	e.sys = &universalSpriteSystem;
	e.physicsObject = true;*/

	player = Player();
	player.lifespan = -1;
	player.sys = &universalSpriteSystem;

	enemy = Enemy();
	enemy.lifespan = -1;
	enemy.sys = &universalSpriteSystem;

	//universalSpriteSystem.add(&e);
	universalSpriteSystem.add(&enemy);
	universalSpriteSystem.add(&player);


}

//--------------------------------------------------------------
void ofApp::update(){
	if (started) {
		universalSpriteSystem.update();
		checkCollisons();
		player.velocity = glm::vec3(0, 0, 0);
		if (keysPressed['w'] || keysPressed['W']) {
			player.velocity += glm::vec3(0, -100, 0);
		}
		if (keysPressed['s'] || keysPressed['S']) {
			player.velocity +=  glm::vec3(0, 100, 0);
		}
		if (keysPressed['a'] || keysPressed['A']) {
			player.velocity += glm::vec3(-100, 0, 0);
		}
		if (keysPressed['d'] || keysPressed['D']) {
			player.velocity += glm::vec3(100, 0, 0);
		}
		if (e.velocity.x != 0 || e.velocity.y != 0) {
			player.velocity = static_cast<float>(speed) * glm::normalize(player.velocity);
		}
	}
	if (keysPressed[' ']) {
		started = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::maroon);
	universalSpriteSystem.draw();
	if (!bHide) {
		gui.draw();
	}
}

void ofApp::checkCollisons() {
	/*
	for (int i = 0; i < ship.s.children.size(); i++) {
		es.removeNear(ship.s.children[i]->position, ship.s.children[i]->width);
	}*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	keysPressed[key] = true;
	if (key == 104 || key == 72) {
		bHide = !bHide;
	}
	if (key == 32) {
		started = true;
	}
	if (key == OF_KEY_LEFT) {
		player.fireDirection = LEFT;
	}
	if (key == OF_KEY_RIGHT) {
		player.fireDirection = RIGHT;

	}
	if (key == OF_KEY_DOWN) {
		player.fireDirection = DOWN;
	}
	if (key == OF_KEY_UP) {
		player.fireDirection = UP;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	keysPressed[key] = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

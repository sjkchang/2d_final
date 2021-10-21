#include "GameObject.h"

GameObject::GameObject() {
	position = glm::vec3(500, 500, 1);
	scale = glm::vec3(1, 1, 1);
	rotation = 0;
	
	birthtime = ofGetElapsedTimeMillis();
	lifespan = 2;
}

glm::mat4 GameObject::getTransMatrix() {
	glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(this->position));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(this->rotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);

	return trans * rot * scale;
}

glm::vec3 GameObject::heading() {
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
	return rot * glm::vec4(0, -1, 0, 1);
}

float GameObject::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

bool GameObject::isAlive() {
	if (lifespan*1000 < age() && lifespan != -1) {
		return false;
	}
	return true;
}

//---------------------------------------------------

PhysicsObject::PhysicsObject() {
	GameObject::GameObject();
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
	mass = 1;
}

void PhysicsObject::integrate() {
	float dt = 1 / ofGetFrameRate();
	position = position + velocity*dt;
	velocity = velocity + acceleration*dt;
	acceleration = force / mass;
}

//---------------------------------------------------

Sprite::Sprite() {
	GameObject::GameObject();
	name = "UnamedSprite";
	color = ofColor(255, 255, 255);
	width = 10;
	height = 10;
}

void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}

void Sprite::setImage(string filePath) {
	image.load(filePath);
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}

void Sprite::draw() {
	ofPushMatrix();
	ofMultMatrix(getTransMatrix());
	if (haveImage) {
		ofSetColor(255, 255, 255);
		image.draw(-width / 2.0, -height / 2.0);
	}
	else {
		ofSetColor(color);
		ofDrawCircle(glm::vec3(0, 0, 1), width);
	}
	ofPopMatrix();
}


void Sprite::update() {
	integrate();
}

//------------------------------------------------------------

void SpriteSystem::add(Sprite * s) {
	children.push_back(s);
}

void SpriteSystem::remove(int i) {
	//delete children[i];
	children.erase(children.begin() + i);
}

void SpriteSystem::update() {

	if (children.size() == 0) return;
	for (int i = 0; i < children.size(); i++) {
		if (!(children[i]->isAlive())) {
			remove(i);
			if (i != children.size() - 1) {
				i--;
			}
		}
	}
	
	for (int i = 0; i < children.size(); i++) {
		children[i]->update();
	}
}

void SpriteSystem::draw() {
	if (children.size() == 0) return;
	for (int i = children.size()-1; i >= 0; i--) {
		children[i]->draw();
	}
}

//-------------------------------------
EmittingSprite::EmittingSprite() {
	Sprite::Sprite();
	started = true;
	lastSpawned = 0;
	spawnRate = 1;
}

void EmittingSprite::update() {
	Sprite::update();
	spawn();
}

void EmittingSprite::spawn() {
	if (started) {
		if (ofGetElapsedTimeMillis() - lastSpawned > spawnRate * 1000) {
			Sprite * sprite = new Sprite();
			sprite->position = position;
			sprite->color = ofColor(0, 0, 0);
			sys->add(sprite);
			lastSpawned = ofGetElapsedTimeMillis();
		}
	}
}

void EmittingSprite::start() {
	started = true;
}

void EmittingSprite::stop() {
	started = false;
}


//----------------------------------------------------

Player::Player() {
	started = true;
	lastSpawned = 0;
	spawnRate = 1;
	childSpeed = 100;
	childLifespan = 0.7;
	fireDirection = RIGHT;
	spray.load("sounds/spray.mp3");
	setImage("images/mask-head.png");
}

void Player::spawn() {
	if (started) {
		if (ofGetElapsedTimeMillis() - lastSpawned > spawnRate * 1000) {
			spray.play();
			//spawn 8 projectiles in a circle
			float rotIncrement = 90.0;
				glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotIncrement * fireDirection), glm::vec3(0, 0, 1));
				Sprite * sprite = new Sprite();
				sprite->position = position;
				sprite->color = ofColor(0, 0, 0);
				sprite->velocity = childSpeed * rot * glm::vec4(0, -1, 0, 1);
				sprite->lifespan = childLifespan;
				sprite->color = ofColor(0, 255, 255);
				sys->add(sprite);
			
			lastSpawned = ofGetElapsedTimeMillis();
		}
	}

}

//---------------------------------------------------

Enemy::Enemy() {
	Sprite::Sprite();
	started = true;
	lastSpawned = 0;
	spawnRate = 3;
	childSpeed = 100;
	childLifespan = 1;
	cough.load("sounds/single-cough.mp3");
	setImage("images/sick.png");
}

void Enemy::spawn() {
	if (started) {
		if (ofGetElapsedTimeMillis() - lastSpawned > spawnRate * 1000) {
			cough.play();
			//spawn 8 projectiles in a circle
			float rotIncrement = 45.0;
			for (int i = 0; i < 8; i++) {
				glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotIncrement * i), glm::vec3(0, 0, 1));
				Sprite * sprite = new Sprite();
				sprite->position = position;
				sprite->color = ofColor(0, 0, 0);
				sprite->velocity = childSpeed * rot * glm::vec4(0, -1, 0, 1);
				sprite->lifespan = childLifespan;
				sprite->setImage("images/covid.png");
				sys->add(sprite);
			}
			lastSpawned = ofGetElapsedTimeMillis();
		}
	}
	
}
#pragma once

#include "ofMain.h"
enum direction { UP, RIGHT, DOWN, LEFT };

class GameObject {
public:
	GameObject();
	glm::vec3 position, scale;
	float rotation;
	glm::mat4 getTransMatrix();
	glm::vec3 heading();

	float age();
	bool isAlive();
	float birthtime;
	float lifespan;
};

//-------------------------------------------

class PhysicsObject : public GameObject {
public:
	PhysicsObject();
	glm::vec3 velocity, acceleration, force;
	float mass;
	void integrate();
	void applyForce(glm::vec3 f) { force += f; }
};

//-------------------------------------------

class Sprite : public PhysicsObject {
public:
	Sprite();
	float width, height;
	string name;

	ofColor color;
	bool haveImage = false;
	ofImage image;
	void setImage(ofImage img);
	void setImage(string filePath);


	bool physicsObject = false;

	virtual void draw();
	virtual void update();
};

//--------------------------------------------

class SpriteSystem {
public:
	SpriteSystem() {};
	vector<Sprite *> children;
	void add(Sprite *);
	void remove(int);

	virtual void draw();
	virtual void update();
};

//-------------------------------------------

class EmittingSprite : public Sprite {
public:
	EmittingSprite();
	SpriteSystem * sys;
	void update();
	virtual void spawn();
	
	void start();
	void stop();
	bool started;
	float lastSpawned;
	float spawnRate;
};

//--------------------------------------------

class Player : public EmittingSprite {
public:
	Player();
	void spawn();
	ofSoundPlayer spray;
	float childSpeed;
	float childLifespan;
	direction fireDirection; //0 = up, 1 = right, 2 = down, 3 = left

};

//--------------------------------------------

class Enemy : public EmittingSprite {
public:
	Enemy();
	void spawn();
	ofSoundPlayer cough;
	float childSpeed;
	float childLifespan;
};
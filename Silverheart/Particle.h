#pragma once

#include "agk.h"

#include "GF.h"
#include "DebugConsole.h"
#include "DataReader.h"
#include "zString.h"

#include <vector>
#include <string>

#ifndef P_GLOBALS
#define P_GLOBALS
	extern char p_location[128];
#endif

class Particle
{
public:
	Particle(void);
	~Particle(void);
	
	void setup();
	void update();

	void addFromFile(int ID, std::string filename, float x, float y);
	void cloneParticle(int ID, Particle* clonePart, float x, float y);
	void remove();
	void removeParticle();

	void setPosition(float x, float y);
	void setVisible(int visible);
	void setFinished(bool finished);

	int getID();

	//std::string getImage();
	std::string getImage();
	float getAngle();
	float getDirX();
	float getDirY();
	float getFrequency();
	float getLife();
	float getSize();
	float getStartX();
	float getStartY();
	bool getFinished();

	struct Keyframe
	{
		float t;
		int r;
		int g;
		int b;
		int a;
	};

	std::vector< Keyframe >* getKeyframes();
private:
	int groupID;

	int imgID;
	int PID;
	
	float x;
	float y;
	
	int visible;

	//Particle properties
	//std::string image;
	std::string image;
	float angle;
	float dirX;
	float dirY;
	float frequency;
	float life;
	float size;
	float startX;
	float startY;
	float timeout;

	float createdAt;
	bool isFinished;

	std::vector< Keyframe >* colorFrame;
};

class ParticleGroup
{
public:
	void setup();
	void update();

	int addFromFile(std::string filename, float x, float y);
	int addFromClone(int cloneID);
	void removeParticles(int ID);

	void setParticlePosition(int ID, float x, float y);
	void setParticleVisible(int ID, int visible);

	Particle* findByID(int ID);
private:
	std::vector< Particle >* particles;

	int nextID;
};


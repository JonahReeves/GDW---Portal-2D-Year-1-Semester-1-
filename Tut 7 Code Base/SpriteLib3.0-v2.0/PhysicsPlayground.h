#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();
	

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	//custom stuff
	int kinematicPlat(int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0, std::string fileName = "Portal Wall.png");
	int dynamicPlat(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0, float gravityScale = 0.0);
	int dynamicBall(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0);
	int basicDestroTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int target, float rotationAngleDeg = 0.0);//Setup trigger
	int translateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float movementX, float movementY, int target, int speed = 100000, float rotationAngleDeg = 0.0);//Setup trigger
	int translateTriggerDoors(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float movementX, float movementY, int target, int speed = 100000, int direction = 2, float rotationAngleDeg = 0.0);//Setup trigger
	int basicTranslateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int direction, int target, int speed, float rotationAngleDeg);//Setup trigger
	int boulder(int fileLength, int fileWidth, float xVal, float yVal, int rotation);
	void bluePortal(float xVal, float yVal, float rotationAngleDeg = 0.0);
	void orangePortal(float xVal, float yVal, float rotationAngleDeg = 0.0);
	static void spawnOrangePortal(float xVal, float yVal, float rotationAngleDeg = 0.0);
	static int getBluePortal();
	static int getOrangePortal();
	static void portalPhysics(int target, int entPortal, int exitPortal);
	int portalProj(bool portalColor, float xVal, float yVal, float directionAngle); //true = blue, false = orange
	void addToPortalSurface(int ent);
	static int* getPortalSurfaces();
	static int getContactSurface();
	static int getActiveProj();
	

	void queueoPortal(float xVal, float yVal, float rotationAngleDeg)
	{
		pqxVal = xVal;
		pqyVal = yVal;
		pqrotationAngleDeg = rotationAngleDeg;
		m_portaloQueued = true;


	}

	void queuebPortal(float xVal, float yVal, float rotationAngleDeg)
	{
		bpqxVal = xVal;
		bpqyVal = yVal;
		bpqrotationAngleDeg = rotationAngleDeg;
		m_portalbQueued = true;


	}

	void portaloQueueCheck()
	{
		if (m_portaloQueued)
		{
			orangePortal(pqxVal, pqyVal, pqrotationAngleDeg);
			m_portaloQueued = false;
		}
	}

	void portalbQueueCheck()
	{
		if (m_portalbQueued)
		{
			bluePortal(bpqxVal, bpqyVal, bpqrotationAngleDeg);
			m_portalbQueued = false;
		}
	}

	

protected:
	bool m_portaloQueued = false;
	float pqxVal = 0.f;
	float pqyVal = 0.f;
	float pqrotationAngleDeg = 0.f;


	bool m_portalbQueued = false;
	float bpqxVal = 0.f;
	float bpqyVal = 0.f;
	float bpqrotationAngleDeg = 0.f;

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int puzzleWall1;
	int puzzleWall2;
	int puzzleWall3;
	int puzzleWall4;
	int puzzleWall5;
};

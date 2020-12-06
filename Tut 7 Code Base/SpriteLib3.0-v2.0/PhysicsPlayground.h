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
	int kinematicPlat(std::string fileName, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0);
	int dynamicPlat(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0, float gravityScale = 0.0);
	int dynamicBall(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0);
	int basicDestroTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int target, float rotationAngleDeg = 0.0);//Setup trigger
	int translateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float movementX, float movementY, int target, int speed = 100000, float rotationAngleDeg = 0.0);//Setup trigger
	int basicTranslateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int direction, int target, int speed, float rotationAngleDeg);//Setup trigger
	int boulder(int fileLength, int fileWidth, float xVal, float yVal, int rotation);
	void bluePortal(float xVal, float yVal, float rotationAngleDeg = 0.0);
	void orangePortal(float xVal, float yVal, float rotationAngleDeg = 0.0);
	static int getBluePortal();
	static int getOrangePortal();
	//int dynamicProj(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0, float gravityScale = 0.0, int& projectile);

protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int puzzleWall1;
	int puzzleWall2;
	int puzzleWall3;
	int puzzleWall4;
	int puzzleWall5;
	int projectile;
};

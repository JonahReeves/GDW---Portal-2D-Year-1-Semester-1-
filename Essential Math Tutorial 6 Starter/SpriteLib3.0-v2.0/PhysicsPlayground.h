#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	int staticBasicPlat(std::string fileName, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0);
	int dynamicPlat(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0);
	int dynamicBall(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg = 0.0);
	int basicDestroTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int target, float rotationAngleDeg = 0.0);//Setup trigger
	int basicTranslateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int direction, int target, int speed = 100000, float rotationAngleDeg = 0.0);//Setup trigger
	int boulder(int fileLength, int fileWidth, float xVal, float yVal, int rotation);
protected:
	PhysicsPlaygroundListener listener;

	int movingPlat = 0;
	int expander1 = 0;
	int wall = 0;
	int spinner = 0;
	int puzzleWall1;
	int puzzleWall2;
	int puzzleWall3;
	int puzzleWall4;
	int puzzleWall5;
};

#pragma once
#include "Trigger.h"
#include <Box2D/Box2D.h>

class TranslateTrigger : public Trigger
{
	//enum class direction
	//{

	//	MLEFT = 0,
	//	MRIGHT = 1,
	//	MUP = 2,
	//	MDOWN = 3

	//};
public:
	//int dir;
	//int speed;
	//int toMove;
	//TranslateTrigger(int d, int ent, int spd = 1000000)
	//{

	//	dir = d;
	//	toMove = ent;
	//	speed = spd;
	//}
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;

	b2Vec2 movement;
protected:
	bool triggered = false;
};


#pragma once
#include "Trigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"

class PhysicsPlayground;
class CheckpointTrigger :
    public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;

	void SetScene(PhysicsPlayground* scene);

	CheckpointTrigger() : Trigger() {

	}

	void setLoc(b2Vec2 vec);
	PhysicsPlayground* m_currScene;
	b2Vec2 respawnLoc;
protected:

	bool triggered = false;
};


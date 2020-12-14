#pragma once
#include "Trigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"

class PhysicsPlayground;
class EffectTrigger :
	public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;

	void SetScene(PhysicsPlayground* scene);

	EffectTrigger() : Trigger() {

	}

	
	PhysicsPlayground* m_currScene;
	
protected:

	bool triggered = false;
};




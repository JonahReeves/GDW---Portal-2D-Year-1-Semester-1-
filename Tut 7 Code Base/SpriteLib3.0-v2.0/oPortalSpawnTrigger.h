#pragma once
#include "Trigger.h"
#include "PhysicsPlayground.h"
class oPortalSpawnTrigger :
    public Trigger
{

public:
	
	void OnEnter() override;
	void OnExit() override;
	
	void GetScene(PhysicsPlayground* scene)
	{
		m_currScene = scene;
	}

private:
	PhysicsPlayground* m_currScene = nullptr;
protected:

	bool triggered = false;

};


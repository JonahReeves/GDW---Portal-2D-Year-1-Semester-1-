#pragma once
#include "Trigger.h"
#include "PhysicsPlayground.h"
class oPortalSpawnTrigger :
    public Trigger
{

public:
	
	void OnEnter() override;
	void OnExit() override;
	PhysicsPlayground* m_currScene = nullptr;
	void GetScene(PhysicsPlayground* scene)
	{
		m_currScene = scene;
	}

private:
	
protected:

	bool triggered = false;

};


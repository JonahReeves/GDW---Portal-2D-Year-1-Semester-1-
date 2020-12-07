#pragma once
#include "Trigger.h"

class PhysicsPlayground;
class oPortalSpawnTrigger :
    public Trigger
{

public:
	
	void OnEnter() override;
	void OnExit() override;
	
	void GetScene(PhysicsPlayground* scene);

private:
	PhysicsPlayground* m_currScene;
protected:

	bool triggered = false;

};


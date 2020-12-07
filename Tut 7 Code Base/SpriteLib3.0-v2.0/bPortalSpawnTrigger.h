#pragma once
#include "Trigger.h"
//#include "PhysicsPlayground.h"


class PhysicsPlayground;
class bPortalSpawnTrigger :
	public Trigger
{

public:

	void OnEnter() override;
	void OnExit() override;

	void SetScene(PhysicsPlayground* scene);


	PhysicsPlayground* m_currScene;
protected:

	bool triggered = false;

};


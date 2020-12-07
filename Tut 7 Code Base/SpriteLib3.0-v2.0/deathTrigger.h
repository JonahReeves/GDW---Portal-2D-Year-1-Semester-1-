#pragma once
#include "Trigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"

class deathTrigger :
    public Trigger
{
	
	public:
		void OnEnter() override;
		void OnExit() override;

		void SetScene(PhysicsPlayground* scene);

		deathTrigger() : Trigger() {

		}

		
		PhysicsPlayground* m_currScene;
		
	protected:

		bool triggered = false;
	
};

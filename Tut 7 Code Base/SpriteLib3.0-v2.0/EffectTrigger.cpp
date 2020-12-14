#include "EffectTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"
#include "EffectManager.h"


void EffectTrigger::OnEnter() //teleport user
{
	//get location of the object its attached to
	//set that location as respawn point


	Trigger::OnEnter();

	if (!triggered)
	{
		
		
		triggered = true;
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void EffectTrigger::OnExit() // literally do nothing
{

	Trigger::OnExit();
	triggered = false;

}

void EffectTrigger::SetScene(PhysicsPlayground* scene)
{
	m_currScene = scene;
}


#include "deathTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"
#include "EffectManager.h"

int index;
void deathTrigger::OnEnter() //respawn user
{
	//get location of the object its attached to
	//set that location as respawn point


	Trigger::OnEnter();

	if (!triggered)
	{

		m_currScene->respawn();
		index = EffectManager::CreateEffect(EffectType::Sepia, 300, 300);
		triggered = true;
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void deathTrigger::OnExit() // literally do nothing
{

	Trigger::OnExit();
	triggered = false;
	EffectManager::RemoveEffect(index);
}

void deathTrigger::SetScene(PhysicsPlayground* scene)
{
	m_currScene = scene;
}

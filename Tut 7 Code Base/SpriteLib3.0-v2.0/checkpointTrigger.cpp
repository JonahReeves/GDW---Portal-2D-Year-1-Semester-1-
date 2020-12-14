#include "checkpointTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"
#include "EffectManager.h"

int effect;
void CheckpointTrigger::OnEnter() //teleport user
{
	//get location of the object its attached to
	//set that location as respawn point


	Trigger::OnEnter();

	if (!triggered)
	{
		
		m_currScene->setRespawn(respawnLoc);
		effect = EffectManager::CreateEffect(EffectType::Sepia, 300, 300);
		
		triggered = true;
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void CheckpointTrigger::OnExit() // literally do nothing
{
	
	Trigger::OnExit();
	triggered = false;
	EffectManager::RemoveEffect(effect);
}

void CheckpointTrigger::SetScene(PhysicsPlayground* scene)
{
	m_currScene = scene;
}

void CheckpointTrigger::setLoc(b2Vec2 vec)
{
	respawnLoc = vec;
}
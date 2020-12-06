#include "bPortalTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"


void bPortalTrigger::OnEnter() //teleport user
{
	Trigger::OnEnter();

	if (active)
	{
		auto& t = ECS::GetComponent<Trigger*>(PhysicsPlayground::getOrangePortal());
		t->active = false;

		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			b2Vec2 orangePos = ECS::GetComponent<PhysicsBody>(PhysicsPlayground::getOrangePortal()).GetPosition();

			
			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(orangePos, true);
			//Update physics here
			
			PhysicsPlayground::portalPhysics(m_targetEntities[i], PhysicsPlayground::getBluePortal(), PhysicsPlayground::getOrangePortal());
		}

		
		triggered = true;

		
		
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void bPortalTrigger::OnExit() // literally do nothing
{
	active = true;
	Trigger::OnExit();
}
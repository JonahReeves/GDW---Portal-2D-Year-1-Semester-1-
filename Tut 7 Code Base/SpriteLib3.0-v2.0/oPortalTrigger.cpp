#include "oPortalTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"


void oPortalTrigger::OnEnter() //teleport user
{
	Trigger::OnEnter();

	if (active && PhysicsPlayground::bPortalExists())
	{
		auto& t = ECS::GetComponent<Trigger*>(PhysicsPlayground::getBluePortal());
		t->active = false;

		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			b2Vec2 bluePos = ECS::GetComponent<PhysicsBody>(PhysicsPlayground::getBluePortal()).GetPosition();


			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(bluePos, true);// MAKE SURE TO REENABLE THIS ONCE U ADD A CD TO THE PORTAL TELEPORTS.
			//Update physics here

			PhysicsPlayground::portalPhysics(m_targetEntities[i], PhysicsPlayground::getOrangePortal(), PhysicsPlayground::getBluePortal());
			
			
		}
		
		triggered = true;

		
		
	}


	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void oPortalTrigger::OnExit() // literally do nothing
{
	active = true;
	Trigger::OnExit();
}
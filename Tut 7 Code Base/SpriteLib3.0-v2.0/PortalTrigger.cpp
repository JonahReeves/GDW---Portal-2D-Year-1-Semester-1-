#include "PortalTrigger.h"
#include "ECS.h"


void PortalTrigger::OnEnter() //teleport user
{
	Trigger::OnEnter();

	//if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			//auto& toTele = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]);
			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(b2Vec2(5.f, 5.f), true);

		}

		triggered = true;
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void PortalTrigger::OnExit() // literally do nothing
{
	Trigger::OnExit();
}


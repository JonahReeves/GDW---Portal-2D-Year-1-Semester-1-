#include "bPortalTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"


void bPortalTrigger::OnEnter() //teleport user
{
	Trigger::OnEnter();

	//On enter, check through a global array of entities (players and cubes) and compare their locations with the location of this portal
	//Test to see if they are touching, if yes, set as targetEntity
	//teleport target entity as done below

	//if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			b2Vec2 orangePos = ECS::GetComponent<PhysicsBody>(PhysicsPlayground::getOrangePortal()).GetPosition();

			
			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(orangePos, true);

		}

		triggered = true;
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void bPortalTrigger::OnExit() // literally do nothing
{
	Trigger::OnExit();

}
/* 
bool isactive

when player goes through entrance portal, exit portal's isactive is set to false, (need a setter method that other portal can access)
teleport function on any portal only triggers when isactive is true
when player leaves hitbox of exit portal, set isactive to true once more, making the portal work again, no more infinite teleport glitch
*/

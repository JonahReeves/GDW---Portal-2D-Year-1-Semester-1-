#include "oPortalSpawnTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"


void oPortalSpawnTrigger::OnEnter() //teleport user
{
	Trigger::OnEnter();

	/*
	when projectile hits a wall, 
	scans this array to see what wall it hit by comparing location values,
	then takes the angle of that wall and creates a portal of the correct color at that location with the same rotation angle as the wall it hit
	*/
	int wallImTouching = PhysicsPlayground::getContactSurface();
	float wallRotationDeg = ECS::GetComponent<PhysicsBody>(wallImTouching).GetRotationAngleDeg();
	int myself = PhysicsPlayground::getActiveProj();


	m_currScene->PhysicsPlayground::orangePortal(ECS::GetComponent<PhysicsBody>(myself).GetPosition().x, ECS::GetComponent<PhysicsBody>(myself).GetPosition().y, wallRotationDeg);









}
void oPortalSpawnTrigger::OnExit() // literally do nothing
{
	Trigger::OnExit();
}

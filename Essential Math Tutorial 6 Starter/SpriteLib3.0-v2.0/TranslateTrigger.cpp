#include "TranslateTrigger.h"
#include "ECS.h"


void TranslateTrigger::OnTrigger()
{
	
	Trigger::OnTrigger();

	
}

void TranslateTrigger::OnEnter()
{

	Trigger::OnEnter();
		auto& plat = ECS::GetComponent<PhysicsBody>(toMove);
		if (dir == 0)//right
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(speed, 0.f), true);
		else if (dir == 1)//left
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-speed, 0.f), true);
		else if (dir == 2)//up
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, speed), true);
		else if (dir == 3)//down
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -speed), true);
		

}

void TranslateTrigger::OnExit()
{
	Trigger::OnExit();
}

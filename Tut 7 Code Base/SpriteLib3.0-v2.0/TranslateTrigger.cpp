#include "TranslateTrigger.h"
#include "ECS.h"

void TranslateTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void TranslateTrigger::OnEnter()
{
	Trigger::OnEnter();
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		//ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).GetPosition() + movement, true);
		////ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(1000, 1000), true);
		//ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).ScaleBody(1.2f, 0, true);

		//MOVES PLATFORM
		auto& plat = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]);
		if (dir == 0)//right
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(speed, 0.f), true);
		else if (dir == 1)//left
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-speed, 0.f), true);
		else if (dir == 2)//up
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, speed), true);
		else if (dir == 3)//down
			plat.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -speed), true);

	}
}

void TranslateTrigger::OnExit()
{
	Trigger::OnExit();
}

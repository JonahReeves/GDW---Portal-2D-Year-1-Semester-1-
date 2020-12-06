#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations JSON file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\

	//Idle Left
	m_animController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["IdleRight"].get<Animation>());

	//WALK ANIMATIONS\\

	//Walk Left
	m_animController->AddAnimation(animations["WalkLeft"].get<Animation>());
	//Walk Right
	m_animController->AddAnimation(animations["WalkRight"].get<Animation>());

	//RUN ANIMATIONS\\

	//Run Left
	m_animController->AddAnimation(animations["RunLeft"].get<Animation>());
	//Run Right
	m_animController->AddAnimation(animations["RunRight"].get<Animation>());

	//ATTACK ANIMATIONS\\

	//Attack Left
	m_animController->AddAnimation(animations["JumpLeft"].get<Animation>());
	//Attack Right
	m_animController->AddAnimation(animations["JumpRight"].get<Animation>());

	//Set default animation
	m_animController->SetActiveAnim(IDLELEFT);

}

void Player::Update()
{
	MovementUpdate();
	AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;
	m_attacking = false;
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	if (Input::GetKey(Key::A))
	{
		m_facing = LEFT;
	}
	if (Input::GetKey(Key::D))
	{
		m_facing = RIGHT;
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;
	float tempVel = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetLinearVelocity().x;

	if (!(ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).m_canJump))
	{
		//Puts it into ATTACK category
		activeAnimation = ATTACK;
	}
	else if (!(tempVel == float32()))
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}
	else
	{
		//Puts it into IDLE category
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}

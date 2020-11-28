#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "TranslateTrigger.h"
#include <string>
#include <stdlib.h>

#include <random>

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

int PhysicsPlayground::staticBasicPlat(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg)
{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = file;
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fileLength, fileWidth);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, layerVal));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(xVal), float32(yVal));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
		return entity;
	
}

int PhysicsPlayground::dynamicPlat(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg)
{
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up the components
	std::string fileName = file;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fileLength, fileWidth);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, layerVal));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);


	tempPhsBody = PhysicsBody(entity, tempBody,  float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	tempPhsBody.SetGravityScale(0.f);
	return entity;
}

int PhysicsPlayground::dynamicBall(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg) //Ball
{
	auto entity = ECS::CreateEntity();
	//ball = entity;
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up the components
	std::string fileName = file;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fileLength, fileWidth);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, layerVal));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
	tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	return entity;
}

int PhysicsPlayground::basicDestroTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int target, float rotationAngleDeg)//Setup trigger
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	std::string fileName = file;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fileLength, fileWidth);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, layerVal));
	ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(target);
	

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;
}

int PhysicsPlayground::basicTranslateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int direction, int target, int speed, float rotationAngleDeg)//Setup trigger
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	std::string fileName = file;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fileLength, fileWidth);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, layerVal));
	ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger(direction, target, speed);
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	


	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);


	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;
}
int PhysicsPlayground::boulder(int fileLength, int fileWidth, float xVal, float yVal, int rotation)

	{ //BOULDER
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "Boulder.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fileLength, fileWidth);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 20.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(xVal), float32(yVal));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 4.f),b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2, -2 * tempSpr.GetHeight() / 3), b2Vec2(tempSpr.GetWidth() / 2, tempSpr.GetHeight() / 2) };
		tempPhsBody = PhysicsBody(entity, BodyType::BOULDER, tempBody, points, vec2(0.f, 0.f), false, OBJECTS, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 0.3f);
		tempPhsBody.SetGravityScale(3.f);
		tempPhsBody.SetRotationAngleDeg(rotation);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		return entity;

	}





void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}
	
	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);

		//Sets up the components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f)); //Level 1
		//tempDef.position.Set(float32(360.f), float32(30.f)); //Level 2
		//tempDef.position.Set(float32(542.5f), float32(85.f)); //Level 3
		//tempDef.position.Set(float32(765.f), float32(-30.f)); 
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		 tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0.f, tempSpr.GetHeight() / 2.f) };
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 0.3f);


		//tempPhsBody.SetRotationAngleDeg(0.f);
		//tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}
	
	// LEVEL 1--------------------------------------------------------------------------------------------
	{
		int startingPlat = staticBasicPlat("boxSprite.jpg", 225, 10, 30, -10, 2);
		int movingPlat = dynamicPlat("boxSprite.jpg", 150, 10, 30, -20, 2);
		int holderPlat = staticBasicPlat("boxSprite.jpg", 225, 10, 30, -30.5f, 2);
		int movingPlatStopper = staticBasicPlat("boxSprite.jpg", 10, 70, 280, -10, 2);
		//int testDestroTrigger = basicDestroTrigger("boxSprite.jpg", 50, 50, 50, 10, 3, movingPlat);//Setup trigger
		int rightTranslateTrigger = basicTranslateTrigger("boxSprite.jpg", 20, 10, 40, 0, 3, 1, movingPlat, 25000);
		int leftTranslateTrigger = basicTranslateTrigger("boxSprite.jpg", 20, 10, 80, 0, 3, 0, movingPlat, 25000);
	}
		//(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int direction, int target, int speed = 100000, float rotationAngleDeg = 0.0);
	// LEVEL 2--------------------------------------------------------------------------------------------
	{
		int startingPlat = staticBasicPlat("boxSprite.jpg", 175, 10, 370, 20, 2);
		int boulderHolderPlat = staticBasicPlat("boxSprite.jpg", 100, 5, 355, 145, 2, -45);
		int boulderHolderCap = staticBasicPlat("boxSprite.jpg", 75, 5, 413, 147, 2, 60);
		
		srand(time(NULL));
		for (int i = 0; i < 7; i++)
			boulder(rand() % 25 + 10, rand() % 25 + 10, 370, 160, rand() % 360);

		
		int ball = dynamicBall("Beachball.png", 10, 10, 370, 25, 2);
		
		int rollerPlat = staticBasicPlat("boxSprite.jpg", 175, 2, 532.5, -19.5, 2, -30);
		
		
		
		int rollerPlat2 = staticBasicPlat("boxSprite.jpg", 175, 2, 532, -2, 2, -30);

		
	
		int wall = staticBasicPlat("boxSprite.jpg", 2, 17, 550, -20, 2);
		int ballTrigger = basicDestroTrigger("boxSprite.jpg", 15, 15, 544, -19, 3, boulderHolderCap, -30);
			
		

		

	}
	// LEVEL 3------------------------------------------------------
	{
		int startingPlat = staticBasicPlat("boxSprite.jpg", 175, 5, 542.5, 75, 2);
		int wall = staticBasicPlat("boxSprite.jpg", 2, 34, 456, 59, 2);

		staticBasicPlat("boxSprite.jpg", 100, 5, 680, 100, 2, -37.5f);
		staticBasicPlat("boxSprite.jpg", 100, 5, 680, 41.5, 2, 37.5f);
		staticBasicPlat("boxSprite.jpg", 100, 5, 580, -5, 2, -37.5f);
		staticBasicPlat("boxSprite.jpg", 50, 5, 540, 51, 2, 90.f);
		staticBasicPlat("boxSprite.jpg", 100, 5, 670, -35, 2);

		spinner = staticBasicPlat("boxSprite.jpg", 130, 5, 630, 75, 2, 90);
	}
	// LEVEL 4-----------------------------------------------------------
	{

		int startingPlat = staticBasicPlat("boxSprite.jpg", 500, 5, 950, -35, 2);

		{//SEESAW


			auto entity = ECS::CreateEntity();
			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up the components
			std::string fileName = "Seesaw.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 160, 40);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 20.f, 3.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(850), float32(-10));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth()/2, tempSpr.GetHeight()/2), b2Vec2(-tempSpr.GetWidth() / 8, -tempSpr.GetHeight() / 2),   b2Vec2(7 * (tempSpr.GetWidth() / 8), tempSpr.GetHeight() / 4),  b2Vec2(tempSpr.GetWidth() / 2, tempSpr.GetHeight() / 2)};
			tempPhsBody = PhysicsBody(entity, BodyType::SEESAW, tempBody, points, vec2(0.f, 0.f), false, OBJECTS, GROUND | ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 0.3f);
			tempPhsBody.SetGravityScale(1.f);
			tempPhsBody.SetRotationAngleDeg(0);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetMass(1000);
			
		}

		{
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 9, 9);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(900, -25, 9));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(900), float32(-25));

		tempBody = m_physicsWorld->CreateBody(&tempDef);


		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER | OBJECTS, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
		tempPhsBody.SetMass(500);
		expander1 = entity;
		}
		int finalPlat = staticBasicPlat("boxSprite.jpg", 100, 5, 950, 35, 2);

		
	}
	
	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	
}


void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	

	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		auto& expander = ECS::GetComponent<PhysicsBody>(expander1);
		expander.ScaleBody(5 * Timer::deltaTime, 0);
		
	}
	else if (Input::GetKey(Key::I))
	{
		
		auto& expander = ECS::GetComponent<PhysicsBody>(expander1);
		expander.ScaleBody(-5 * Timer::deltaTime, 0);
	}

	if (Input::GetKey(Key::R)) //SPINNNNNNNNNNNNNN
	{

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(spinner);
		
		tempPhsBody.SetRotationAngleDeg(tempPhsBody.GetRotationAngleDeg() - 2.5f);
		
		
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 160000.f), true);
			canJump.m_canJump = false;
		}
	}
}

void PhysicsPlayground::KeyboardUp()
{
	

}

#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "TranslateTrigger.h"
#include "PortalTrigger.h"
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

static unsigned int playerid;


int movingPlat;
int holderPlat;
int movingPlatStopper;
int rightTranslateTrigger;
int leftTranslateTrigger;
int finalPlat;

static unsigned int square1id;
static unsigned int portalgunid;

bool squarepickup;
bool portalgunmove;
bool pgangleup;
bool pgangledown;


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

int PhysicsPlayground::dynamicPlat(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg, float gravityScale)
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


	tempPhsBody = PhysicsBody(entity, tempBody,  float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, GROUND | ENVIRONMENT | PLAYER | TRIGGER | OBJECTS, 0.3f);

	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	tempPhsBody.SetGravityScale(gravityScale);
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


	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, OBJECTS, ENVIRONMENT | OBJECTS, ENEMY);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;
}

int PhysicsPlayground::basicPortal(float xVal, float yVal, float rotationAngleDeg)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	std::string fileName = "BluePortal.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 40);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, 10));
	ECS::GetComponent<Trigger*>(entity) = new PortalTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(playerid);



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

static unsigned int square(b2World* m_physicsWorld, float shapex, float shapey, float shapew, float shapeh, int& puzzleWall)
{
	{
	//Square
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up the components
	std::string fileName = "BoxBlock.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, shapew, shapeh);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(shapex), float32(shapey));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	return entity;
	}
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
		playerid = entity;

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
		tempDef.position.Set(float32(-30.f), float32(0.f)); //3100 -320

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 2.f), b2Vec2(0.f, tempSpr.GetHeight() / 2.f) };
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//Portal Gun
	{
		float playerx, playery;
		playerx = ECS::GetComponent<Transform>(playerid).GetPositionX();
		playery = ECS::GetComponent<Transform>(playerid).GetPositionY();
		auto entity = ECS::CreateEntity();
		portalgunid = entity;
		ECS::SetPortalGun(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "portalgun.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(playerx), float32(playery));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, ENVIRONMENT, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetFixedRotation(false);
		tempPhsBody.SetRotationAngleDeg(0.f);
	}
	
	// TESTING PLATFORMS--------------------------------------------------------------------------------------------
	{
		square1id = square(m_physicsWorld, 120.f, 0.f, 20, 20, puzzleWall1);

		int startingPlat = staticBasicPlat("boxSprite.jpg", 225, 10, 30, -10, 2);
		int testPortal = basicPortal(70, 20);
		/*{
			staticBasicPlat("boxSprite.jpg", 150, 10, 80, 100, 2); //platform above moving door
			int movingPlat = dynamicPlat("boxSprite.jpg", 10, 50, 120, 40, 2, 0.0f, 0.01f);
			staticBasicPlat("boxSprite.jpg", 10, 50, 109, 60, 2);
			staticBasicPlat("boxSprite.jpg", 10, 50, 131, 60, 2);
			int upTranslateTrigger = basicTranslateTrigger("boxSprite.jpg", 20, 10, 40, 0, 3, 2, movingPlat, 25000);
		}*/
		
	}
	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& portalgun = ECS::GetComponent<PhysicsBody>(MainEntities::PortalGun());

	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	auto& square1 = ECS::GetComponent<PhysicsBody>(square1id);

	float playerx, playery;
	playerx = ECS::GetComponent<Transform>(playerid).GetPositionX();
	playery = ECS::GetComponent<Transform>(playerid).GetPositionY();

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}
	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
		portalgunmove = true;
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
		portalgunmove = true;
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
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	float playerx, playery, squarepositionx, squarepositiony;
	playerx = ECS::GetComponent<Transform>(playerid).GetPositionX();
	playery = ECS::GetComponent<Transform>(playerid).GetPositionY();
	squarepositionx = ECS::GetComponent<Transform>(square1id).GetPositionX();
	squarepositiony = ECS::GetComponent<Transform>(square1id).GetPositionY();

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

	if (Input::GetKeyDown(Key::E) && playerx > (squarepositionx - 30) && playerx < (squarepositionx + 30) && playery > (squarepositiony - 30) && playery < (squarepositiony + 30)) //only picks up the square if its within 30 units of it
	{
		squarepickup = true;
	}

	if (Input::GetKeyDown(Key::UpArrow))
	{
		pgangleup = true;
	}

	if (Input::GetKeyDown(Key::DownArrow))
	{
		pgangledown = true;
	}

}

void PhysicsPlayground::KeyboardUp()
{
	if (Input::GetKeyUp(Key::E))
	{
		squarepickup = false;
	}

	if (Input::GetKeyUp(Key::UpArrow))
	{
		pgangleup = false;
		pgangledown = false;
	}

	if (Input::GetKeyUp(Key::DownArrow))
	{
		pgangleup = false;
		pgangledown = false;
	}
}

void PhysicsPlayground::Update()
{
	using namespace std;

	auto& square1 = ECS::GetComponent<PhysicsBody>(square1id);
	auto& portalgun = ECS::GetComponent<PhysicsBody>(portalgunid);
	auto& player = ECS::GetComponent<PhysicsBody>(playerid);

	float playerx, playery, square1x, square1y, portalgunangle;
	playerx = ECS::GetComponent<Transform>(playerid).GetPositionX();
	playery = ECS::GetComponent<Transform>(playerid).GetPositionY();
	square1x = ECS::GetComponent<Transform>(square1id).GetPositionX();
	square1y = ECS::GetComponent<Transform>(square1id).GetPositionY();

	portalgunangle = portalgun.GetRotationAngleDeg();

	if (squarepickup) {
		//square1.SetPosition(b2Vec2(playerx + 30, playery));
	}
	//portalgun.SetPosition(b2Vec2(playerx + 10, playery)); //update portal gun position

	if (pgangleup) {
		portalgun.SetRotationAngleDeg(portalgunangle + 2.f);
	}
	else if (pgangledown) {
		portalgun.SetRotationAngleDeg(portalgunangle - 2.f);
	}
	else if (!pgangleup && !pgangledown) {
		portalgun.SetRotationAngleDeg(portalgunangle);
	}
}
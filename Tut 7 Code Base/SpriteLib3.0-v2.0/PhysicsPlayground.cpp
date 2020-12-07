#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "bPortalTrigger.h"
#include "oPortalTrigger.h"
#include "FirePortal.h"

#include <random>

int playerId;
int bPortal;
int oPortal;
int activeProj;
b2Vec2 activeProjDir;

static unsigned int square1id;
static unsigned int square2id;
static unsigned int square3id;
static unsigned int portalgunid;

bool squarepickup;
bool portalgunmove;
float portalGunAngle = 0;
bool pgangleup;
bool pgangledown;

int level1x = 0;
int level1y = 0;
int level2x = 0;
int level2y = 0;
int level3x = 0;
int level3y = 0;

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}
int PhysicsPlayground::kinematicPlat(int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg, std::string file)
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
	tempDef.type = b2_kinematicBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PROJECTILE | PLAYER | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;

}

int PhysicsPlayground::NonPortPlat(int fileLength, int fileWidth, float xVal, float yVal, float layerVal, float rotationAngleDeg, std::string file)
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
	tempDef.type = b2_kinematicBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PROJECTILE | PLAYER | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;

}

int PhysicsPlayground::getBluePortal()
{
	return bPortal;
}
int PhysicsPlayground::getOrangePortal()
{
	return oPortal;
}

void PhysicsPlayground::portalPhysics(int target, int entPortal, int exitPortal)
{
	//get bodies
	auto& targetPhsBody = ECS::GetComponent<PhysicsBody>(target);
	auto& entPhysBody = ECS::GetComponent<PhysicsBody>(entPortal);
	auto& exitPhysBody = ECS::GetComponent<PhysicsBody>(exitPortal);

	//get angles
	float entrancePortalAngle = entPhysBody.GetRotationAngleDeg();
	float exitPortalAngle = exitPhysBody.GetRotationAngleDeg() + 180;
	float rotationAngleDeg = exitPortalAngle - entrancePortalAngle;
	float theta = (rotationAngleDeg * PI / 180); //convert to rad


	//vector math
	vec3 vec = targetPhsBody.GetVelocity();

	float rotatedX = (cos(theta) * vec.x) - (sin(theta) * vec.y);
	float rotatedY = (sin(theta) * vec.x) + (cos(theta) * vec.y);
	vec3 rotatedVec = vec3(rotatedX, rotatedY, vec.z);

	targetPhsBody.SetVelocity(rotatedVec);
}

int PhysicsPlayground::translateTrigger(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float movementX, float movementY, int target, int speed, float rotationAngleDeg)
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
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, 80.f));
	ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(target);
	TranslateTrigger* temp = (TranslateTrigger*)ECS::GetComponent<Trigger*>(entity);
	temp->movement = b2Vec2(movementX, movementY);

	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(puzzleWall2);

	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;
}

int PhysicsPlayground::translateTriggerDoors(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float movementX, float movementY, int target, int speed, int direction, float rotationAngleDeg)
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
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, 80.f));
	ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger(direction, speed);
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(target);
	TranslateTrigger* temp = (TranslateTrigger*)ECS::GetComponent<Trigger*>(entity);
	temp->movement = b2Vec2(movementX, movementY);

	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(puzzleWall2);

	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);
	return entity;
}

void PhysicsPlayground::bluePortal(float xVal, float yVal, float rotationAngleDeg)
{
	if (bPortal != NULL) //Destroy previous portal
	{
		PhysicsBody::m_bodiesToDelete.push_back(bPortal);
	}
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
	ECS::GetComponent<Trigger*>(entity) = new bPortalTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(playerId);



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


	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

	bPortal = entity;
}
void PhysicsPlayground::orangePortal(float xVal, float yVal, float rotationAngleDeg)
{
	if (oPortal != NULL) //Destroy previous portal
	{
		PhysicsBody::m_bodiesToDelete.push_back(oPortal);
	}
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);
	

	//Sets up components
	std::string fileName = "OrangePortal.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 40);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, 10));
	ECS::GetComponent<Trigger*>(entity) = new oPortalTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(playerId);



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


	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

	oPortal = entity;
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


	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PROJECTILE | GROUND | ENVIRONMENT | PLAYER | TRIGGER | OBJECTS, 0.3f);

	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	tempPhsBody.SetGravityScale(gravityScale);
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

int PhysicsPlayground::portalProj(bool portalColor, float xVal, float yVal, float directionAngle)
{
	if (activeProj != NULL) //Destroy any other portalproj that could be mid travel
	{
		PhysicsBody::m_bodiesToDelete.push_back(activeProj);
	}
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);
	

	//Sets up the components
	std::string fileName = "BeachBall.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3, 3);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, 10));

	if (portalColor) //true = blue, false = orange
		ECS::GetComponent<Trigger*>(entity) = new bPortalSpawnTrigger();
	else
		ECS::GetComponent<Trigger*>(entity) = new oPortalSpawnTrigger();





	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);


	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PROJECTILE, GROUND | ENVIRONMENT, 0.3f);
	

	if (portalColor) //can remove later if need be
		tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));
	else
		tempPhsBody.SetColor(vec4(1.f, 0.65f, 0.f, 0.3f));

	tempPhsBody.SetRotationAngleDeg(directionAngle);
	tempPhsBody.SetGravityScale(0.f);

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

	activeProj = entity;
	float projSpeedMult = 10;
	activeProjDir = b2Vec2(cos(directionAngle * PI / 180) * projSpeedMult, sin(directionAngle * PI / 180) * projSpeedMult);

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

	//EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	//EffectManager::CreateEffect(EffectType::Sepia, windowWidth, windowHeight);
	

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
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
		
		playerId = entity;//assign link to playerId
	}

	//Basic level
	{
		//int traslator = translateTrigger("boxSprite.jpg", 20, 20, 30, 10, 10, 0, startingPlat);
		//bluePortal(30, 20, 0);
		//orangePortal(100, 50, 270);
		
	
	}
 
		

		//Portal Gun
	{
		float playerx, playery;
		playerx = ECS::GetComponent<Transform>(playerId).GetPositionX();
		playery = ECS::GetComponent<Transform>(playerId).GetPositionY();
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

	
	 //Level 1
	{
		square1id = square(m_physicsWorld, -50.f + level1x, 0.f + level1y, 20, 20, puzzleWall1);
		int startingPlat = NonPortPlat(10, 300, 80 + level1x, -10 + level1y, 2, 90.f); //floor
		NonPortPlat(10, 300, 80 + level1x, 100 + level1y, 2, 270.f); //platform above moving door //roof
		kinematicPlat(10, 110, -75 + level1x, 40 + level1y, 2, 180.f);
		int movingPlat = dynamicPlat("boxSprite.jpg", 10, 50, 200 + level1x, 40 + level1y, 2, 0.0f, 0.01f); //door
		int upTranslateTrigger = translateTriggerDoors("boxSprite.jpg", 20, 10, -45 + level1x, 70 + level1y, 3, 2, movingPlat, 25000, 2, 0); //button
		kinematicPlat(10, 60, 194 + level1x, 65 + level1y, 2, 0); //staticplat platforms next to door
		kinematicPlat(10, 60, 216 + level1x, 65 + level1y, 2, 180); //staticplat platforms next to door
		kinematicPlat(10, 50, 164 + level1x, 50 + level1y, 2, 90.f); //platform on left of door
		kinematicPlat(10, 50, -45 + level1x, 50 + level1y, 2, 90.f); //platform above start
		kinematicPlat(23, 30, 85 + level1x, 5 + level1y, 2, 90.f); //box in middle of room
		//TEMPORARY PORTALS
		bluePortal(190, 75, 0);
		orangePortal(-70, 75, 180);
	}
	//Level 2
	{
		square2id = square(m_physicsWorld, 280.f + level1x, 0.f + level1y, 20, 20, puzzleWall3);
		int startingPlat2 = NonPortPlat(10, 200, 330 + level2x, -10 + level2y, 2, 90.f); //floor
		NonPortPlat(10, 300, 300 + level1x, 190 + level1y, 2, 270.f); //roof
		kinematicPlat(10, 35, 425 + level1x, 100 + level1y, 2, 270.f); //top of door
		kinematicPlat(10, 90, 145 + level1x, 145 + level1y, 2, 180.f); //connects to roof left
		kinematicPlat(10, 60, 180 + level1x, 160 + level1y, 2, 180.f); //connects to roof right (2)
		int movingPlat2 = dynamicPlat("boxSprite.jpg", 10, 50, 420 + level2x, 40 + level2y, 2, 0.0f, 0.01f); //door
		int upTranslateTrigger2 = translateTriggerDoors("boxSprite.jpg", 20, 10, 155 + level2x, 110 + level2y, 3, 2, movingPlat2, 25000, 2, 0); //button FIX
		kinematicPlat(10, 60, 414 + level2x, 65 + level2y, 2, 0); //staticplat platforms next to door
		kinematicPlat(10, 60, 436 + level2x, 65 + level2y, 2, 0); //staticplat platforms next to door
		kinematicPlat(10, 50, 384 + level2x, 50 + level2y, 2, 90.f); //platform on left of door
		kinematicPlat(10, 80, 415 + level2x, 145 + level2y, 2, 0.f); // platform above right door
		kinematicPlat(10, 50, 250 + level2x, 100 + level2y, 2, 90.f); //above start
		//kinematicPlat(10, 50, 450 + level2x, 100 + level2y, 2, 90.f); //mirrors above start
		//kinematicPlat(23, 30, 300 + level1x, 5 + level1y, 2, 90.f); //box in middle of room delete later
	}
	//Level 3
	{
		square3id = square(m_physicsWorld, 400.f + level3x, 0.f + level3y, 20, 20, puzzleWall4);
		int startingPlat3 = kinematicPlat(10, 100, 480 + level3x, -10 + level3y, 2, 90.f); //FLOOR
		kinematicPlat(10, 100, 580 + level3x, -10 + level3y, 2, 90.f); //FLOOR
		kinematicPlat(10, 100, 680 + level3x, -10 + level3y, 2, 90.f); //FLOOR
		kinematicPlat(10, 100, 630 + level3x, 10 + level3y, 2, 0.f); //wall on right
		kinematicPlat(10, 70, 470 + level3x, 50 + level3y, 2, 90.f); //the thing up start
		kinematicPlat(10, 70, 470 + level3x, 50 + level3y, 2, 90.f); //the thing up start 2
		kinematicPlat(10, 100, 432 + level3x, 145 + level3y, 2, 180); //thing 3
		kinematicPlat(10, 100, 446 + level3x, 85 + level3y, 2, 180); //thing 3 2
		kinematicPlat(10, 60, 620 + level3x, 125 + level3y, 2, 0); //staticplat platforms next to door r
		kinematicPlat(10, 60, 642 + level3x, 125 + level3y, 2, 0); //staticplat platforms next to door r
		int movingPlat3 = dynamicPlat("boxSprite.jpg", 10, 50, 630 + level3x, 100 + level3y, 2, 0.0f, 0.01f); //door
		int upTranslateTrigger3 = translateTriggerDoors("boxSprite.jpg", 20, 10, 480 + level2x, 80 + level2y, 3, 2, movingPlat3, 25000, 2, 0); //button FIX
		kinematicPlat(10, 100, 500 + level3x, 120 + level3y, 2, 270.f); //roof
		kinematicPlat(10, 70, 580 + level3x, 120 + level3y, 2, 270.f); //roof
		kinematicPlat(10, 20, 625 + level2x, 145 + level2y, 2, 270.f); // platform above right door
		kinematicPlat(10, 100, 710 + level3x, 80 + level3y, 2, 0.f); // wall past door
		//kinematicPlat(10, 100, 980 + level3x, -10 + level3y, 2, 0.f, "boxSprite.jpg"); //FLOOR
		//kinematicPlat(10, 80, 700 + level3x, 145 + level3y, 2, 0.f); // platform above first door
	}

	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}
}

void PhysicsPlayground::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}
	
	ImGui::End();
}

void PhysicsPlayground::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();
	
	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();
	
	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void PhysicsPlayground::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0,1), ImVec2(1,0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();

	ImGui::End();
}




void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& portalgun = ECS::GetComponent<PhysicsBody>(MainEntities::PortalGun());

	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	auto& square1 = ECS::GetComponent<PhysicsBody>(square1id);
	auto& square2 = ECS::GetComponent<PhysicsBody>(square2id);

	float playerx, playery;
	playerx = ECS::GetComponent<Transform>(playerId).GetPositionX();
	playery = ECS::GetComponent<Transform>(playerId).GetPositionY();

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

	/*Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}
	*/
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	float playerx, playery, squarepositionx, squarepositiony;
	playerx = ECS::GetComponent<Transform>(playerId).GetPositionX();
	playery = ECS::GetComponent<Transform>(playerId).GetPositionY();
	squarepositionx = 0;
	squarepositiony = 0;
	if (playerx < (216 + level1x) && playery < 110) {
		squarepositionx = ECS::GetComponent<Transform>(square1id).GetPositionX();
		squarepositiony = ECS::GetComponent<Transform>(square1id).GetPositionY();
	}
	else if (playerx > (216 + level1x) && playerx < (422 + level2x) || playery > 110) {
		squarepositionx = ECS::GetComponent<Transform>(square2id).GetPositionX();
		squarepositiony = ECS::GetComponent<Transform>(square2id).GetPositionY();
	}

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

	if (Input::GetKeyDown(Key::E) && playerx > (squarepositionx - 30) && playerx < (squarepositionx + 30) && playery >(squarepositiony - 30) && playery < (squarepositiony + 30))
	{
		squarepickup = true;
	}



	//angle portal guns
	if (Input::GetKey(Key::UpArrow) && Input::GetKey(Key::LeftArrow))
	{
		portalGunAngle = 135;
	}
	else if (Input::GetKey(Key::UpArrow) && Input::GetKey(Key::RightArrow))
	{
		portalGunAngle = 45;
	}
	else if (Input::GetKey(Key::DownArrow) && Input::GetKey(Key::LeftArrow))
	{
		portalGunAngle = 225;
	}
	else if (Input::GetKey(Key::DownArrow) && Input::GetKey(Key::RightArrow))
	{
		portalGunAngle = 315;
	}
	else if (Input::GetKey(Key::UpArrow))
	{
		portalGunAngle = 90;
	}
	else if (Input::GetKey(Key::DownArrow))
	{
		portalGunAngle = 270;
	}
	else if (Input::GetKey(Key::LeftArrow))
	{
		portalGunAngle = 180;
	}
	else if (Input::GetKey(Key::RightArrow))
	{
		portalGunAngle = 0;
	}
	else
	{
		portalGunAngle = 0;
	}

	

	//spawn portals
	if (Input::GetKeyDown(Key::K)) //Blue 
	{
		portalProj(true, playerx, playery, portalGunAngle);
		//bluePortal(playerx +20, playery, 0);
	}
	if (Input::GetKeyDown(Key::L)) //Orange
	{
		portalProj(false, playerx, playery, portalGunAngle);
		//orangePortal(playerx - 20, playery, 0);
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
	auto& square2 = ECS::GetComponent<PhysicsBody>(square2id);
	auto& portalgun = ECS::GetComponent<PhysicsBody>(portalgunid);
	auto& player = ECS::GetComponent<PhysicsBody>(playerId);

	float playerx, playery, square1x, square1y, square2x, square2y;
	playerx = ECS::GetComponent<Transform>(playerId).GetPositionX();
	playery = ECS::GetComponent<Transform>(playerId).GetPositionY();
	square1x = ECS::GetComponent<Transform>(square1id).GetPositionX();
	square1y = ECS::GetComponent<Transform>(square1id).GetPositionY();
	square2x = ECS::GetComponent<Transform>(square2id).GetPositionX();
	square2y = ECS::GetComponent<Transform>(square2id).GetPositionY();

	//if portal projectile exists, update it
	if (activeProj != NULL)
	{
		ECS::GetComponent<PhysicsBody>(activeProj).SetPosition(ECS::GetComponent<PhysicsBody>(activeProj).GetPosition() + (activeProjDir));
		
	}



	if (squarepickup) {
		if (playerx < (216 + level1x)) {
			square1.SetPosition(b2Vec2(playerx + 30, playery));
		}
		else if (playerx > (216 + level1x) && playerx < (422 + level2x)) {
			square2.SetPosition(b2Vec2(playerx + 30, playery));
		}
	}
	portalgun.SetPosition(b2Vec2(playerx + 5, playery));

	

	portalgun.SetRotationAngleDeg(portalGunAngle);
	
}
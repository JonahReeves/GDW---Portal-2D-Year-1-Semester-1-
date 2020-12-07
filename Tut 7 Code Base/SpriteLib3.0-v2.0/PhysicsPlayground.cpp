#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "bPortalTrigger.h"
#include "oPortalTrigger.h"
#include "CheckpointTrigger.h"
#include "deathTrigger.h"



#include "FirePortal.h"

#include <random>

int playerId;
int bPortal;
int oPortal;

b2Vec2 activeProjDir;
b2Vec2 respawnPoint = b2Vec2(0.f, 30.f);
int portalSurface[200]; //MAX 200 WALLS IN THE GAME
int portalSurfaceSize = 0;
int activeProj;

int loopLevelx = 1090;
int loopLevely = 115;

int momentumLevelx;
int momentumLevely;

static unsigned int square1id;
static unsigned int square2id;
static unsigned int square3id;
static unsigned int square4id;
static unsigned int square5id;
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
int level4x = 720;
int level4y = 0;
int CompCubex = 1340;
int CompCubey = 235;
int levelFPx = 2230;
int levelFPy = 230;


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
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, TRIGGER | PROJECTILE | PLAYER | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);

	PhysicsPlayground::addToPortalSurface(entity);

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
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, TRIGGER | PLAYER | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotationAngleDeg);

	PhysicsPlayground::addToPortalSurface(entity);

	return entity;

}


bool PhysicsPlayground::bPortalExists()
{
	if (bPortal != NULL)
		return true;
	return false;
}
bool PhysicsPlayground::oPortalExists()
{
	if (oPortal != NULL)
		return true;
	return false;
}
int PhysicsPlayground::getActiveProj()
{
	return activeProj;
}

void PhysicsPlayground::respawn()
{
	ECS::GetComponent<PhysicsBody>(playerId).SetPosition(respawnPoint, true);
}

void PhysicsPlayground::setRespawn(b2Vec2 newSpawn)
{
	respawnPoint = newSpawn;
}

int PhysicsPlayground::makeCheckpoint(std::string file, int fileLength, int fileWidth, float xVal, float yVal)
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


	ECS::GetComponent<Trigger*>(entity) = new CheckpointTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	((CheckpointTrigger*)ECS::GetComponent<Trigger*>(entity))->SetScene(this);
	((CheckpointTrigger*)ECS::GetComponent<Trigger*>(entity))->setLoc(b2Vec2(xVal, yVal + 20)); //respawn location is set to where the checkpoint is
	
	

	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(fileLength), float(fileWidth), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

	return entity;
}

int PhysicsPlayground::makeDeathObject(std::string file, int fileLength, int fileWidth, float xVal, float yVal)
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


	ECS::GetComponent<Trigger*>(entity) = new deathTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	((deathTrigger*)ECS::GetComponent<Trigger*>(entity))->SetScene(this);
	



	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(fileLength), float(fileWidth), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

	return entity;
}

int PhysicsPlayground::getContactSurface()
{
	/*
	when projectile hits a wall,
	scans this array to see what wall it hit by comparing location values,
	then takes the angle of that wall and creates a portal of the correct color at that location with the same rotation angle as the wall it hit
	*/
	
	float currentWallx;
	float currentWally;
	float currentDistance;
	float projx = ECS::GetComponent<PhysicsBody>(activeProj).GetPosition().x;
	float projy = ECS::GetComponent<PhysicsBody>(activeProj).GetPosition().y;

	float closestDistance = 9999.f;
	int closestWall;
	for (int i = 0; i < portalSurfaceSize; i++)
	{
		currentWallx = ECS::GetComponent<PhysicsBody>(portalSurface[i]).GetPosition().x;
		currentWally = ECS::GetComponent<PhysicsBody>(portalSurface[i]).GetPosition().y;

		currentDistance = sqrt(((projx - currentWallx) * (projx - currentWallx)) + ((projy - currentWally) * (projy - currentWally)));
		if (currentDistance < closestDistance)
		{
			closestWall = portalSurface[i];
			closestDistance = currentDistance;
		}
	}
	return closestWall;
}

void PhysicsPlayground::addToPortalSurface(int ent)
{
	portalSurface[portalSurfaceSize] = ent;
	portalSurfaceSize++;
}

int* PhysicsPlayground::getPortalSurfaces() //gives a reference to the array of all portable surfaces
{
	return portalSurface;
}

int PhysicsPlayground::getBluePortal()
{
	return bPortal;
}
int PhysicsPlayground::getOrangePortal()
{
	return oPortal;
}

void PhysicsPlayground::resetActiveProj()
{
	activeProj = NULL;
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

int PhysicsPlayground::translateDoorsObjOnly(std::string file, int fileLength, int fileWidth, float xVal, float yVal, float movementX, float movementY, int target, int speed, int direction, float rotationAngleDeg)
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

	tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, OBJECTS);
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

static unsigned int square(b2World* m_physicsWorld, float shapex, float shapey, float shapew, float shapeh, int& puzzleWall, std::string file)
{
	{
		//Square
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = file;
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
	if (activeProj != NULL)
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
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 5);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xVal, yVal, 10));



	if (portalColor)
	{
		ECS::GetComponent<Trigger*>(entity) = new bPortalSpawnTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		((bPortalSpawnTrigger*)ECS::GetComponent<Trigger*>(entity))->SetScene(this);
	}
	else
	{
		ECS::GetComponent<Trigger*>(entity) = new oPortalSpawnTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		((oPortalSpawnTrigger*)ECS::GetComponent<Trigger*>(entity))->SetScene(this);
	}
		
	
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xVal), float32(yVal));

	tempBody = m_physicsWorld->CreateBody(&tempDef);


	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), true, PROJECTILE, GROUND, 0.3f);

	if (portalColor) //can remove later if need be
		tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));
	else
		tempPhsBody.SetColor(vec4(1.f, 0.65f, 0.f, 0.3f));

	tempPhsBody.SetRotationAngleDeg(directionAngle);
	tempPhsBody.SetGravityScale(0.f);

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

	activeProj = entity;

	float projSpeedMult = 100000.f;
	activeProjDir = b2Vec2(cos(directionAngle * PI / 180) * projSpeedMult, sin(directionAngle * PI / 180) * projSpeedMult);
	ECS::GetComponent<PhysicsBody>(activeProj).GetBody()->SetLinearVelocity(activeProjDir);

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

		vec4 temp = vec4(-150.f, 150.f, -150.f, 150.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "bg.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 1000);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(000.f, 0.f, 0.f));
	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "bg.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 1000);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1000.f, 0.f, 0.f));
	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "bg.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 1000);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(2000.f, 0.f, 0.f));
	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "bg.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 1000);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(3000.f, 0.f, 0.f));
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
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//sets up the components
		std::string fileName = "spritesheets/Atlas.png";
		std::string animations = "atlasAnimations.json";

		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 30, 30, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(0.f)); //tempDef.position.Set(float32(1950.f), float32(230.f)); end of level

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
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
		//square1id = square(m_physicsWorld, -50.f, 0.f, 20, 20, puzzleWall1);
		//int floor = kinematicPlat(10, 300, 30, -10, 2, 270);
		//int leftWall = kinematicPlat(10, 200, -125, 95, 2, 180);
		//int rightWall = kinematicPlat(10, 200, 170, 95, 2, 0);
		//int roof = kinematicPlat(10, 300, -10, 100, 2, 90);
		//int testCheck1 = makeCheckpoint("boxSprite.jpg", 10, 10, 60, -5);
		//int testCheck2 = makeCheckpoint("boxSprite.jpg", 10, 10, -40, -5);
		//int deathCheck = makeDeathObject("Boulder.png", 10, 10, 90, 10);
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
	
	/* simple door
	{
		

		kinematicPlat(150, 10, 80, 100, 2,0, "boxSprite.jpg"); //platform above moving door //staticplat
		int movingPlat = dynamicPlat("boxSprite.jpg", 10, 50, 120, 40, 2, 0.0f, 0.01f);
		kinematicPlat(10, 50, 109, 60, 2, 0, "boxSprite.jpg"); //staticplat
		kinematicPlat(10, 50, 131, 60, 2, 0, "boxSprite.jpg"); //staticplat
	//	std::string file, int fileLength, int fileWidth, float xVal, float yVal, float layerVal, int direction, int target, int speed, float rotationAngleDeg
		int upTranslateTrigger = translateTriggerDoors("boxSprite.jpg", 20, 10, 40, 0, 3, 2, movingPlat, 25000, 2, 0);
		//int upTranslateTrigger = basicTranslateTrigger("boxSprite.jpg", 20, 10, 40, 0, 3, 2, movingPlat, 25000, 0);
	}
	*/


	//Level 1
	{
		square4id = square(m_physicsWorld, -1000000.f + level1x, 80.f + level1y, 20, 20, puzzleWall1, "cube.jpg");
		int startingPlat = NonPortPlat(10, 300, 80 + level1x, -10 + level1y, 2, 270.f, "NonPortal Wall.png"); //floor
		kinematicPlat(5, 300, 80 + level1x, 97.5 + level1y, 2, 90.f); //platform above moving door //roof
		kinematicPlat(5, 300, 80 + level1x, 102.5 + level1y, 2, 270.f); //platform above moving door //roof
		kinematicPlat(10, 110, -75 + level1x, 40 + level1y, 2, 180.f);
		int movingPlat = dynamicPlat("door.png", 10, 50, 205 + level1x, 20 + level1y, 2, 0.0f, 0.f); //door
		int upTranslateTrigger = translateTriggerDoors("button.png", 30, 25, -45 + level1x, 70 + level1y, 3, 2, movingPlat, 25000, 2, 0); //button
		kinematicPlat(10, 60, 194 + level1x, 65 + level1y, 2, 0); //staticplat platforms next to door
		kinematicPlat(10, 60, 216 + level1x, 65 + level1y, 2, 180); //staticplat platforms next to door
		kinematicPlat(5, 50, 164 + level1x, 50 + level1y, 2, 90.f); //platform on left of door
		kinematicPlat(5, 80, -35 + level1x, 50 + level1y, 2, 90.f); //platform above start
		kinematicPlat(5, 50, 164 + level1x, 55 + level1y, 2, 270.f); //platform on left of door
		kinematicPlat(5, 80, -35 + level1x, 55 + level1y, 2, 270.f); //platform above start
		NonPortPlat(23, 30, 85 + level1x, 5 + level1y, 2, 90.f, "NonPortal Wall.png"); //box in middle of room
	}
	//Level 2
	{
		square2id = square(m_physicsWorld, 250.f + level1x, 115.f + level1y, 20, 20, puzzleWall3, "cube.jpg");
		int startingPlat2 = kinematicPlat(10, 200, 330 + level2x, -10 + level2y, 2, 270.f); //floor
		kinematicPlat(10, 300, 300 + level1x, 190 + level1y, 2, 90.f); //roof
		kinematicPlat(10, 10, 425 + level1x, 100 + level1y, 2, 270.f); //top of door
		kinematicPlat(10, 90, 145 + level1x, 145 + level1y, 2, 180.f); //connects to roof left
		kinematicPlat(10, 60, 180 + level1x, 160 + level1y, 2, 180.f); //connects to roof right (2)
		int movingPlat2 = dynamicPlat("door.png", 10, 50, 425 + level2x, 25 + level2y, 2, 0.0f, 0.0f); //door
		int upTranslateTrigger2 = translateTriggerDoors("Portal_Button.png", 30, 7.5, 165 + level2x, 110 + level2y, 3, 2, movingPlat2, 25000, 2, 0); //button FIX
		kinematicPlat(10, 60, 414 + level2x, 65 + level2y, 2, 0); //staticplat platforms next to door
		kinematicPlat(10, 60, 436 + level2x, 65 + level2y, 2, 0); //staticplat platforms next to door
		NonPortPlat(10, 50, 384 + level2x, 50 + level2y, 2, 90.f, "NonPortal Wall.png"); //platform on left of door
		kinematicPlat(10, 100, 414 + level2x, 145 + level2y, 2, 0.f); // Wall above right door
		kinematicPlat(5, 40, 250 + level2x, 97.5 + level2y, 2, 90.f); //above start
		kinematicPlat(5, 40, 250 + level2x, 102.5 + level2y, 2, 270.f); //above start flipped
	}
	//Level 3
	{
		//square3id = square(m_physicsWorld, 400.f + level3x, 0.f + level3y, 20, 20, puzzleWall4);
		int startingPlat3 = kinematicPlat(10, 100, 480 + level3x, -10 + level3y, 2, 270.f); //FLOOR
		NonPortPlat(10, 100, 580 + level3x, -10 + level3y, 2, 90.f, "NonPortal Wall.png"); //FLOOR
		NonPortPlat(10, 100, 680 + level3x, -10 + level3y, 2, 90.f, "NonPortal Wall.png"); //FLOOR
		kinematicPlat(5, 100, 627.5 + level3x, 10 + level3y, 2, 0.f); //wall on right
		kinematicPlat(5, 100, 632.5 + level3x, 10 + level3y, 2, 180.f); //wall on right flipped
		kinematicPlat(5, 70, 470 + level3x, 52.5 + level3y, 2, 270.f); //the thing up start
		kinematicPlat(5, 70, 470 + level3x, 47.5 + level3y, 2, 90.f); //the thing up start 2
		kinematicPlat(10, 100, 432 + level3x, 145 + level3y, 2, 180); //thing 3
		kinematicPlat(10, 100, 446 + level3x, 85 + level3y, 2, 180); //thing 3 2
		kinematicPlat(10, 60, 620 + level3x, 125 + level3y, 2, 0); //staticplat platforms next to door r
		kinematicPlat(10, 60, 642 + level3x, 125 + level3y, 2, 0); //staticplat platforms next to door r
		int movingPlat3 = dynamicPlat("door.png", 10, 50, 630 + level3x, 85 + level3y, 2, 0.0f, 0.0f); //door
		int upTranslateTrigger3 = translateTriggerDoors("button.png", 30, 25, 480 + level2x, 67 + level2y, 3, 2, movingPlat3, 25000, 2, 0); //button FIX
		kinematicPlat(10, 100, 500 + level3x, 120 + level3y, 2, 90.f); //roof
		kinematicPlat(10, 70, 580 + level3x, 120 + level3y, 2, 90.f); //roof
		kinematicPlat(10, 20, 625 + level2x, 145 + level2y, 2, 90.f); // platform above right door
		kinematicPlat(10, 100, 720 + level3x, 95 + level3y, 2, 0.f); // wall past door

	}
	//Momentum level
	{
		int floor1 = kinematicPlat(10, 80, 40 + level4x, -10 + level4y, 2, 270);
		int pit_wall_L = kinematicPlat(10, 100, 85 + level4x, -55 + level4y, 2, 180);
		int pit_wall_R = kinematicPlat(10, 100, 145 + level4x, -55 + level4y, 2, 0);
		int pit1 = kinematicPlat(10, 60, 115 + level4x, -105 + level4y, 2, 270);
		int momentumCheck = makeCheckpoint("Checkpoint.png", 10, 10, 30 + level4x, 20 + level4y);
		int floor2 = kinematicPlat(10, 50, 165 + level4x, -10 + level4y, 2, 270);
		int wall1 = kinematicPlat(10, 60, 190 + level4x, 25 + level4y, 2, 0);
		int floor_upper1 = kinematicPlat(10, 80, 230 + level4x, 50 + level4y, 2, 270);
		int wall2 = kinematicPlat(10, 60, 270 + level4x, 80 + level4y, 2, 0);
		int floor_upper2 = kinematicPlat(10, 80, 310 + level4x, 105 + level4y, 2, 270);
		int roof1 = NonPortPlat(10, 150, 5 + level4x, 120 + level4y, 2, 180, "NonPortal Wall.png");
		int roof2 = NonPortPlat(10, 350, 175 + level4x, 195 + level4y, 2, 90, "NonPortal Wall.png");
		int roof3 = NonPortPlat(10, 50, 345 + level4x, 170 + level4y, 2, 0, "NonPortal Wall.png");
	}
	//Infinite Portal Loop
	{
		int leftWall = kinematicPlat(10, 150, -18 + loopLevelx, 115 + loopLevely, 2, 180, "Portal Wall.png"); //Left Border Wall
		kinematicPlat(10, 100, -18 + loopLevelx, 240 + loopLevely, 2 , 180, "Portal Wall.png");
		int floor = kinematicPlat(10, 150, 55 + loopLevelx, -10 + loopLevely, 2, 270, "Portal Wall.png");  //First Floor
		int rightWall = kinematicPlat(10, 110, 132 + loopLevelx, 50 + loopLevely, 2, 0, "Portal Wall.png"); //Right Border Wall
		NonPortPlat(10, 150, 202 + loopLevelx, 105 + loopLevely, 2, 90, "NonPortal Wall.png");
		int roof = kinematicPlat(10, 100, 30 + loopLevelx, 285 + loopLevely, 2, 90, "Portal Wall.png");
		NonPortPlat(10, 60, 110 + loopLevelx, 285 + loopLevely, 2, 90, "NonPortal Wall.png");
		NonPortPlat(10, 100, 140 + loopLevelx, 235 + loopLevely, 2, 0, "NonPortal Wall.png");
		NonPortPlat(10, 145, 208 + loopLevelx, 185 + loopLevely, 2, 90, "NonPortal Wall.png");
		NonPortPlat(10, 30, 275 + loopLevelx, 165 + loopLevely, 2, 0, "NonPortal Wall.png");
	}
	//Companion Cube Death currently moving
	{

		NonPortPlat(140, 125, 40 + CompCubex, -80 + CompCubey, 2, 90, "NonPortal Wall.png");//Starting Floor
		NonPortPlat(10, 250, 105 + CompCubex, 65 + CompCubey, 2, 90, "NonPortal Wall.png");//Roof
		NonPortPlat(10, 250, 355 + CompCubex, 65 + CompCubey, 2, 90, "NonPortal Wall.png");//Roof2
		NonPortPlat(10, 250, 605 + CompCubex, 65 + CompCubey, 2, 90, "NonPortal Wall.png");//Roof3
		NonPortPlat(10, 130, 795 + CompCubex, 65 + CompCubey, 2, 90, "NonPortal Wall.png");//Roof3

		NonPortPlat(10, 125, 220 + CompCubex, -15 + CompCubey, 2, 90, "NonPortal Wall.png");//Platform 1
		NonPortPlat(10, 125, 220 + CompCubex, -80 + CompCubey, 2, 0, "NonPortal Wall.png");//Platform 1 Arm

		NonPortPlat(10, 125, 390 + CompCubex, -15 + CompCubey, 2, 90, "NonPortal Wall.png");//Platform 2
		NonPortPlat(10, 125, 390 + CompCubex, -80 + CompCubey, 2, 0, "NonPortal Wall.png");//Platform 2 Arm

		NonPortPlat(10, 125, 560 + CompCubex, -15 + CompCubey, 2, 90, "NonPortal Wall.png");//Platform 3
		NonPortPlat(10, 125, 560 + CompCubex, -80 + CompCubey, 2, 0, "NonPortal Wall.png");//Platform 3 Arm

		NonPortPlat(140, 200, 770 + CompCubex, -80 + CompCubey, 2, 90, "NonPortal Wall.png");//Ending Floor

		makeDeathObject("Boulder.png", 570, 150, 387 + CompCubex, -115 + CompCubey);

		NonPortPlat(10, 30, 855 + CompCubex, 50 + CompCubey, 2, 0, "NonPortal Wall.png");//DoorBlock
		NonPortPlat(10, 30, 875 + CompCubex, 50 + CompCubey, 2, 0, "NonPortal Wall.png");//DoorBlock
		square1id = square(m_physicsWorld, 700.f + CompCubex, 0.f + CompCubey, 20, 20, puzzleWall1, "Companion Cube.png");//Companion Cube
		int CompanionDoor = dynamicPlat("door.png", 10, 80, 865 + CompCubex, 30 + CompCubey, 2, 0.0f, 0.0f); //door
		int CompanionDoorUp = translateDoorsObjOnly("fire.png", 90, 45, 800 + CompCubex, 5 + CompCubey, 1, 2, CompanionDoor, 25000, 2, 0); //Incinerator
		int comp_cubeCheck = makeCheckpoint("Checkpoint.png", 10, 10, 30 + CompCubex, 20 + CompCubey);


	}

	//Falling platformer
	{
		int starting_plat = kinematicPlat(10, 120, 40 + levelFPx, -10 + levelFPy, 2, 270);
		int platformerCheck = makeCheckpoint("Checkpoint.png", 10, 10, 10 + levelFPx, 10 + levelFPy);
		int starting_side = kinematicPlat(10, 80, 105 + levelFPx, -45 + levelFPy, 2, 180);
		int starting_bot_plat = kinematicPlat(10, 60, 135 + levelFPx, -90 + levelFPy, 2, 270);
		int cross_plat1 = kinematicPlat(10, 60, 250 + levelFPx, 160 + levelFPy, 2, 90);
		int cross_plat2 = kinematicPlat(10, 60, 325 + levelFPx, -90 + levelFPy, 2, 270);
		int cross_plat3 = kinematicPlat(10, 60, 400 + levelFPx, 160 + levelFPy, 2, 90);
		int cross_plat4 = kinematicPlat(10, 60, 475 + levelFPx, -90 + levelFPy, 2, 270);
		int cross_plat5 = kinematicPlat(10, 60, 550 + levelFPx, 160 + levelFPy, 2, 90);
		int end_plat = kinematicPlat(10, 120, 610 + levelFPx, 50 + levelFPy, 2, 270);
		//int acidpit = makeDeathObject("Boulder.png", 10, 415 , 402.5, -100);
		//remade deathpit with proper angle
		{

			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Trigger*>(entity);


			//Sets up components
			std::string fileName = "Boulder.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 415);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(372.5 + levelFPx, -100 + levelFPy, 80.f));


			ECS::GetComponent<Trigger*>(entity) = new deathTrigger();
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			((deathTrigger*)ECS::GetComponent<Trigger*>(entity))->SetScene(this);




			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(372.5 + levelFPx), float32(-100 + levelFPy));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(10), float(415), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
			tempPhsBody.SetRotationAngleDeg(90.f);
		};
		int end_wall = kinematicPlat(10, 150, 585 + levelFPx, -25 + levelFPy, 2, 0);
		int roof1 = NonPortPlat(10, 120, 0 + levelFPx, 100 + levelFPy, 2, 180, "NonPortal Wall.png");
		int roof2 = NonPortPlat(10, 215, 112.5 + levelFPx, 160 + levelFPy, 2, 90, "NonPortal Wall.png");
		int roof3 = NonPortPlat(10, 90, 325 + levelFPx, 160 + levelFPy, 2, 90, "NonPortal Wall.png");
		int roof4 = NonPortPlat(10, 90, 475 + levelFPx, 160 + levelFPy, 2, 90, "NonPortal Wall.png");
		int roof5 = NonPortPlat(10, 80, 620 + levelFPx, 160 + levelFPy, 2, 90, "NonPortal Wall.png");
		int roof6 = NonPortPlat(10, 60, 665 + levelFPx, 130 + levelFPy, 2, 180, "NonPortal Wall.png");
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
			if (!squarepickup) {
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 160000.f), true);
				canJump.m_canJump = false;
			}
		}
	}

	/*if (Input::GetKeyDown(Key::F) && playerx > (squarepositionx - 30) && playerx < (squarepositionx + 30) && playery >(squarepositiony - 30) && playery < (squarepositiony + 30))
	{
		squarepickup = true;
	}
	*/

	if (Input::GetKeyDown(Key::R)) //reset portals
	{
		if (bPortal != NULL) //Destroy previous portal
		{
			PhysicsBody::m_bodiesToDelete.push_back(bPortal);
			bPortal = NULL;
		}
		if (oPortal != NULL) //Destroy previous portal
		{
			PhysicsBody::m_bodiesToDelete.push_back(oPortal);
			oPortal = NULL;
		}
	}

	if (Input::GetKeyDown(Key::Zero))
	{
		PhysicsPlayground::respawn();
		
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
	
	

	//spawn portals
	if (Input::GetKeyDown(Key::Q)) //Blue 
	{
		portalProj(true, playerx, playery, portalGunAngle);
		//bluePortal(playerx +20, playery, 0);
	}
	if (Input::GetKeyDown(Key::E)) //Orange
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
	
	portaloQueueCheck();
	portalbQueueCheck();
	

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

	ECS::GetComponent<Player>(MainEntities::MainPlayer()).Update();

	if (square1x > 790 + CompCubex && square1x < 810 + CompCubex && square1y > -50 + CompCubey && square1y < 50 + CompCubey) {
		square1.SetPosition(b2Vec2(1000000, 0));
	}


	if (squarepickup) {
			square1.SetPosition(b2Vec2(playerx + 30, playery));
		}
	portalgun.SetPosition(b2Vec2(playerx + 5, playery));
	

	portalgun.SetRotationAngleDeg(portalGunAngle);
	
}
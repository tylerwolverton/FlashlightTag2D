#include "LevelFactory.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "Level.h"
#include "Level1.h"
#include "MainMenuLevel.h"

// from rapidjson
#include "filereadstream.h"

const std::string LevelFactory::LevelNames::MainMenu = "MainMenu";
const std::string LevelFactory::LevelNames::Level1 = "Level1";

const std::string LevelFactory::LevelPaths::MainMenu = "resources/levels/main_menu.json";
const std::string LevelFactory::LevelPaths::Level1 = "resources/levels/level1.json";

LevelFactory::LevelFactory(std::shared_ptr<PhysicsManager> physicsManager, std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<ActorFactory> actorFactory)
	: m_pPhysicsManager(physicsManager),
	  m_pGraphicsManager(graphicsManager),
	  m_pActorFactory(actorFactory)
{
}

LevelFactory::~LevelFactory()
{
}

void LevelFactory::ChangeLevel(const std::string& levelPath)
{
	FILE* fp;
	fopen_s(&fp, levelPath.c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);
	fclose(fp);

	auto newLevel = createLevelFromJson(d["level"]);

	m_pPhysicsManager->ClearPhysicsComponents();
	m_pPhysicsManager->SetLevelSize(newLevel->GetLevelSize());

	m_pGraphicsManager->Reset();
	m_pGraphicsManager->LoadNewLevel(newLevel);

	m_pActorFactory->ClearActors();
	m_pActorFactory->CreateActorsFromJSONArray(d["actor_list"], *m_pPhysicsManager, *m_pGraphicsManager, newLevel);

	m_pGraphicsManager->AddCamera(m_pActorFactory->CreateCamera(newLevel->GetLevelSize()));
}

std::shared_ptr<Level> LevelFactory::createLevelFromJson(const rapidjson::Value& level)
{
	int levelWidth = level["size"]["x"].GetInt();
	int levelHeight = level["size"]["y"].GetInt();
	std::string sprite = level["sprite"].GetString();
	std::string vertShader = level["vert_shader"].GetString();
	std::string fragShader = level["frag_shader"].GetString();

	std::string levelName = level["name"].GetString();
	if (levelName == LevelNames::Level1)
	{
		return std::make_shared<Level1>(levelWidth, levelHeight, sprite, vertShader, fragShader);
	}
	else if (levelName == LevelNames::MainMenu)
	{
		return std::make_shared<Level1>(levelWidth, levelHeight, sprite, vertShader, fragShader);
	}

	return nullptr;
}
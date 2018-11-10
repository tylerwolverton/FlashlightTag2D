#include "LevelFactory.h"
#include "ActorFactory.h"
#include "Level.h"
#include "Level1.h"
#include "Level2.h"
#include "Overworld1.h"
#include "LevelWithLightingLight.h"
#include "MainMenuLevel.h"
#include "GameTile.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "DefaultPhysicsComponent.h"

#include "ServiceLocator.h"
#include "PhysicsManager.h"
#include "World.h"

// from rapidjson
#include "filereadstream.h"

const std::string LevelFactory::LevelNames::MainMenu = "MainMenu";
const std::string LevelFactory::LevelNames::Level1 = "Level1";
const std::string LevelFactory::LevelNames::Level2 = "Level2";
const std::string LevelFactory::LevelNames::BossLevel1 = "BossLevel1";
const std::string LevelFactory::LevelNames::Overworld1 = "Overworld1";
const std::string LevelFactory::LevelNames::ControlsScreen = "ControlsScreen";
const std::string LevelFactory::LevelNames::LoseScreen = "LoseScreen";
const std::string LevelFactory::LevelNames::WinScreen = "WinScreen";

const std::string LevelFactory::LevelPaths::MainMenu = "resources/levels/main_menu.json";
const std::string LevelFactory::LevelPaths::Level1 = "resources/levels/level1.json";
const std::string LevelFactory::LevelPaths::Level2 = "resources/levels/level2.json";
const std::string LevelFactory::LevelPaths::Overworld1 = "resources/levels/overworld1.json";
const std::string LevelFactory::LevelPaths::ControlsScreen = "resources/levels/controls_screen.json";
const std::string LevelFactory::LevelPaths::LoseScreen = "resources/levels/lose_screen.json";
const std::string LevelFactory::LevelPaths::WinScreen = "resources/levels/win_screen.json";

LevelFactory::LevelFactory(std::shared_ptr<ActorFactory> actorFactory)
    : m_pActorFactory(actorFactory)
{
}

LevelFactory::~LevelFactory()
{
}

void LevelFactory::ChangeLevel(const std::string& levelPath)
{
    //ServiceLocator::GetWorld()->PauseGame();

    FILE* fp;
    fopen_s(&fp, levelPath.c_str(), "rb");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document d;
    d.ParseStream(is);
    fclose(fp);

    m_curLevel = createLevelFromJson(d["level"]);
    m_pActorFactory->InitLevelActors(d["actor_list"], m_curLevel);
    
    m_curLevel->SetupLevel();
    //ServiceLocator::GetWorld()->ResumeGame();
}

std::shared_ptr<Level> LevelFactory::createLevelFromJson(const rapidjson::Value& level)
{
    std::string levelName = level["name"].GetString();
    int levelWidth = level["size"]["x"].GetInt();
    int levelHeight = level["size"]["y"].GetInt();
    std::string vertShader = level["vert_shader"].GetString();
    std::string fragShader = level["frag_shader"].GetString();

    // clear existing tile vector
    for (auto vec : m_tileVec)
    {
        vec.clear();
    }
    m_tileVec.clear();

    if (level.HasMember("sprite"))
    {
        std::string sprite = level["sprite"].GetString();

        if (levelName == LevelNames::Level1)
        {
            return std::make_shared<Level1>(levelWidth, levelHeight, sprite, vertShader, fragShader);
        }
        if (levelName == LevelNames::Overworld1
            || levelName == LevelNames::ControlsScreen
            || levelName == LevelNames::LoseScreen
            || levelName == LevelNames::WinScreen)
        {
            return std::make_shared<LevelWithLightingLight>(levelWidth, levelHeight, sprite, vertShader, fragShader);
        }
        else if (levelName == LevelNames::MainMenu)
        {
            return std::make_shared<MainMenuLevel>(levelWidth, levelHeight, sprite, vertShader, fragShader);
        }
    }

    if (level.HasMember("tiles"))
    {
        int tileVecArrayIdx = 0;
        const rapidjson::Value& tiles = level["tiles"];
        for (rapidjson::SizeType i = 0; i < tiles.Size(); i++)
        {
            m_tileVec.push_back(std::vector<std::shared_ptr<GameTile>>());
            for (rapidjson::SizeType j = 0; j < tiles[i].Size(); j++)
            {
                int spriteIdx = tiles[i][j].GetInt();
                std::shared_ptr<TransformComponent> transformComp = std::make_shared<TransformComponent>(-1, 
                                                                                                         Vector2D<float>(Level::TILE_WIDTH * j, 
                                                                                                                         levelHeight - (Level::TILE_HEIGHT * i)),
                                                                                                         Vector2D<float>(Level::TILE_WIDTH, Level::TILE_HEIGHT),
                                                                                                         Vector2D<float>(0, 0));
                // Hack to give walls a physics comp. Replace this with any other approach later
                if (spriteIdx == 3)
                {
                    std::shared_ptr<PhysicsComponent> physicsComp = std::make_shared<DefaultPhysicsComponent>(-1,
                                                                                                               transformComp,
                                                                                                               0,
                                                                                                               9999,
                                                                                                               .9f);


                    m_tileVec[tileVecArrayIdx].push_back(std::make_shared<GameTile>(spriteIdx, transformComp, physicsComp));
                }
                else
                {
                    m_tileVec[tileVecArrayIdx].push_back(std::make_shared<GameTile>(spriteIdx, transformComp));
                }
            }
            tileVecArrayIdx++;
        }

        if (levelName == LevelNames::MainMenu)
        {
            return std::make_shared<MainMenuLevel>(levelWidth, levelHeight, m_tileVec, vertShader, fragShader);
        }
        else if (levelName == LevelNames::Overworld1)
        {
            return std::make_shared<Overworld1>(levelWidth, levelHeight, m_tileVec, vertShader, fragShader);
        }
        else if (levelName == LevelNames::Level1)
        {
            return std::make_shared<Level1>(levelWidth, levelHeight, m_tileVec, vertShader, fragShader);
        }
        else if (levelName == LevelNames::Level2)
        {
            return std::make_shared<Level2>(levelWidth, levelHeight, m_tileVec, vertShader, fragShader);
        }
        else if (levelName == LevelNames::BossLevel1
                || levelName == LevelNames::ControlsScreen
                || levelName == LevelNames::LoseScreen
                || levelName == LevelNames::WinScreen)
        {
            return std::make_shared<LevelWithLightingLight>(levelWidth, levelHeight, m_tileVec, vertShader, fragShader);
        }
    }

    return nullptr;
}

void LevelFactory::UpdateLevelTilesForActor(std::shared_ptr<GameActor> actor)
{
    m_curLevel->AddActorToTiles(actor);
}
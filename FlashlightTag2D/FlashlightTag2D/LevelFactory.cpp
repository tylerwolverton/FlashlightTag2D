#include "LevelFactory.h"
#include "ActorFactory.h"
#include "Level.h"
#include "Level1.h"
#include "OutsideLevelDay.h"
#include "MainMenuLevel.h"

// from rapidjson
#include "filereadstream.h"

const std::string LevelFactory::LevelNames::MainMenu = "MainMenu";
const std::string LevelFactory::LevelNames::Level1 = "Level1";
const std::string LevelFactory::LevelNames::Overworld1 = "Overworld1";

const std::string LevelFactory::LevelPaths::MainMenu = "resources/levels/main_menu.json";
const std::string LevelFactory::LevelPaths::Level1 = "resources/levels/level1.json";
const std::string LevelFactory::LevelPaths::Overworld1 = "resources/levels/overworld1.json";

LevelFactory::LevelFactory(std::shared_ptr<ActorFactory> actorFactory)
	: m_pActorFactory(actorFactory)
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
    m_pActorFactory->InitLevelActors(d["actor_list"], newLevel);
}

std::shared_ptr<Level> LevelFactory::createLevelFromJson(const rapidjson::Value& level)
{
    std::string levelName = level["name"].GetString();
	int levelWidth = level["size"]["x"].GetInt();
	int levelHeight = level["size"]["y"].GetInt();
	std::string vertShader = level["vert_shader"].GetString();
	std::string fragShader = level["frag_shader"].GetString();

    if (level.HasMember("sprite"))
    {
        std::string sprite = level["sprite"].GetString();

        if (levelName == LevelNames::Level1)
        {
            return std::make_shared<Level1>(levelWidth, levelHeight, sprite, vertShader, fragShader);
        }
        if (levelName == LevelNames::Overworld1)
        {
            return std::make_shared<OutsideLevelDay>(levelWidth, levelHeight, sprite, vertShader, fragShader);
        }
        else if (levelName == LevelNames::MainMenu)
        {
            return std::make_shared<MainMenuLevel>(levelWidth, levelHeight, sprite, vertShader, fragShader);
        }
    }

    if (level.HasMember("tiles"))
    {
        int tileVecArrayIdx = 0;
        std::vector<std::vector<int>> tileVec;
        const rapidjson::Value& tiles = level["tiles"];
        for (rapidjson::SizeType i = 0; i < tiles.Size(); i++)
        {
            tileVec.push_back(std::vector<int>());
            for (rapidjson::SizeType j = 0; j < tiles[i].Size(); j++)
            {
                tileVec[tileVecArrayIdx].push_back(tiles[i][j].GetInt());
            }
            tileVecArrayIdx++;
        }

        if (levelName == LevelNames::Level1)
        {
            return std::make_shared<Level1>(levelWidth, levelHeight, tileVec, vertShader, fragShader);
        }
        if (levelName == LevelNames::Overworld1)
        {
            return std::make_shared<OutsideLevelDay>(levelWidth, levelHeight, tileVec, vertShader, fragShader);
        }
        else if (levelName == LevelNames::MainMenu)
        {
            return std::make_shared<MainMenuLevel>(levelWidth, levelHeight, tileVec, vertShader, fragShader);
        }
    }
	return nullptr;
}
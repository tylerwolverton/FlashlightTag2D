#pragma once
#include <memory>
#include <vector>
#include <string>
#include <document.h>

class Level;
class ActorFactory;
class GameTile;
class GameActor;

class LevelFactory
{
public:
    const class LevelNames
    {
    public:
        static const std::string MainMenu;
        static const std::string Level1;
        static const std::string Level2;
        static const std::string BossLevel1;
        static const std::string Overworld1;
        static const std::string ControlsScreen;
        static const std::string LoseScreen;
        static const std::string WinScreen;
    }; 

    const class LevelPaths
    {
    public:
        static const std::string MainMenu;
        static const std::string Level1;
        static const std::string Level2;
        static const std::string BossLevel1;
        static const std::string Overworld1;
        static const std::string ControlsScreen;
        static const std::string LoseScreen;
        static const std::string WinScreen;
    };

    LevelFactory(std::shared_ptr<ActorFactory> actorFactoryPtr);
    ~LevelFactory();

    void ChangeLevel(const std::string& levelPath);

    void LevelFactory::UpdateLevelTilesForActor(std::shared_ptr<GameActor> actorPtr);

private:
    std::shared_ptr<ActorFactory> m_actorFactoryPtr;
	std::vector<std::vector<std::shared_ptr<GameTile>>> m_tilePtrVecVec;
	std::shared_ptr<Level> m_curLevelPtr;

    std::shared_ptr<Level> createLevelFromJson(const rapidjson::Value& level);
};


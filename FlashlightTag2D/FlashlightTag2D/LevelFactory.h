#pragma once
#include<memory>
#include<string>
#include<document.h>

class Level;
class ActorFactory;

class LevelFactory
{
public:
	const class LevelNames
	{
	public:
		static const std::string MainMenu;
		static const std::string Level1;
        static const std::string Overworld1;
	}; 

	const class LevelPaths
	{
	public:
		static const std::string MainMenu;
		static const std::string Level1;
        static const std::string Overworld1;
	};

	LevelFactory(std::shared_ptr<ActorFactory> actorFactory);
	~LevelFactory();

	void ChangeLevel(const std::string& levelPath);

private:
	std::shared_ptr<ActorFactory> m_pActorFactory;

	std::shared_ptr<Level> createLevelFromJson(const rapidjson::Value& level);
};


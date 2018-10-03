#include "Level1.h"
#include "Shader.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"
#include "GameStateComponent.h"

#define MAX_NUM_LIGHTS 20

Level1::Level1(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
	: Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

Level1::Level1(int levelWidth, int levelHeight, std::vector<std::vector<int>> tileVec, std::string vertexShader, std::string fragmentShader)
    : Level(levelWidth, levelHeight, tileVec, vertexShader, fragmentShader)
{
}

Level1::~Level1()
{
}

void Level1::PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrVec, Vector2D<float> cameraPos)
{
	// opengl likes flat arrays...
	int lightCount = 0;
	std::vector<GLfloat> lightVec;
	std::vector<GLfloat> lightDirVec;
	std::vector<GLfloat> lightPosVec;
	for (auto graphicsComponent : graphicsComponentPtrVec)
	{
		if (lightCount >= MAX_NUM_LIGHTS) { break; }

		auto actorTransformComponent = *(graphicsComponent.second->GetTransformComponent());

        std::shared_ptr<GameActor> parent = graphicsComponent.second->GetParent();
        if (parent == nullptr)
        {
            continue;
        }

        std::shared_ptr<GameStateComponent> gameStateComp = parent->GetGameStateComponent();
        if (gameStateComp == nullptr)
        {
            continue;
        }

        if(gameStateComp->GetName() == "Player")
        {
            Vector2D<float> actorPos = actorTransformComponent.GetPosition();
            Vector2D<float> actorSize = actorTransformComponent.GetSize();
            Vector2D<float> actorLocation = actorPos - cameraPos - graphicsComponent.second->GetImageOffset();

            lightVec.push_back(actorLocation.x + actorSize.x / 2); lightVec.push_back(actorLocation.y + actorSize.y / 2); lightVec.push_back(100.0f);

            lightDirVec.push_back(actorTransformComponent.GetDirection().x);
            lightDirVec.push_back(actorTransformComponent.GetDirection().y);

            lightPosVec.push_back(actorLocation.x + actorTransformComponent.GetSize().x / 2);
            lightPosVec.push_back(actorLocation.y + actorTransformComponent.GetSize().y / 2);

            lightCount++;
        }
	}

	// Use a special vector to tell the shader there are no more lights
	if (lightCount < MAX_NUM_LIGHTS)
	{
		lightVec.push_back(-901.0f); lightVec.push_back(-901.0f); lightVec.push_back(-901.0f);
	}

	m_shader->SetVec3("lightSrc", &lightVec.front(), lightVec.size() / 3);
//	m_shader->SetVec2("lightDir", &lightDirVec.front(), lightDirVec.size() / 2);
//	m_shader->SetVec2("lightPos", &lightPosVec.front(), lightPosVec.size() / 2);
}
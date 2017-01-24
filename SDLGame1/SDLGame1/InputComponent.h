#pragma once
#include "ActorComponent.h"
class Command;

class InputComponent : public ActorComponent
{
public:
	InputComponent();
	virtual ~InputComponent();

	bool Init(tinyxml2::XMLElement* pData) override;
	void PostInit() override;
	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;

	/*class InputState {
	public:*/

		enum EInputValues : uint32_t
		{
			None = 0,
			Esc = 1 << 0,
			W = 1 << 1,
			A = 1 << 2,
			S = 1 << 3,
			D = 1 << 4
		};


private:
	std::list<std::shared_ptr<Command>> HandleInput();

	std::shared_ptr<Command> buttonEsc;
	std::shared_ptr<Command> buttonW;
	std::shared_ptr<Command> buttonA;
	std::shared_ptr<Command> buttonS;
	std::shared_ptr<Command> buttonD;

	//	inline constexpr EInputValues operator|=(EInputValues& a, EInputValues b)
	//	{
	//		return a = static_cast<EInputValues> (a | b);
	//	}

	//	inline constexpr EInputValues operator&=(EInputValues& a, EInputValues b)
	//	{
	//		return a = static_cast<EInputValues> (a & b);
	//	}
	//};
};


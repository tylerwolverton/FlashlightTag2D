#pragma once
#include "ActorComponent.h"

class InputComponent : public ActorComponent
{
public:
	InputComponent();
	virtual ~InputComponent();

	bool VInit(tinyxml2::XMLElement* pData) override;
	void VPostInit() override;
	void VUpdate(int deltaMs) override;

	ComponentId VGetComponentId() const override;

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


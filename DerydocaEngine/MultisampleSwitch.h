#pragma once
#include "GameComponent.h"
#include "Keyboard.h"

class MultisampleSwitch : public GameComponent
{
public:
	GENINSTANCE(MultisampleSwitch);

	MultisampleSwitch();
	~MultisampleSwitch();

	virtual void init();
	virtual void update(float deltaTime);

	void setEnableMultisample(bool enable);
private:
	Keyboard* m_keyboard;
	bool m_enableMultisample;
	bool m_msaaSupported;
};


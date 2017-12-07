#pragma once
#include "yaml.h"
#include <assert.h>

class EngineSettings
{
public:
	EngineSettings();
	~EngineSettings();
private:
	void onScalarRead(yaml_event_t event);

	int m_width;
	int m_height;
};


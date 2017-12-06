#pragma once
#include "yaml.h"
#include <assert.h>

class EngineSettings
{
public:
	EngineSettings();
	~EngineSettings();
private:
	int m_width;
	int m_height;
};


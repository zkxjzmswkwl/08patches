#pragma once
#include "Watcher.h"

class Monitor
{
private:
	bool m_bActive;

public:
	bool IsActive();
	void TurnOn();
	void TurnOff();
};


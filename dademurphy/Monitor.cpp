#include "pch.h"
#include "Monitor.h"

bool Monitor::IsActive()
{
	return this->m_bActive;
}

void Monitor::TurnOn()
{
	this->m_bActive = true;
}

void Monitor::TurnOff()
{
	this->m_bActive = false;
}

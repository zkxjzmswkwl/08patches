#pragma once
#include "pch.h"
#include <vector>

struct BlizzThread
{
	enum State { UNTOUCHED, SUSPENDED, RUNNING };
	BlizzThread(uint32 threadId, State state) :
		threadId(threadId), state(state) {}

	uint32 threadId;
	State state;
	Handle handle = NULL;

	void Suspend()
	{
		if (this->handle == NULL)
		{
			this->handle = OpenThread(THREAD_ALL_ACCESS, FALSE, this->threadId);
		}

		if (this->state != State::SUSPENDED)
		{
			SuspendThread(this->handle);
			this->state = State::SUSPENDED;
		}
	}
};

class Prep
{
private:
	bool m_bIsDebuggerPresent;
	bool m_bQuerySysInfo;
	Vector<BlizzThread> m_vApplicationThreads;

public:
	bool HasPatchedDebuggerPresent();

	bool HasPatchedQuerySysInfo();

	void SuspendApplicationThreads();

	void ResumeApplicationThreads();

	Vector<BlizzThread>* GetApplicationThreadsPtr();
};

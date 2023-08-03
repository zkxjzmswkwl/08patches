#include "pch.h"
#include "Prep.h"

//---------------------------------------------------------------------------
Vector<BlizzThread>* Prep::GetApplicationThreadsPtr()
{
	return &this->m_vApplicationThreads;
}
//---------------------------------------------------------------------------
void Prep::SuspendApplicationThreads()
{
	auto appThreadsVecPtr = this->GetApplicationThreadsPtr();
	const auto threads = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	ThreadEntry entry;
	entry.dwSize = sizeof(THREADENTRY32);

	Thread32First(threads, &entry);
	while (Thread32Next(threads, &entry))
	{
		// *puts on silly goggles*
		// `CreateToolhelp32Snapshot` is not limited to the scope of current process.
		// You'd think I wouldn't make that mistake given how common the function is when enumerating processes.
		// You'd really think I wouldn't be silly enough to suspend every thread on my machine.
		// You'd *think* that.
		if (entry.th32OwnerProcessID == GetCurrentProcessId() && entry.th32ThreadID != GetCurrentThreadId())
		{
			// `BlizzThread` stores necessary data to resume later on without needing to create & iterate over another snapshot.
			auto blizzThread = BlizzThread(entry.th32ThreadID, BlizzThread::State::UNTOUCHED);
			blizzThread.Suspend();
			// We put them here.
			appThreadsVecPtr->push_back(blizzThread);
		}
	}
}
//---------------------------------------------------------------------------
void Prep::ResumeApplicationThreads()
{
	for (auto t : *this->GetApplicationThreadsPtr())
	{
		ResumeThread(t.handle);
		t.state = BlizzThread::State::RUNNING;
	}
}
//---------------------------------------------------------------------------

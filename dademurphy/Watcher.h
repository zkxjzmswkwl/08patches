#pragma once

template <typename T>
class Watcher
{
private:
	T m_tExpectedValue;
	String m_sLabel;
};

extern "C" int __fastcall GetNtGlobal();
// No setter prototypes should actually be here. Move eventually.
extern "C" void __fastcall SetNtGlobal(int);


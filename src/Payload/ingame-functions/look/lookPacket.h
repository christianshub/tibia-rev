#pragma once
#include "iostream"
#include "../../globals.h"
#include "../trampoline.h"

namespace lookPacket
{
	namespace
	{
		// Packet1
		typedef void(__thiscall* _pkgId)(unsigned int id);
		_pkgId PktId = (_pkgId)(globals::moduleBase + 0x1B5C10);

		// Packet2
		typedef void(__thiscall* _pktUnk2)(int num);
		_pktUnk2 PktUnk2 = (_pktUnk2)(globals::moduleBase + 0x1B6180);

		// Packet3
		typedef void(__thiscall* _pktUnk3)(int num);
		_pktUnk3 PktUnk3 = (_pktUnk3)(globals::moduleBase + 0x1B6180);

		// Packet4
		typedef void(__thiscall* _pktUnk4)(int num);
		_pktUnk4 PktUnk4 = (_pktUnk4)(globals::moduleBase + 0x1B5FD0);

		// Packet5
		typedef void(__thiscall* _pktUnk5)(int num);
		_pktUnk5 PktUnk5 = (_pktUnk5)(globals::moduleBase + 0x1B6180);

		// Packet6
		typedef void(__thiscall* _pktUnk6)(int num);
		_pktUnk6 PktUnk6 = (_pktUnk6)(globals::moduleBase + 0x1B5FD0);

		// Packet7
		typedef void(__thiscall* _pktFlag)(BYTE num);
		_pktFlag PktFlag = (_pktFlag)(globals::moduleBase + 0x1B6B10);
	}

	void Send()
	{
		PktId(0x8C);
		PktUnk2(0x00007D50);
		PktUnk3(0x00007DC8);
		PktUnk4(0x00000007);
		PktUnk5(0x00000063);
		PktUnk6(0x00000001);
		PktFlag(0x1);
	}
}
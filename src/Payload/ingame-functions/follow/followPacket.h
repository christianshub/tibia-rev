#pragma once
#include "iostream"
#include "../../globals.h"
#include "../trampoline.h"

namespace followPacket
{
	namespace
	{
		// PktId
		typedef void(__fastcall* _pkgId)(int id);
		_pkgId PktId = (_pkgId)(globals::moduleBase + 0x1B5C10);

		// Packet2
		typedef void(__fastcall* _pktObjectId)(int objId);
		_pktObjectId PktObjectId = (_pktObjectId)(globals::moduleBase + 0x1B6330);

		// Packet3
		typedef void(__fastcall* _pktUnk3)(int num);
		_pktUnk3 PktUnk3 = (_pktUnk3)(globals::moduleBase + 0x1B6330);

		// PktFlag
		typedef void(__fastcall* _pktFlag)(BYTE num);
		_pktFlag PktFlag = (_pktFlag)(globals::moduleBase + 0x1B6B10);
	}

	void Send()
	{
		PktId(0xA2);
		PktObjectId(0x40215070);
		PktUnk3(0xF);
		PktFlag(0x1);
		std::cout << "following cat!\n";
	}
}
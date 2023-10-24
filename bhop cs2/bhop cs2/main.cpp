#include "memory.h"
#include <thread>
#include <iostream>
#include <iomanip>

namespace offsets
{
	constexpr std::ptrdiff_t dwForceJump = 0x169E360; // https://github.com/a2x/cs2-dumper/blob/main/generated/offsets.hpp
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1885C28; //

	constexpr std::ptrdiff_t m_fFlags = 0x3C8; //
}

int main()
{

	std::cout << "Nebula External\n\n";

	std::cout << std::hex << std::uppercase;

	std::cout << "dwForceJump: 0x" << offsets::dwForceJump << std::endl;
	std::cout << "dwLocalPlayerPawn: 0x" << offsets::dwLocalPlayerPawn << std::endl;
	std::cout << "m_fFlags: 0x" << offsets::m_fFlags << std::endl;
	
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		uintptr_t localPlayer = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwLocalPlayerPawn);

		if (!localPlayer)
		{
			continue;
		}

		int32_t m_fFlags = VARS::memRead<int32_t>(localPlayer + offsets::m_fFlags);

		if (GetAsyncKeyState(VK_SPACE) && (m_fFlags & (1 << 0)))
		{
			VARS::memWrite(VARS::baseAddress + offsets::dwForceJump, 65537);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			VARS::memWrite(VARS::baseAddress + offsets::dwForceJump, 256);
		}
	}

	return 0;
}

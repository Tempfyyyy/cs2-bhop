// Include the necessary libraries
#include "memory.h"
#include "offsets.h"

#include <thread>
#include <iostream>
#include <iomanip>


//discord : https://discord.gg/RqNF4Hg6uG
//youtube : https://www.youtube.com/@Tempfy

namespace offsets
{
	constexpr std::ptrdiff_t dwForceJump = 0x169E360; // https://github.com/a2x/cs2-dumper/blob/main/generated/offsets.hpp
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1885C28; // https://github.com/a2x/cs2-dumper/blob/main/generated/offsets.hpp

	constexpr std::ptrdiff_t m_fFlags = 0x3C8; // https://github.com/a2x/cs2-dumper/blob/main/generated/client.dll.hpp
}


int main()
{
	// Print the program name
	std::cout << "Nebula External\n\n";

	// Set the output format for hexadecimal numbers
	std::cout << std::hex << std::uppercase;

	// Print the memory offsets
	std::cout << "dwForceJump: 0x" << offsets::dwForceJump << std::endl;
	std::cout << "dwLocalPlayerPawn: 0x" << offsets::dwLocalPlayerPawn << std::endl;
	std::cout << "m_fFlags: 0x" << offsets::m_fFlags << std::endl;

	// Main loop
	while (true)
	{
		// Sleep for 1 millisecond to reduce CPU usage
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Read the local player's memory address
		uintptr_t localPlayer = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwLocalPlayerPawn);

		// If the local player's memory address is null, skip this iteration
		if (!localPlayer)
		{
			continue;
		}

		// Read the local player's flags
		int32_t m_fFlags = VARS::memRead<int32_t>(localPlayer + offsets::m_fFlags);

		// If the space key is pressed and the local player is on the ground, perform a jump
		if (GetAsyncKeyState(VK_SPACE) && (m_fFlags & (1 << 0)))
		{
			VARS::memWrite(VARS::baseAddress + offsets::dwForceJump, 65537);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			VARS::memWrite(VARS::baseAddress + offsets::dwForceJump, 256);
		}
	}

	return 0;
}
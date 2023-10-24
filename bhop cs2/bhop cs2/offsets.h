#pragma once

// Include the necessary libraries
#include <cstddef>

// Define a namespace for offsets
namespace offsets
{
    // dwForceJump offset, can be found in "offsets.hpp"
    constexpr std::ptrdiff_t dwForceJump = 0x169E360;

    // dwLocalPlayerPawn offset, can be found in "offsets.hpp"
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1885C28;

    // m_fFlags offset, can be found in "client.dll"
    constexpr std::ptrdiff_t m_fFlags = 0x3C8;
}

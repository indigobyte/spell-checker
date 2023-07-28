#pragma once

namespace nuspell::icu
{
	__declspec(dllexport) void init() noexcept;
	__declspec(dllexport) void cleanup() noexcept;
}
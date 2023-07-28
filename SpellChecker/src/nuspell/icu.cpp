#include "icu.h"
#include <unicode/uclean.h>

namespace nuspell::icu
{
	void init() noexcept
	{
		UErrorCode code = U_ZERO_ERROR;
		u_init(&code);
	}

	void cleanup() noexcept
	{
		u_cleanup();
	}
}
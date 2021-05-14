#include <windows.h>

#include <filesystem>

#include <unicode/uclean.h>
#include <unicode/putil.h>

namespace
{
	std::filesystem::path full_filename()
	{
	    wchar_t buffer[32768 + 1];
	    GetModuleFileName(NULL, buffer, 32768);
	    return buffer;
	}
    std::filesystem::path path()
	{
	    return full_filename().parent_path();
	}
    std::filesystem::path rpath()
	{
#ifdef _DEBUG
		const auto exe_folder = path();
	    if (exe_folder.filename() == L"__bin64_debug")
			return exe_folder.parent_path() / L"__bin64";
	    if (exe_folder.parent_path().filename() == L"__bin64_debug")
			return exe_folder.parent_path().parent_path() / L"__bin64" / exe_folder.filename();
		return exe_folder;
#else
		return path();
#endif
	}

	class CICUController final
	{
	public:
		CICUController()
		{
			auto icu_data_folder = rpath() / L"icu_data";
			if (!std::filesystem::is_directory(icu_data_folder))
				icu_data_folder = rpath().parent_path() / L"icu_data";
			u_setDataDirectory(icu_data_folder.generic_string().c_str());
			UErrorCode errorCode = U_ZERO_ERROR;
			u_init(&errorCode);
		}

		~CICUController()
		{
			u_cleanup();
		}
	} icu_controller;
}
#pragma once

#include <nuspell/suggester.hxx>

namespace nuspell
{
	class __declspec(dllexport) DrDictionary : private Suggester
	{
	public:
		bool parse_aff_dic(std::istream & aff, std::istream & dic);
		bool spell(std::string_view word) const;
		void suggest(std::string_view word, std::vector<std::string> & out) const;

		auto & words_ref()
		{
			return words;
		}

		auto const& words_ref() const
		{
			return words;
		}
	};
}
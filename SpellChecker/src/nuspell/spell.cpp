#include "spell.h"
#include <sstream>

namespace nuspell
{
	bool DrDictionary::parse_aff_dic(std::istream & aff, std::istream & dic)
	{
		auto err_msg = std::ostringstream{};
		if (parse_aff(aff, err_msg))
			return parse_dic(dic, err_msg);
		return false;
	}

	bool DrDictionary::spell(std::string_view word) const
	{
		if (360 < word.size())
			return false;
		auto word_buf = std::string(word);
		return spell_priv(word_buf);
	}

	void DrDictionary::suggest(std::string_view word, std::vector<std::string> & out) const
	{
		out.clear();
		if (360 < word.size())
			return;
		suggest_priv(word, out);
	}
}
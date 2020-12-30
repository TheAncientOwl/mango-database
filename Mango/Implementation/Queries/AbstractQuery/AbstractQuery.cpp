#include "standard_library.hpp"
#include "AbstractQuery.hpp"

#include "../../StaticBlock/StaticBlock.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API std::vector<std::string_view> AbstractQuery::splitAtChar(std::string_view str, char c)
	{
		std::vector<std::string_view> out;

		auto first = std::cbegin(str);
		auto last = std::find(first, std::cend(str), c);

		while (last != std::cend(str))
		{
			if (first != last)
				out.emplace_back(first._Unwrapped(), std::distance(first, last));

			first = std::next(last);
			last = std::find(first, std::cend(str), c);
		}

		if (first != last)
		{
			std::string_view s(first._Unwrapped(), std::distance(first, last));
			if (!s.empty())
				out.push_back(std::move(s));
		}

		return out;
	}

	MANGO_PUBLIC_API std::vector<std::string_view> AbstractQuery::splitAtCharWithEscape(std::string_view str, char c)
	{
		std::vector<std::string_view> out;

		auto first = std::cbegin(str);
		auto last = std::find(first, std::cend(str), c);

		while (last != std::cend(str))
		{
			if (first != last && *std::prev(last) != MANGO_ESCAPE_CHAR)
			{
				out.emplace_back(first._Unwrapped(), std::distance(first, last));
				first = std::next(last);
			}

			last = std::find(std::next(last), std::cend(str), c);
		}

		if (first != last)
		{
			std::string_view s(first._Unwrapped(), std::distance(first, last));
			if (!s.empty())
				out.push_back(std::move(s));
		}

		return out;
	}

	MANGO_PUBLIC_API void AbstractQuery::splitInCleanStringsAt(std::string_view str, char c, ref<std::vector<std::string>> out)
	{
		out.clear();

		auto first = std::cbegin(str);
		auto last = std::find(first, std::cend(str), c);

		while (last != std::cend(str))
		{
			if (first != last && *std::prev(last) != MANGO_ESCAPE_CHAR)
			{
				std::string_view s(first._Unwrapped(), std::distance(first, last));
				out.push_back(cleanString(trimWhiteSpaces(s), c));
				first = std::next(last);
			}

			last = std::find(std::next(last), std::cend(str), c);
		}

		if (first != last)
		{
			std::string_view s(first._Unwrapped(), std::distance(first, last));
			if (!s.empty())
				out.push_back(cleanString(trimWhiteSpaces(s), c));
		}

	}

	MANGO_PUBLIC_API bool AbstractQuery::isValidIdentifier(std::string_view identifier)
	{
		if (identifier.empty())
			return false;

		if ('0' <= identifier.front() && identifier.front() <= '9')
			return false;

		for (auto it = std::begin(identifier), end = std::end(identifier); it != end; ++it)
			if (s_InvalidIdentifierChars[*it])
				return false;

		return true;
	}

	MANGO_PUBLIC_API std::string_view AbstractQuery::trimWhiteSpaces(std::string_view str)
	{
		while (!str.empty() && str.front() == ' ')
			str.remove_prefix(1);
		while (!str.empty() && str.back() == ' ')
			str.remove_suffix(1);
		return str;
	}

	MANGO_PUBLIC_API std::string AbstractQuery::cleanString(std::string_view str, char escapedChar)
	{
		if (str.empty())
			return {};

		if (str.front() == '"' || str.back() == '"')
		{
			if (str.size() == 1 || str.front() != '"' || str.back() != '"')
				throw InvalidArgumentException({ "Missing '\"' at \"", str, "\""});
			
			std::string s;
			s.reserve(str.size());

			for (auto it = std::next(std::cbegin(str)), next = std::next(it), end = std::cend(str); next != end; ++it, ++next)
			{
				if (*it != MANGO_ESCAPE_CHAR || *next != escapedChar)
					s.push_back(*it);
			}

			s.shrink_to_fit();

			return s;
		}
		else return std::string(str);
	}

	MANGO_PUBLIC_API void AbstractQuery::removeEscapeChar(ref<std::string> str, char escapedChar)
	{
		for (size_t index = 1; index < str.size(); ++index)
			if (str[index] == escapedChar && str[index - 1] == MANGO_ESCAPE_CHAR)
			{
				for (size_t j = index - 1, end = str.size() - 1; j < end; ++j)
					str[j] = str[j + 1];
				str.pop_back();
			}
		str.shrink_to_fit();
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API AbstractQueryStaticInitializer::AbstractQueryStaticInitializer()
	{
		for (bool& b : AbstractQuery::s_InvalidIdentifierChars)
			b = false;

		std::string_view s("`~!@#$%^&*()-+={}[]|\\;:'\",.<>/?");
		for (auto c : s)
			AbstractQuery::s_InvalidIdentifierChars[c] = true;
	}
#pragma endregion

	bool AbstractQuery::s_InvalidIdentifierChars[257];

	MANGO_STATIC_BLOCK
	{
		AbstractQueryStaticInitializer _;
	};
}
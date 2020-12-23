#include "standard_library.hpp"
#include "AbstractQuery.hpp"

namespace Mango::Queries
{
#pragma region MANGO_PROTECTED_API
	MANGO_PROTECTED_API std::vector<std::string_view> AbstractQuery::splitAtAnyOf(std::string_view str, std::string_view delims) const
	{
		std::vector<std::string_view> out;

		auto first = std::cbegin(str);
		auto last = std::find_first_of(first, std::cend(str), std::cbegin(delims), std::cend(delims));

		while (last != std::cend(str))
		{
			if (first != last)
				out.emplace_back(first._Unwrapped(), std::distance(first, last));

			first = std::next(last);
			last = std::find_first_of(first, std::cend(str), std::cbegin(delims), std::cend(delims));
		}

		if (first != last)
		{
			std::string_view s(first._Unwrapped(), std::distance(first, last));
			if (!s.empty())
				out.push_back(std::move(s));
		}

		return out;
	}

	MANGO_PROTECTED_API std::vector<std::string_view> AbstractQuery::splitAtChar(std::string_view str, char c) const
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

	MANGO_PROTECTED_API bool AbstractQuery::isValidIdentifier(std::string_view identifier) const
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

	static_block
	{
		AbstractQueryStaticInitializer _;
	};
}
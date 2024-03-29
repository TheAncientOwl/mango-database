#include "standard_library.hpp"
#include "TruncateTableQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool TruncateTableQuery::match(std::string_view sql) const
	{
		return sql.starts_with("TRUNCATE");
	}

	MANGO_QUERY_API void TruncateTableQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid truncate query syntax");

		if (args[0] != "TRUNCATE" || args[1] != "TABLE")
			throw InvalidArgumentException("Check missing \"TRUNCATE\" nor \"TABLE\" keywords");

		m_TableName = args[2];
	}

	MANGO_QUERY_API void TruncateTableQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));
	}

	MANGO_QUERY_API void TruncateTableQuery::execute(ref<MangoDB> dataBase)
	{
		dataBase.getTable(m_TableName)->clearDataFile();
	}
#pragma endregion

}
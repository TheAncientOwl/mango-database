#include "standard_library.hpp"
#include "DeleteQuery.hpp"

#include "../../FileIO/Binary/Binary.hpp"
using namespace Mango::Implementation::FileIO::Binary;

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool DeleteQuery::match(std::string_view sql) const
	{
		return sql.starts_with("DELETE");
	}

	MANGO_QUERY_API void DeleteQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid delete query syntax");

		if (args[0] != "DELETE" || args[1] != "FROM")
			throw InvalidArgumentException("Invalid delete query syntax");

		m_TableName = args[2];
	}

	MANGO_QUERY_API void DeleteQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exists", std::move(m_TableName));
	}

	MANGO_QUERY_API void DeleteQuery::execute(ref<MangoDB> dataBase)
	{
		MangoClauseGuard _(dataBase);
		auto table = static_cast<const_ptr<Table>>(dataBase.getTable(m_TableName));

		auto tableIterator = table->begin();

		std::filesystem::path dummyFilePath = table->getDataFilePath();

		dummyFilePath.replace_filename("dummy");
		std::fstream dummyFile(dummyFilePath, std::ios::out | std::ios::trunc | std::ios::binary);

		while (tableIterator.advance())
		{
			auto& row = tableIterator.getRow();
			if (!dataBase.m_WhereClause(row))
				serializePOD(dummyFile, row.getData(), row.getSize());
		}

		dummyFile.close();
		tableIterator.releaseFile();

		std::filesystem::remove(table->getDataFilePath());
		std::filesystem::rename(dummyFilePath, table->getDataFilePath());
	}
#pragma endregion

}
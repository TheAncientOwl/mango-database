#include "standard_library.hpp"
#include "MangoDB.hpp"

namespace Mango
{
#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API std::filesystem::path MangoDB::getConfigFilePath() const
	{
		return m_DirectoryPath / "tables" MANGO_CONFIG_EXTENSION;
	}

	MANGO_PRIVATE_API void MangoDB::addTable(std::unique_ptr<Table> table)
	{
		table->createFiles();
		m_Tables.push_back(std::move(table));
		storeTableConfigs();
	}

	MANGO_PRIVATE_API void MangoDB::removeTable(std::string_view tableName)
	{
		std::vector<std::unique_ptr<Table>> newTables;
		newTables.reserve(m_Tables.size() - 1);

		for (auto& table : m_Tables)
			if (table->getName() != tableName)
				newTables.push_back(std::move(table));
			else
				table->removeFiles();

		m_Tables = std::move(newTables);

		storeTableConfigs();
	}

	MANGO_PRIVATE_API const_ptr<Table> MangoDB::getTable(std::string_view tableName) const
	{
		for (const auto& tablePtr : m_Tables)
			if (tablePtr->getName() == tableName)
				return tablePtr.get();
		return nullptr;
	}

	MANGO_PRIVATE_API ptr<Table> MangoDB::getTable(std::string_view tableName)
	{
		for (const auto& tablePtr : m_Tables)
			if (tablePtr->getName() == tableName)
				return tablePtr.get();
		return nullptr;
	}

	MANGO_PRIVATE_API const_ref<std::vector<std::unique_ptr<Table>>> MangoDB::tables() const
	{
		return m_Tables;
	}

	MANGO_PRIVATE_API void MangoDB::storeTableConfigs() const
	{
		std::fstream file(getConfigFilePath(), std::ios::out | std::ios::trunc | std::ios::binary);

		size_t tablesSize = m_Tables.size();
		serializePOD(file, &tablesSize);

		for (const auto& table : m_Tables)
		{
			size_t tableNameLength = table->getName().length() + 1;
			serializePOD(file, &tableNameLength);
			serializePOD(file, table->getName().data(), tableNameLength);

			table->serializeConfig();
		}

		file.close();
	}
#pragma endregion
	
#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API void MangoDB::loadTableConfigs()
	{
		std::fstream file(getConfigFilePath(), std::ios::in | std::ios::binary);

		size_t tablesSize = 0;
		deserializePOD(file, &tablesSize);
		m_Tables.reserve(tablesSize);

		for (size_t i = 0; i < tablesSize; ++i)
		{
			size_t tableNameLenth = 0;
			deserializePOD(file, &tableNameLenth);
			char buffer[MANGO_MAX_TABLE_NAME_LENGTH + 1]{};
			deserializePOD(file, buffer, tableNameLenth);

			std::vector<Column> columns;
			auto& table = m_Tables.emplace_back(std::make_unique<Table>(buffer, m_DirectoryPath, std::move(columns)));
			table->deserializeConfig();
		}

		file.close();
	}

	MANGO_PUBLIC_API const_ref<std::vector<Row>> MangoDB::lastResult()
	{
		return m_LastResult;
	}

	MANGO_PUBLIC_API std::vector<Row> MangoDB::extractLastResult()
	{
		std::vector<Row> dummy(std::move(m_LastResult));
		std::vector<Row>().swap(m_LastResult);
		return dummy;
	}

	MANGO_PUBLIC_API void MangoDB::disposeLastResult()
	{
		std::vector<Row>().swap(m_LastResult);
	}

	void MangoDB::setWhereClause(WhereClause whereClause)
	{
		m_WhereClause = whereClause;
	}

	void MangoDB::resetWhereClause()
	{
		m_WhereClause = &RowFilters::allwaysTrue;
	}

	void MangoDB::setSetClause(SetClause setClause)
	{
		m_SetClause = setClause;
	}

	void MangoDB::resetSetClause()
	{
		m_SetClause = &RowFilters::doNothing;
	}

	MANGO_PUBLIC_API MangoDB::MangoDB(std::filesystem::path dataBaseDirectoryPath)
		: m_WhereClause(&RowFilters::allwaysTrue), m_SetClause(&RowFilters::doNothing)
	{
		m_DirectoryPath = std::move(dataBaseDirectoryPath);
		std::filesystem::create_directories(m_DirectoryPath);

		std::fstream file;
		file.open(getConfigFilePath(), std::ios::out | std::ios::app | std::ios::binary);
		file.close();
	}

	MANGO_PUBLIC_API MangoDB::~MangoDB()
	{
		storeTableConfigs();
	}

	MANGO_PUBLIC_API std::ostream& operator<<(std::ostream& out, const MangoDB& mango)
	{
		out << ccolor::light_red << "|_____________[Data Base]_____________|\n";
		for (const auto& table : mango.tables())
			out << *table << '\n';

		return out;
	}
#pragma endregion
}

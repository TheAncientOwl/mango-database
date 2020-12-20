#include "standard_library.hpp"
#include "MangoDB.hpp"

namespace Mango
{
	void PUBLIC_API MangoDB::storeTables() const
	{
		std::fstream file(getConfigFilePath(), std::ios::out | std::ios::trunc | std::ios::binary);

		size_t tablesSize = m_Tables.size();
		serializePOD(file, &tablesSize);

		for (const auto& table : m_Tables)
		{
			size_t tableNameLength = table->name().length() + 1;
			serializePOD(file, &tableNameLength);
			serializePOD(file, table->name().data(), tableNameLength);
			
			table->serializeConfig();
		}

		file.close();
	}

	void PUBLIC_API MangoDB::loadTables()
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

	const_ref<std::vector<std::unique_ptr<Table>>> MangoDB::tables() const
	{
		return m_Tables;
	}

	inline std::filesystem::path MangoDB::getConfigFilePath() const
	{
		return m_DirectoryPath / "tables" MANGO_CONFIG_EXTENSION;
	}

	void PRIVATE_API MangoDB::addTable(std::unique_ptr<Table> table)
	{
		table->createFiles();
		m_Tables.push_back(std::move(table));
		storeTables();
	}

	void PRIVATE_API MangoDB::removeTable(std::string_view tableName)
	{
		std::vector<std::unique_ptr<Table>> newTables;
		newTables.reserve(m_Tables.size() - 1);

		for (auto& table : m_Tables)
			if (table->name() != tableName)
				newTables.push_back(std::move(table));
			else
				table->removeFiles();

		m_Tables = std::move(newTables);

		storeTables();
	}

	MangoDB::MangoDB(std::filesystem::path dataBaseDirectoryPath)
	{
		m_DirectoryPath = std::move(dataBaseDirectoryPath);
		std::filesystem::create_directories(m_DirectoryPath);

		std::fstream file;
		file.open(getConfigFilePath(), std::ios::out | std::ios::app | std::ios::binary);
		file.close();
	}

}
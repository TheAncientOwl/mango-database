#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "../../MangoDB/MangoDB.fwd.hpp"

#include "../Column/Column.hpp"
#include "../Row/Row.hpp"
#include "ConstTableIterator.hpp"

#define MANGO_CONFIG_EXTENSION ".mangocfg"
#define MANGO_DATA_EXTENSION   ".mangodb"

namespace Mango::Implementation
{
	class Table
	{
	private:
		std::string m_Name;
		std::filesystem::path m_DirectoryPath;
		
		std::vector<Column> m_Columns;

	private:
		MANGO_API std::filesystem::path getConfigFilePath() const;
		MANGO_API std::filesystem::path getDataFilePath() const;
		MANGO_API const_ref<std::filesystem::path> getDirectoryPath() const;

		MANGO_API void createFiles();
		MANGO_API void removeFiles();
		MANGO_API void clearDataFile();
		MANGO_API void serializeConfig();
		MANGO_API void deserializeConfig();

		MANGO_API void insertRow(const_ref<Row> row);

		MANGO_API const_ref<std::vector<Column>> getColumns() const;

		MANGO_API std::string_view getName() const;

		MANGO_API const_ptr<Column> getColumn(std::string_view columnName) const;
		MANGO_API ptr<Column> getColumn(std::string_view columnName);

		MANGO_API const_ref<Column> getColumn(int index) const;
		MANGO_API ref<Column> getColumn(int index);

		MANGO_API size_t getColumnIndex(std::string_view columnName) const;

		MANGO_API std::shared_ptr<RowConfiguration> makeRowConfiguration() const;

		MANGO_API TableIterator begin();
		MANGO_API ConstTableIterator begin() const;

	public:
		MANGO_PUBLIC_API Table(std::string name, const_ref<std::filesystem::path> dataBaseDirectoryPath, std::vector<Column> columns);

		MANGO_PUBLIC_API Table() = default;
		MANGO_PUBLIC_API Table(const Table&) = delete;
		MANGO_PUBLIC_API Table(Table&&) noexcept = default;
		MANGO_PUBLIC_API Table& operator=(const Table&) = delete;
		MANGO_PUBLIC_API Table& operator=(Table&&) noexcept = default;
		MANGO_PUBLIC_API ~Table() = default;

	private:
		friend class Mango::MangoDB;
		friend class Mango::Implementation::Queries::TruncateTableQuery;
		friend class Mango::Implementation::Queries::InsertIntoQuery;
		friend class Mango::Implementation::Queries::SelectQuery;
		friend class Mango::Implementation::Queries::DeleteQuery;
		friend class Mango::Implementation::Queries::UpdateQuery;
		friend class Mango::Implementation::Queries::CommandLineAdapter::WhereCondition;
		friend class Mango::Implementation::Queries::CommandLineAdapter::UpdateQueryCLI;
		friend class Mango::Implementation::Queries::CommandLineAdapter::SetClauseData;
		friend class Mango::Implementation::Queries::ImportDataQuery;
		friend std::ostream& operator<<(std::ostream& out, const Table& table);
	};
}
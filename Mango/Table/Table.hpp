#pragma once
namespace Mango
{
	class MangoDB;
	namespace Queries
	{
		class TruncateTableQuery;
		class InsertIntoQuery;
		class SelectQuery;
		class DeleteQuery;
	}
}

#include "../Column/Column.hpp"
#include "../Row/Row.hpp"
#include "TableIterator.hpp"

#define MANGO_CONFIG_EXTENSION ".mangocfg"
#define MANGO_DATA_EXTENSION   ".mangodb"

namespace Mango
{
	class Table
	{
	private: /// Fields
		std::string m_Name;
		std::filesystem::path m_DirectoryPath;
		
		std::vector<Column> m_Columns;
		std::map<std::string_view, size_t> m_ColumnIndexes;

	private: /// API
		std::filesystem::path PRIVATE_API getConfigFilePath() const;
		std::filesystem::path PRIVATE_API getDataFilePath() const;
		const_ref<std::filesystem::path> PRIVATE_API getDirectoryPath() const;

		void PRIVATE_API createFiles();
		void PRIVATE_API removeFiles();
		void PRIVATE_API clearDataFile();
		void PRIVATE_API serializeConfig();
		void PRIVATE_API deserializeConfig();

		void PRIVATE_API insertRow(const_ref<Row> row);

		const_ref<std::vector<Column>> PRIVATE_API columns() const;

		const_ptr<Column> PRIVATE_API getColumn(std::string_view columnName) const;
		ptr<Column> PRIVATE_API getColumn(std::string_view columnName);

		const_ref<Column> PRIVATE_API getColumn(int index) const;
		ref<Column> PRIVATE_API getColumn(int index);

		size_t getColumnIndex(std::string_view columnName) const;

		std::shared_ptr<RowConfiguration> PRIVATE_API makeSharedRowConfiguration() const;
		RowConfiguration PRIVATE_API makeRowConfiguration() const;

		TableIterator PRIVATE_API makeIterator();

	public: /// Interface
		std::string_view name() const;

	public: /// Constructors
		Table(std::string name, const_ref<std::filesystem::path> dataBaseDirectoryPath, std::vector<Column>&& columns);

		Table() = default;
		Table(const Table&) = delete;
		Table(Table&&) noexcept = default;
		Table& operator=(const Table&) = delete;
		Table& operator=(Table&&) noexcept = default;
		~Table() = default;

	private: /// Friends
		friend class Mango::MangoDB;
		friend class Mango::Queries::TruncateTableQuery;
		friend class Mango::Queries::InsertIntoQuery;
		friend class Mango::Queries::SelectQuery;
		friend class Mango::Queries::DeleteQuery;
		friend std::ostream& operator<<(std::ostream& out, const Table& table);
	};
}
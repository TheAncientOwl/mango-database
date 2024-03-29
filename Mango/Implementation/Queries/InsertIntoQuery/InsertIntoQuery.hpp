#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_INSERT_INTO_SYNTAX "INSERT INTO table_name [col1, col2, ...] VALUES (val1, val2, ...);"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// INSERT INTO table_name [col1, col2, ...] VALUES (val1, val2, ...);
	/// [!] If inserted string containts ',' it must be escaped with '/': "/,"
	/// </summary>
	class InsertIntoQuery final : public AbstractQuery
	{
	private:
		bool m_InsertAll = false;
		std::string m_TableName;
		std::vector<std::string> m_ColumnNames;
		std::vector<std::string> m_ColumnValues;

	private:
		/// <summary>
		/// [] before ()
		/// </summary>
		MANGO_API void checkStatementsOrder(Statement columns, Statement values, Statement::iterator defaultIt) const;

		/// <summary>
		/// allowed: VALUES after [] optionally.
		/// </summary>
		MANGO_API void checkResidualParts(Statement columns, Statement values, Statement::iterator stringEnd) const;

		/// <summary>
		/// INSERT INTO table_name
		/// </summary>
		MANGO_API void parseTableName(std::string_view firstPart);

		/// <summary>
		/// col1, col2, col3, ...
		/// </summary>
		MANGO_API void parseColumnNames(std::string_view columnsPart);

		/// <summary>
		/// val1, val2, val3, ...
		/// </summary>
		MANGO_API void parseColumnValues(std::string_view valuesPart);

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API InsertIntoQuery() = default;
		MANGO_PUBLIC_API InsertIntoQuery(const InsertIntoQuery&) = default;
		MANGO_PUBLIC_API InsertIntoQuery(InsertIntoQuery&&) noexcept = default;
		MANGO_PUBLIC_API InsertIntoQuery& operator=(const InsertIntoQuery&) = default;
		MANGO_PUBLIC_API InsertIntoQuery& operator=(InsertIntoQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~InsertIntoQuery() = default;
	};
}
#pragma once
#include "../../SelectQuery/SelectQuery.hpp"
#include "../WhereCondition/WhereCondition.hpp"
#define MANGO_SELECT_CLI_SYNTAX "SELECT */[col1, col2, ...] FROM (table_name) WHERE <col = value>;"

namespace Mango::Queries::CommandLineAdapter
{
	/// <summary>
	/// SELECT */[col1, col2, ...] FROM (table_name) WHERE <col = value>;
	/// </summary>
	class SelectQueryCLI final : public SelectQuery
	{
	private:
		WhereCondition m_WhereCondition;

	private:
		/// <summary>
		/// [] before () before <>
		/// </summary>
		MANGO_PRIVATE_API void checkStatementsOrder(Statement columns, Statement table, Statement condition, Statement::iterator defaultIt) const;
		
		/// <summary>
		/// SELECT [...] FROM (...) WHERE <...>;
		/// </summary>
		MANGO_PRIVATE_API void checkResidualParts(Statement columns, Statement table, Statement condition, std::string_view sql) const;

	private:
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API SelectQueryCLI() = default;
		MANGO_PUBLIC_API SelectQueryCLI(const SelectQueryCLI&) = default;
		MANGO_PUBLIC_API SelectQueryCLI(SelectQueryCLI&&) noexcept = default;
		MANGO_PUBLIC_API SelectQueryCLI& operator=(const SelectQueryCLI&) = default;
		MANGO_PUBLIC_API SelectQueryCLI& operator=(SelectQueryCLI&&) noexcept = default;
		MANGO_PUBLIC_API ~SelectQueryCLI() = default;
	};
}
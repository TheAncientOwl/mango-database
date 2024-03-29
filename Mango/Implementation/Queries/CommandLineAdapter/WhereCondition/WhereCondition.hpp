#pragma once
#include "../../../Row/RowFilters.hpp"

namespace Mango::Implementation::Queries::CommandLineAdapter
{
	class WhereCondition
	{
	private:
		std::vector<std::string> m_Args;
		std::string m_ColumnName;
		std::string m_Operation;
		std::string m_Value;

		WhereClause m_Clause;

	public:
		WhereClause getClause() const;
		void setClause(WhereClause clause);

		bool empty() const;

		void clear();

		void parseFrom(std::string_view condition);

		void validate(std::string_view tableName, const_ref<MangoDB> dataBase);

		WhereCondition() = default;
		WhereCondition(const WhereCondition&) = default;
		WhereCondition(WhereCondition&&) noexcept = default;
		WhereCondition& operator=(const WhereCondition&) = default;
		WhereCondition& operator=(WhereCondition&&) noexcept = default;
		~WhereCondition() = default;
	};
}
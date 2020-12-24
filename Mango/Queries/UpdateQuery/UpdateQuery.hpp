#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_UPDATE_SYNTAX "UPDATE table_name;"

namespace Mango::Queries
{
	/// <summary>
	/// UPDATE table_name;
	/// [!] Where clause can be specified through MagoDB::setWhereClause(WhereClause whereClause);
	/// [!] By default all values are updated.
	/// [!] Set clause can be specified through MangoDB::setSetClause(SetClause setClause);
	/// [!] By default nothing will be modified.
	/// </summary>
	class UpdateQuery final : public AbstractQuery
	{
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_INTERFACE bool match(std::string_view sql) const override;
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API UpdateQuery() = default;
		MANGO_PUBLIC_API UpdateQuery(const UpdateQuery&) = default;
		MANGO_PUBLIC_API UpdateQuery(UpdateQuery&&) noexcept = default;
		MANGO_PUBLIC_API UpdateQuery& operator=(const UpdateQuery&) = default;
		MANGO_PUBLIC_API UpdateQuery& operator=(UpdateQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~UpdateQuery() = default;
	};
}
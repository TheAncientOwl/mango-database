#pragma once
#include "../MangoAPI.hpp"

namespace test
{
	using namespace Mango;

	std::ostream& operator<<(std::ostream& out, const Column& col);

	void columnSerializeDeserialize();
	
}
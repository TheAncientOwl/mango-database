#pragma once
#include "../MangoAPI.hpp"

namespace Mango
{
	using namespace Queries;
	using namespace Exceptions;

	class CommandLineInterface
	{
	private:
		std::filesystem::path m_DataBaseDirectoryPath;
		static const std::array<std::unique_ptr<AbstractQuery>, 7> s_Queries;

	public:
		void run();

		MANGO_PUBLIC_API CommandLineInterface(std::string_view dataBaseDirectoryPath);

		MANGO_PUBLIC_API CommandLineInterface(const CommandLineInterface&) = delete;
		MANGO_PUBLIC_API CommandLineInterface(CommandLineInterface&&) noexcept = default;
		MANGO_PUBLIC_API CommandLineInterface& operator=(const CommandLineInterface&) = delete;
		MANGO_PUBLIC_API CommandLineInterface& operator=(CommandLineInterface&&) noexcept = default;
		MANGO_PUBLIC_API ~CommandLineInterface() = default;
	};
}
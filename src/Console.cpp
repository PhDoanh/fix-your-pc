#include "Console.hpp"

Console console;
std::time_t now = time(0);
std::tm *ltm = std::localtime(&now);

Console::Console()
{
	file.open("res/dev_data.txt", std::ios::trunc | std::ios::out | std::ios::ate);
	if (!file.is_open())
		error("Could not open default log file (dev data)!");
}

Console::~Console()
{
	file.close();
}

void Console::log(const std::string &txt)
{
	if (txt == "logged!")
		std::clog << "    Log| " << txt << '\n';
	else
	{
		std::clog << "    Log| " << txt << '\n';
		file << ltm->tm_hour << ':' << ltm->tm_min << ':' << ltm->tm_sec << "| " << txt << '\n';
	}
}

void Console::info(const std::string &txt)
{
	std::clog << "   Info| " << txt << '\n';
}

void Console::warn(const std::string &txt)
{
	std::cout << "Warning| " << txt << '\n';
}

void Console::error(const std::string &txt)
{
	std::cerr << "  Error| " << txt << '\n';
	exit(1);
}
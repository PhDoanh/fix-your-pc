#include <bits/stdc++.h>
#include <../inc/SDL.h>

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

class Console
{
private:
	std::fstream file;

public:
	Console();	// constructor
	~Console(); // destructor

	// a member function
	void log(const std::string &txt = "logged!");		// logging data (saved)
	void info(const std::string &txt = "information!"); // show messages (not saved)
	void warn(const std::string &txt = "warning!");		// show warnings
	void error(const std::string &txt = "error!");		// show errors (stop program)
};

extern Console console;

#endif // CONSOLE_HPP
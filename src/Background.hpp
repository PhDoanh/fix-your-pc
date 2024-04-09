#include <string>

#ifndef BG_HPP
#define BG_HPP

class Background
{
public:
	int x, y;
	float speed;

	Background();	   // constructor
	~Background();	   // destructor
	void move();	   // a member function
};

extern Background *bg;

#endif // BG_HPP
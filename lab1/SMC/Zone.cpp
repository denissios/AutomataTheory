#include "Zone.h"

Zone::Zone()
{
	count = 1;
	str = "";
}

void Zone::addSymbol(const char ch)
{
	str += ch;
	count++;
}

void Zone::reset()
{
	count = 1;
	str = "";
}
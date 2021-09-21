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
#include "BS.h"

void BS::stoupper(std::string& s)
{
    std::string::iterator i = s.begin();
    std::string::iterator end = s.end();

    while (i != end) {
        *i = std::toupper((unsigned char)*i);
        ++i;
    }
}
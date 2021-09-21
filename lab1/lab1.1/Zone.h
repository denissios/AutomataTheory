#ifndef __ZONE_HPP_INCLUDED__
#define __ZONE_HPP_INCLUDED__

#include <string>

class Zone {
private:
    size_t count;
    std::string str;

public:
    Zone();
    void addSymbol(const char ch);
    [[nodiscard]] inline constexpr bool isCorrect() { return count > 4 ? false : true; }

};

#endif //__ZONE_HPP_INCLUDED__
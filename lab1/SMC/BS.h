#ifndef __TC_HPP_INCLUDED__
#define __TC_HPP_INCLUDED__

#include <string>

class BS {
private:
    std::string str;
    void stoupper(std::string& s);

public:
    BS() { str = ""; }
    inline void reset() { str = ""; }
    inline void addSymbol(const char ch) { str += ch; }
    [[nodiscard]] inline bool isCorrect() { stoupper(str); return (str == "GTALK:TALK?JID=" || str == "GTALK:CHAT?JID=") ? true : false; }

};

#endif //__TC_HPP_INCLUDED__
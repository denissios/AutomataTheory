#ifndef __SERVER_HPP_INCLUDED__
#define __SERVER_HPP_INCLUDED__

#include <string>

class Server {
private:
    std::string str;

public:
    Server() { str = ""; }
    inline void reset() { str = ""; }
    inline void addSymbol(const char ch) { str += ch; }
    inline std::string getStr() { return str; }

};

#endif //__SERVER_HPP_INCLUDED__
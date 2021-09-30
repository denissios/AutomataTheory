#ifndef __APPCLASS_HPP_INCLUDED__
#define __APPCLASS_HPP_INCLUDED__

#include <map>
#include "AppClass_sm.h"
#include "Zone.h"
#include "Server.h"


#ifdef CRTP
class AppClass : public AppClassContext<AppClass>
#else
class AppClass
#endif
{
private:
#ifndef CRTP
    AppClassContext _fsm;
#endif

    Zone zone;
    Server server;
    bool isAcceptable;

public:
    AppClass();

    ~AppClass() {};

    [[nodiscard]] inline Zone getZone() { return zone; }

    inline void addZoneSymbol(char ch)   { zone.addSymbol(ch); }
    inline void addServerSymbol(char ch) { server.addSymbol(ch); }

    void reset();
    bool CheckString(std::string, std::map<std::string, size_t>& server_map);

    inline void Acceptable()   { isAcceptable = true; };
    inline void Unacceptable() { isAcceptable = false; };
};

#endif //__APPCLASS_HPP_INCLUDED__

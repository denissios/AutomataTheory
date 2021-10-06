#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

#include "AppClass.h"

AppClass::AppClass()
#ifdef CRTP
    : isAcceptable(false)
#else
    : _fsm(*this),
    isAcceptable(false)
#endif
{
#ifdef FSM_DEBUG
#ifdef CRTP
    setDebugFlag(true);
#else
    _fsm.setDebugFlag(true);
#endif
#endif
}

void AppClass::reset()
{
    zone.reset();
    server.reset();
    bs.reset();
}

bool AppClass::CheckString(std::string theString, std::map<std::string, size_t>& server_map)
{
#ifdef CRTP
#else
    _fsm.enterStartState();
    size_t i = 0;

    while (i != theString.size())
    {   
        switch (theString[i])
        {
        case '?':
            _fsm.letter_question(theString[i]);
            break;

        case '=':
            _fsm.letter_equals(theString[i]);
            break;

        case ':':
            _fsm.letter_colon(theString[i]);
            break;

        case '@':
            _fsm.letter_at();
            break;

        case '.':
            _fsm.letter_point();
            break;

        case ' ':
            _fsm.letter_space();
            break;

        case '\t':
            _fsm.letter_tab();
            break;

        default:
            if ((theString[i] >= 'a' && theString[i] <= 'z') || (theString[i] >= 'A' && theString[i] <= 'Z')) {
                _fsm.a_z(theString[i]);
                break;
            }
            else if (theString[i] >= '0' && theString[i] <= '9') {
                _fsm.a_zAND0_9(theString[i]);
                break;
            }
            _fsm.other();
            break;
        }

        ++i;
    }

    // end of string has been reached - send the EOS transition.
    _fsm.EOS();
#endif

    if (isAcceptable) {
        bool isAdd = false;
        for (auto& [key, value] : server_map) {
            if (key == server.getStr()) {
                value++;
                isAdd = true;
                break;
            }
        }

        if(!isAdd)
            server_map.emplace(server.getStr(), 1);
    }

    return isAcceptable;
}

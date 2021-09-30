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
}

bool AppClass::CheckString(std::string theString, std::map<std::string, size_t>& server_map)
{
#ifdef CRTP
#else
    _fsm.enterStartState();
    size_t i = 0, n = 0;

    while (i != theString.size())
    {   
        switch (n)
        {
        case 0:
            switch (theString[i])
            {
            case ':':
                _fsm.letter_colon();
                break;

            case '?':
                _fsm.letter_question();
                break;

            case '=':
                if (n == 0)
                    n++;
                _fsm.letter_equals();
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
            
            case 'T':
            case 't':
                _fsm.letter_t();
                break;

            case 'A':
            case 'a':
                _fsm.letter_a();
                break;
    
            case 'L':
            case 'l':
                _fsm.letter_l();
                break;

            case 'K':
            case 'k':
                _fsm.letter_k();
                break;

            case 'C':
            case 'c':
                _fsm.letter_c();
                break;

            case 'H':
            case 'h':
                _fsm.letter_h();
                break;

            case 'J':
            case 'j':
                _fsm.letter_j();
                break;

            case 'I':
            case 'i':
                _fsm.letter_i();
                break;

            case 'D':
            case 'd':
                _fsm.letter_d();
                break;

            default:
                _fsm.other();
                break;
            }
            break;

        case 1:
            if ((theString[i] >= 'a' && theString[i] <= 'z') || (theString[i] >= 'A' && theString[i] <= 'Z')) {
                _fsm.a_z(theString[i]);
                break;
            }
            else if (theString[i] >= '0' && theString[i] <= '9') {
                _fsm.a_zAND0_9(theString[i]);
                break;
            }

            switch (theString[i])
            {
            case '@':
                _fsm.letter_at();
                break;

            case '.':
                _fsm.letter_point();
                break;

            default:
                _fsm.other();
                break;
            }
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

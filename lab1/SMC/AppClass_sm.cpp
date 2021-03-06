//
// ex: set ro:
// DO NOT EDIT.
// generated by smc (http://smc.sourceforge.net/)
// from file : AppClass.sm
//


//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of
// the License at http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an "AS
// IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
// 
// The Original Code is State Machine Compiler (SMC).
// 
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2003 Charles W. Rapp.
// All Rights Reserved.
//
// Contributor(s): 
//
// State Map
//	This state map is recognizes the regular expression 0*1*.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.5  2005/05/28 13:31:16  cwrapp
// Updated C++ examples.
//
// Revision 1.1  2005/02/21 14:19:18  charlesr
// Added spaces around header file name to test SMC.
//
// Revision 1.0  2003/12/14 19:11:57  charlesr
// Initial revision
//


#include "AppClass.h"
#include "../../lab1/lab1.1\AppClass_sm.h"

using namespace statemap;

// Static class declarations.
Map1_Start Map1::Start("Map1::Start", 0);
Map1_BS Map1::BS("Map1::BS", 1);
Map1_at Map1::at("Map1::at", 2);
Map1_user Map1::user("Map1::user", 3);
Map1_point Map1::point("Map1::point", 4);
Map1_server Map1::server("Map1::server", 5);
Map1_eos Map1::eos("Map1::eos", 6);
Map1_zone Map1::zone("Map1::zone", 7);
Map1_OK Map1::OK("Map1::OK", 8);
Map1_Error Map1::Error("Map1::Error", 9);

void AppClassState::EOS(AppClassContext& context)
{
    Default(context);
}

void AppClassState::a_z(AppClassContext& context, char& ch)
{
    Default(context);
}

void AppClassState::a_zAND0_9(AppClassContext& context, char& ch)
{
    Default(context);
}

void AppClassState::letter_at(AppClassContext& context)
{
    Default(context);
}

void AppClassState::letter_colon(AppClassContext& context, char& ch)
{
    Default(context);
}

void AppClassState::letter_equals(AppClassContext& context, char& ch)
{
    Default(context);
}

void AppClassState::letter_point(AppClassContext& context)
{
    Default(context);
}

void AppClassState::letter_question(AppClassContext& context, char& ch)
{
    Default(context);
}

void AppClassState::letter_space(AppClassContext& context)
{
    Default(context);
}

void AppClassState::letter_tab(AppClassContext& context)
{
    Default(context);
}

void AppClassState::other(AppClassContext& context)
{
    Default(context);
}

void AppClassState::Default(AppClassContext& context)
{
    throw (
        TransitionUndefinedException(
            context.getState().getName(),
            context.getTransition()));

}

void Map1_Default::other(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Default::letter_space(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Default::letter_tab(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Start::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_Start::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.reset();
        ctxt.addBSSymbol(ch);
        context.setState(Map1::BS);
    }
    catch (...)
    {
        context.setState(Map1::BS);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_Start::a_zAND0_9(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Start::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Start::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Start::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Start::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Start::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_BS::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_BS::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addBSSymbol(ch);
        context.setState(Map1::BS);
    }
    catch (...)
    {
        context.setState(Map1::BS);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_BS::a_zAND0_9(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_BS::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_BS::letter_colon(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addBSSymbol(ch);
        context.setState(Map1::BS);
    }
    catch (...)
    {
        context.setState(Map1::BS);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_BS::letter_equals(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addBSSymbol(ch);
        context.setState(Map1::at);
    }
    catch (...)
    {
        context.setState(Map1::at);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_BS::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_BS::letter_question(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addBSSymbol(ch);
        context.setState(Map1::BS);
    }
    catch (...)
    {
        context.setState(Map1::BS);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_at::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_at::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    if (ctxt.getBS().isCorrect())
    {
        context.getState().Exit(context);
        // No actions.
        context.setState(Map1::user);
        context.getState().Entry(context);
    }
    else
    {
        context.getState().Exit(context);
        context.setState(Map1::Error);
        context.getState().Entry(context);
    }

}

void Map1_at::a_zAND0_9(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    if (ctxt.getBS().isCorrect())
    {
        context.getState().Exit(context);
        // No actions.
        context.setState(Map1::user);
        context.getState().Entry(context);
    }
    else
    {
        context.getState().Exit(context);
        context.setState(Map1::Error);
        context.getState().Entry(context);
    }

}

void Map1_at::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_at::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_at::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_at::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_at::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_user::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_user::a_z(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::user);
    context.getState().Entry(context);

}

void Map1_user::a_zAND0_9(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::user);
    context.getState().Entry(context);

}

void Map1_user::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::point);
    context.getState().Entry(context);

}

void Map1_user::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_user::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_user::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_user::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_point::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_point::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addServerSymbol(ch);
        context.setState(Map1::server);
    }
    catch (...)
    {
        context.setState(Map1::server);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_point::a_zAND0_9(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addServerSymbol(ch);
        context.setState(Map1::server);
    }
    catch (...)
    {
        context.setState(Map1::server);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_point::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_point::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_point::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_point::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_point::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_server::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_server::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addServerSymbol(ch);
        context.setState(Map1::server);
    }
    catch (...)
    {
        context.setState(Map1::server);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_server::a_zAND0_9(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addServerSymbol(ch);
        context.setState(Map1::server);
    }
    catch (...)
    {
        context.setState(Map1::server);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_server::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_server::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_server::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_server::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::eos);
    context.getState().Entry(context);

}

void Map1_server::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_eos::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_eos::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.addZoneSymbol(ch);
        context.setState(Map1::zone);
    }
    catch (...)
    {
        context.setState(Map1::zone);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_eos::a_zAND0_9(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_eos::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_eos::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_eos::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_eos::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_eos::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_zone::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Acceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_zone::a_z(AppClassContext& context, char& ch)
{
    AppClass& ctxt = context.getOwner();

    if (ctxt.getZone().isCorrect())
    {
        context.getState().Exit(context);
        context.clearState();
        try
        {
            ctxt.addZoneSymbol(ch);
            context.setState(Map1::zone);
        }
        catch (...)
        {
            context.setState(Map1::zone);
            throw;
        }
        context.getState().Entry(context);
    }
    else
    {
        context.getState().Exit(context);
        context.setState(Map1::Error);
        context.getState().Entry(context);
    }

}

void Map1_zone::a_zAND0_9(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_zone::letter_at(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_zone::letter_colon(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_zone::letter_equals(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_zone::letter_point(AppClassContext& context)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_zone::letter_question(AppClassContext& context, char& ch)
{

    context.getState().Exit(context);
    context.setState(Map1::Error);
    context.getState().Entry(context);

}

void Map1_Error::EOS(AppClassContext& context)
{
    AppClass& ctxt = context.getOwner();

    context.getState().Exit(context);
    context.clearState();
    try
    {
        ctxt.Unacceptable();
        context.setState(Map1::Start);
    }
    catch (...)
    {
        context.setState(Map1::Start);
        throw;
    }
    context.getState().Entry(context);

}

void Map1_Error::a_z(AppClassContext& context, char& ch)
{


}

void Map1_Error::a_zAND0_9(AppClassContext& context, char& ch)
{


}

void Map1_Error::letter_at(AppClassContext& context)
{


}

void Map1_Error::letter_colon(AppClassContext& context, char& ch)
{


}

void Map1_Error::letter_equals(AppClassContext& context, char& ch)
{


}

void Map1_Error::letter_point(AppClassContext& context)
{


}

void Map1_Error::letter_question(AppClassContext& context, char& ch)
{


}

//
// Local variables:
//  buffer-read-only: t
// End:
//

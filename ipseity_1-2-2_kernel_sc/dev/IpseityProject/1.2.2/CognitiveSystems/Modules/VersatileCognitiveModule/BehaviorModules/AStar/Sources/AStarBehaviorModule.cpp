/*#########################################################################

    MAIN: A* Artificial Intelligence Module

    ---------------------------------------------------------------------

    Author(s) : S. Le Guelvouit & J. Bonfort
    Creation Date : 2013/04/12
    Last Update : 2013/04/12

#########################################################################*/


#include "AStarBehaviorModule.h"

#include <QDebug>


AStarBehaviorModule::AStarBehaviorModule()
{
}


AStarBehaviorModule::~AStarBehaviorModule()
{
}


const char*
AStarBehaviorModule::getName() const
{
    return "A* Behavior Module";
}


const char*
AStarBehaviorModule::getDescription() const
{
    return "";
}


const char*
AStarBehaviorModule::getAuthors() const
{
    return "S. Le Guelvouit & J. Bonfort";
}


void
AStarBehaviorModule::attachToWidget( QWidget* )
{
}


void
AStarBehaviorModule::init( const char*, uint32 data_channel_id, AbstractResponseModule* )
{
    m_DataChannelId = data_channel_id;
}


void
AStarBehaviorModule::loadParameters( const char*, const char* )
{
}


const Response &
AStarBehaviorModule::selectResponse( const Stimulus & stimulus )
{
    return m_Response;
}

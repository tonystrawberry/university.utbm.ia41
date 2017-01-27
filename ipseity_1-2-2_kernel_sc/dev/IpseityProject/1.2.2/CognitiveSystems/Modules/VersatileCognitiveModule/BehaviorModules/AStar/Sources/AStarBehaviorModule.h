#ifndef AStarBehaviorModule_h
#define AStarBehaviorModule_h


#include "AL/AbstractVersatileBehaviorModule.h"


class AStarBehaviorModule : public AbstractVersatileBehaviorModule
{
    public:
        AStarBehaviorModule();
        ~AStarBehaviorModule();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_VERSATILE_BEHAVIOR_MODULE


    protected:
        float64* m_StimulusRawData;
        Response m_Response;

        uint32 m_DataChannelId;
};


#endif

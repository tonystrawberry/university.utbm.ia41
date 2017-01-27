#ifndef BinaryActionSearch_h
#define BinaryActionSearch_h


#include "AL/AbstractRLBehaviorModule.h"
//#include "IO/Plugin.h"
#include "AL/AbstractPolicy.h"
#include "AL/RLCognitiveSystem/Policy/AbstractContinuousPolicy.h"


class BinaryActionSearch : public AbstractContinuousPolicy
{
    public:
        BinaryActionSearch();
        ~BinaryActionSearch();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_POLICY
		INTERFACE_ABSTRACT_CONTINUOUS_POLICY


    protected:
        uint32 m_NbrDecisions;
		BinaryDecisionSequence m_BinaryDecisionSequence;

	private:
		ResponseProperties m_ResponseDescription;
		ResponseProperties m_BinaryDecisionDescription;
		AbstractPolicy* m_Policy;
		Response m_Response;
		Response m_ResponseMax;
		Response m_ResponseMin;
		IpseityPrecisionType m_Delta0;
};


#endif

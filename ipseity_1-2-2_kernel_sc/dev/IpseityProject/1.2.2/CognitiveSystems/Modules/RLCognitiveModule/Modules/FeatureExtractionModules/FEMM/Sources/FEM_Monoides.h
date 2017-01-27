#ifndef FEM_Monoides_h
#define FEM_Monoides_h


#include "AL/RLCognitiveSystem/FeatureExtractionModules/BaseFeatureExtractionModule.h"
#include "AL/IpseityTypes.h"
#include "Math/CombinationWithRepetition.h"

#include <vector>


class FEM_Monoides : public BaseFeatureExtractionModule
{
    public:
		FEM_Monoides();
		~FEM_Monoides();

        INTERFACE_ABSTRACT_PLUGIN
        IMPLEMENTABLE_FEATURE_EXTRACTION_MODULE_METHODS


    protected:
        uint32 m_MonoideDegree;
        CombinationWithRepetition m_CWR;
};


#endif

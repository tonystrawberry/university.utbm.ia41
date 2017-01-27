#ifndef RBF_h
#define RBF_h


#include "AL/RLCognitiveSystem/FeatureExtractionModules/BaseFeatureExtractionModule.h"
#include "AL/IpseityTypes.h"

#include <vector>


class RBF : public BaseFeatureExtractionModule
{
    public:
        RBF();
        ~RBF();

        INTERFACE_ABSTRACT_PLUGIN
        IMPLEMENTABLE_FEATURE_EXTRACTION_MODULE_METHODS

    protected:
        void prepare();
        void defineNRegularTilings();
        void defineNRandomTilings();


    protected:
        IpseityPrecisionType* m_CenterDatas;
        IpseityPrecisionType  m_Width;
        IpseityPrecisionType  m_Height;
        uint32 m_CenterDimension;
        uint32 m_CenterNbr;
};


#endif

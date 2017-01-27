#ifndef GSBFN_h
#define GSBFN_h


#include "AL/AbstractFeatureExtractionModule.h"
#include "AL/IpseityTypes.h"
#include "GSBFN_Grid.h"

#include <vector>


class GSBFN : public AbstractFeatureExtractionModule
{
    public:
        GSBFN();
        ~GSBFN();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_FEATURE_EXTRACTION_MODULE

    protected:
        bool extractFromStimulusOnly() const;
        void prepare();


    protected:
        SensoryProperties* m_StimulusDescription;
        ResponseProperties* m_ResponseDescription;
        AgentDataProperties* m_DataDescription;

        bool m_ExtractFromStimulusOnly;
        Vector<IpseityPrecisionType> m_SR;

        std::vector<GSBFN_Grid<IpseityPrecisionType>*> m_Grids;
        uint32 m_NbrCells;
        uint32 m_NbrTotalFeatures;
        uint32 m_ResponseIndex;

        IpseityPrecisionType m_EMax;
        IpseityPrecisionType m_MaxA;
        IpseityPrecisionType m_AMin;
};


#endif

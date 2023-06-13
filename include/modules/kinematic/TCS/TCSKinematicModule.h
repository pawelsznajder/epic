/*
 * TCSKinematicModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_TCS_TCSKINEMATICMODULE_H_
#define MODULES_KINEMATIC_TCS_TCSKINEMATICMODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../beans/containers/DDVCSKinematicRanges.h"
#include "../../../beans/containers/DDVCSKinematic.h"
#include "../../../beans/types/ParticleType.h"
#include "../KinematicModule.h"

namespace EPIC {

/**
 * @class TCSKinematicModule
 *
 * @brief Kinematic module for TCS.
 *
 * This class defines kinematic module for TCS process.
 */
class TCSKinematicModule: public KinematicModule<DDVCSKinematicRanges, DDVCSKinematic> {

public:
    static const std::string TCS_KINEMATIC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.
    static const std::string TCS_KINEMATIC_MODULE_PARAMETER_DECAY_TYPE; ///< String used to set decay lepton type through XML.

    /**
     * Constructor.
     */
    TCSKinematicModule(const std::string &className);

    /**
     * Copy constructor.
     */
    TCSKinematicModule(const TCSKinematicModule &other);

    /**
     * Destructor.
     */
    virtual ~TCSKinematicModule();

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual bool runTest() const;

    virtual std::vector<KinematicRange> getKinematicRanges(const ExperimentalConditions &conditions, const DDVCSKinematicRanges& ranges);

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DDVCSKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DDVCSKinematic &kin) = 0;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get decay lepton type.
     */
    ParticleType::Type getDecayType() const;

    /**
     * Set decay lepton type.
     */
    void setDecayType(ParticleType::Type decayType);

protected:
    virtual void initModule();
    virtual void isModuleWellConfigured();

    ParticleType::Type m_decayType; ///< Decay lepton type.
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_TCS_TCSKINEMATICMODULE_H_ */

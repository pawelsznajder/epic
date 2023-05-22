/*
 * DDVCSKinematicModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_DDVCS_DDVCSKINEMATICMODULE_H_
#define MODULES_KINEMATIC_DDVCS_DDVCSKINEMATICMODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../beans/containers/DDVCSKinematic.h"
#include "../../../beans/types/ParticleType.h"
#include "../KinematicModule.h"

namespace EPIC {

/**
 * @class DDVCSKinematicModule
 *
 * @brief Kinematic module for DDVCS.
 *
 * This class defines kinematic module for DDVCS process.
 */
class DDVCSKinematicModule: public KinematicModule<DDVCSKinematic> {

public:
    static const std::string DDVCS_KINEMATIC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.
    static const std::string DDVCS_KINEMATIC_MODULE_PARAMETER_DECAY_TYPE; ///< String used to set decay lepton type through XML.

    /**
     * Constructor.
     */
    DDVCSKinematicModule(const std::string &className);

    /**
     * Copy constructor.
     */
    DDVCSKinematicModule(const DDVCSKinematicModule &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSKinematicModule();

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual bool runTest() const;

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

#endif /* MODULES_KINEMATIC_DDVCS_DDVCSKINEMATICMODULE_H_ */

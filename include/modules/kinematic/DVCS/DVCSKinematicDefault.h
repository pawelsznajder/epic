/*
 * DVCSKinematicDefault.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_KINEMATIC_DVCS_DVCSKINEMATICDEFAULT_H_
#define MODULES_KINEMATIC_DVCS_DVCSKINEMATICDEFAULT_H_

#include <string>

#include "DVCSKinematicModule.h"

namespace EPIC {

/**
 * @class DVCSKinematicDefault
 *
 * @brief Kinematic module to determine four-momenta from DVCS kinematics.
 *
 * This class defines kinematic module for DVCS process. That is, four-momenta
 * of particles and the signature of event are defined here from kinematics and
 * kinematic conditions.
 */
class DVCSKinematicDefault: public DVCSKinematicModule {

public:
    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DVCSKinematicDefault(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSKinematicDefault(const DVCSKinematicDefault &other);

    /**
     * Destructor.
     */
    virtual ~DVCSKinematicDefault();

    virtual DVCSKinematicDefault *clone() const;

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DVCSKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DVCSKinematic &kin);
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_DVCS_DVCSKINEMATICDEFAULT_H_ */

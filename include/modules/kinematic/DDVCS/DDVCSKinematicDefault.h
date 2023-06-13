/*
 * DDVCSKinematicDefault.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_DDVCS_DDVCSKINEMATICDEFAULT_H_
#define MODULES_KINEMATIC_DDVCS_DDVCSKINEMATICDEFAULT_H_

#include <string>

#include "DDVCSKinematicModule.h"

namespace EPIC {

/**
 * @class DDVCSKinematicDefault
 *
 * @brief Kinematic module to determine four-momenta from DDVCS kinematics.
 *
 * This class defines kinematic module for DDVCS process. That is, four-momenta
 * of particles and the signature of event are defined here from kinematics and
 * kinematic conditions.
 */
class DDVCSKinematicDefault: public DDVCSKinematicModule {

public:
    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DDVCSKinematicDefault(const std::string &className);

    /**
     * Copy constructor.
     */
    DDVCSKinematicDefault(const DDVCSKinematicDefault &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSKinematicDefault();

    virtual DDVCSKinematicDefault *clone() const;

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DDVCSKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DDVCSKinematic &kin);
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_DDVCS_DDVCSKINEMATICDEFAULT_H_ */

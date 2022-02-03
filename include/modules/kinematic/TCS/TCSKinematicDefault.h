/*
 * TCSKinematicDefault.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_TCS_TCSKINEMATICDEFAULT_H_
#define MODULES_KINEMATIC_TCS_TCSKINEMATICDEFAULT_H_

#include <string>

#include "TCSKinematicModule.h"

namespace EPIC {

/**
 * @class TCSKinematicDefault
 *
 * @brief Kinematic module to determine four-momenta from TCS kinematics.
 *
 * This class defines kinematic module for TCS process. That is, four-momenta
 * of particles and the signature of event are defined here from kinematics and
 * kinematic conditions.
 */
class TCSKinematicDefault: public TCSKinematicModule {

public:
    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    TCSKinematicDefault(const std::string &className);

    /**
     * Copy constructor.
     */
    TCSKinematicDefault(const TCSKinematicDefault &other);

    /**
     * Destructor.
     */
    virtual ~TCSKinematicDefault();

    virtual TCSKinematicDefault *clone() const;

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DDVCSKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DDVCSKinematic &kin);
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_TCS_TCSKINEMATICDEFAULT_H_ */

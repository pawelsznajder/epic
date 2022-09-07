/*
 * GAM2KinematicDefault.h
 *
 *  Created on: Aug 24, 2022
 *      Author: Bartosz Skura (PW)
 */

#ifndef MODULES_KINEMATIC_GAM2_GAM2KINEMATICDEFAULT_H_
#define MODULES_KINEMATIC_GAM2_GAM2KINEMATICDEFAULT_H_

#include <string>
#include <memory>

#include "GAM2KinematicModule.h"

namespace EPIC {
class RandomNumberModule;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class GAM2KinematicDefault
 *
 * @brief Kinematic module to determine four-momenta from GAM2 kinematics.
 *
 * This class defines kinematic module for GAM2 process. That is, four-momenta
 * of particles and the signature of event are defined here from kinematics and
 * kinematic conditions.
 */
class GAM2KinematicDefault: public GAM2KinematicModule {

public:
    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    GAM2KinematicDefault(const std::string &className);

    /**
     * Copy constructor.
     */
    GAM2KinematicDefault(const GAM2KinematicDefault &other);

    /**
     * Destructor.
     */
    virtual ~GAM2KinematicDefault();

    virtual GAM2KinematicDefault *clone() const;

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const GAM2Kinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const GAM2Kinematic &kin);

private:

    std::shared_ptr<RandomNumberModule> m_randomNumberModule; ///< Random number module.
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_GAM2_GAM2KINEMATICDEFAULT_H_ */

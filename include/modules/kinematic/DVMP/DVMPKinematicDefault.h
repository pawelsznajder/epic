/*
 * DVMPKinematicDefault.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_KINEMATIC_DVMP_DVMPKINEMATICDEFAULT_H_
#define MODULES_KINEMATIC_DVMP_DVMPKINEMATICDEFAULT_H_

#include <memory>
#include <string>

#include "DVMPKinematicModule.h"

namespace EPIC {
class RandomNumberModule;
} /* namespace EPIC */

namespace EPIC {
class Particle;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class DVMPKinematicDefault
 *
 * @brief Kinematic module to determine four-momenta from DVMP kinematics.
 *
 * This class defines kinematic module for DVMP process. That is, four-momenta
 * of particles and the signature of event are defined here from kinematics and
 * kinematic conditions.
 */
class DVMPKinematicDefault: public DVMPKinematicModule {

public:
    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DVMPKinematicDefault(const std::string &className);

    /**
     * Copy constructor.
     */
    DVMPKinematicDefault(const DVMPKinematicDefault &other);

    /**
     * Destructor.
     */
    virtual ~DVMPKinematicDefault();

    virtual DVMPKinematicDefault *clone() const;

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DVMPKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DVMPKinematic &kin);

private:

    /**
     * Simulate decay.
     */
    void simulateDecay(Event& event);

    /**
     * Simulate pi0 decay.
     */
    void simulateDecayPi0(Event& event, std::shared_ptr<Particle> pi0);

    /**
     * Simulate J/Psi decay.
     */
    void simulateDecayJPsi(Event& event, std::shared_ptr<Particle> jpsi);

    /**
     * Distribution of phi of muon in J/Psi decay.
     */
    double WPhi(double x);

    /**
     * Distribution of theta of muon in J/Psi decay.
     */
    double WTheta(double x);

    /**
     * Get random phi from the WPhi distribution ranging from 0 to range using the Von Neumann method.
     */
    double VonNeumannPhi(double range);

    /**
     * Get random theta from the WTheta distribution ranging from 0 to range using the Von Neumann method.
     */
    double VonNeumannTheta(double range);

    std::shared_ptr<RandomNumberModule> m_randomNumberModule; ///< Random number module.

};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_DVMP_DVMPKINEMATICDEFAULT_H_ */

#ifndef BEANS_CONTAINERS_TCS_KINEMATIC_H
#define BEANS_CONTAINERS_TCS_KINEMATIC_H

/**
 * @file TCSObservableKinematic.h
 * @author: Pawel Sznajder (NCBJ)
 * @date 19 June 2021
 * @version 1.0
 */

#include "DDVCSKinematic.h"

namespace PARTONS {
class TCSObservableKinematic;
} /* namespace PARTONS */

namespace EPIC {

/**
 * @class TCSKinematic
 *
 * @brief Class representing single observable kinematics for TCS process.
 *
 * This class represents a single observable kinematics for TCS process
 * (y, \f$Q^{2}\f$, t, \f$Q'^{2}\f$, E_{b}, \f$\phi_{S}\f$, \f$\phi_{L}\f$, \f$\theta_{L}\f$).
 */
class TCSKinematic: public DDVCSKinematic {

public:

    /**
     * Default constructor.
     */
    TCSKinematic();

    /**
     * Assignment constructor.
     */
    TCSKinematic(double y, double Q2, double t, double Q2Prim, double E,
            double phiS, double phiL, double thetaL);

    /**
     * Copy constructor.
     */
    TCSKinematic(const TCSKinematic &other);

    /**
     * Destructor.
     */
    virtual ~TCSKinematic();

    virtual std::string toString() const;

    /**
     * Convert to PARTONS::TCSObservableKinematic.
     */
    PARTONS::TCSObservableKinematic toPARTONSTCSObservableKinematic() const;

    PARTONS::DDVCSObservableKinematic toPARTONSDDVCSObservableKinematic() const = delete;
    double getPhi() const = delete;
    void setPhi(double phi) = delete;
};

}

#endif /* BEANS_CONTAINERS_TCS_KINEMATIC_H */

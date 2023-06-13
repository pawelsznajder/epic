#ifndef BEANS_CONTAINERS_DVCS_KINEMATIC_H
#define BEANS_CONTAINERS_DVCS_KINEMATIC_H

/**
 * @file DVCSObservableKinematic.h
 * @author: Pawel Sznajder (NCBJ)
 * @date 19 June 2021
 * @version 1.0
 */

#include "DDVCSKinematic.h"

namespace PARTONS {
class DVCSObservableKinematic;
} /* namespace PARTONS */

namespace EPIC {

/**
 * @class DVCSKinematic
 *
 * @brief Class representing single observable kinematics for DVCS process.
 *
 * This class represents a single observable kinematics for DVCS process
 * (y, \f$Q^{2}\f$, t, \f$Q'^{2}\f$, E_{b}, \f$\phi\f$, \f$\phi_{S}\f$).
 */
class DVCSKinematic: public DDVCSKinematic {

public:

    /**
     * Default constructor.
     */
    DVCSKinematic();

    /**
     * Assignment constructor.
     */
    DVCSKinematic(double y, double Q2, double t, double E,
            double phi, double phiS);

    /**
     * Copy constructor.
     */
    DVCSKinematic(const DVCSKinematic &other);

    /**
     * Destructor.
     */
    virtual ~DVCSKinematic();

    virtual std::string toString() const;

    /**
     * Convert to PARTONS::DVCSObservableKinematic.
     */
    PARTONS::DVCSObservableKinematic toPARTONSDVCSObservableKinematic() const;

    PARTONS::DDVCSObservableKinematic toPARTONSDDVCSObservableKinematic() const = delete;
    double getQ2Prim() const = delete;
    void setQ2Prim(double phi) = delete;
    double getPhiL() const = delete;
    void setPhiL(double phi) = delete;
    double getThetaL() const = delete;
    void setThetaL(double phi) = delete;
};

}

#endif /* BEANS_CONTAINERS_DVCS_KINEMATIC_H */

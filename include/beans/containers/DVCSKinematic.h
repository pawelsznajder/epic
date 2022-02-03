#ifndef BEANS_CONTAINERS_DVCS_KINEMATIC_H
#define BEANS_CONTAINERS_DVCS_KINEMATIC_H

/**
 * @file DVCSObservableKinematic.h
 * @author: Pawel Sznajder (NCBJ)
 * @date 19 June 2021
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace PARTONS {
class DVCSObservableKinematic;
} /* namespace PARTONS */

namespace EPIC {

/**
 * @class DVCSKinematic
 *
 * @brief Class representing single observable kinematics for DVCS process.
 *
 * This class represents a single observable kinematics for DVCS process (\f$x_{B}\f$, t, \f$Q^{2}\f$, t, E_{b}, \f$\phi\f$, \f$\phi_{S}\f$).
 */
class DVCSKinematic: public PARTONS::BaseObject {

public:

    /**
     * Default constructor.
     */
    DVCSKinematic();

    /**
     * Assignment constructor.
     */
    DVCSKinematic(double xB, double t, double Q2, double E, double phi,
            double phiS);

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

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get Bjorken variable.
     */
    double getXB() const;

    /**
     * Set Bjorken variable.
     */
    void setXB(double xB);

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    double getT() const;

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t);

    /**
     * Get incoming virtual-photon virtuality.
     */
    double getQ2() const;

    /**
     * Set incoming virtual-photon virtuality.
     */
    void setQ2(double Q2);

    /**
     * Get beam energy.
     */
    double getE() const;

    /**
     * Set beam energy.
     */
    void setE(double E);

    /**
     * Get angle between leptonic and hadronic planes (in radians).
     */
    double getPhi() const;

    /**
     * Set angle between leptonic and hadronic planes (in radians).
     */
    void setPhi(double phi);

    /**
     * Get angle between leptonic and transverse component of target polarization (in radians).
     */
    double getPhiS() const;

    /**
     * Set angle between leptonic and transverse component of target polarization (in radians).
     */
    void setPhiS(double phiS);

private:

    /**
     * Bjorken variable.
     */
    double m_xB;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Initial virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    double m_Q2;

    /**
     * Beam energy (in GeV).
     */
    double m_E;

    /**
     * Angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    double m_phi;

    /**
     * Angle between leptonic and transverse component of target polarization (in radians).
     */
    double m_phiS;
};

}

#endif /* BEANS_CONTAINERS_DVCS_KINEMATIC_H */

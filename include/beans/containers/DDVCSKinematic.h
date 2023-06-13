#ifndef BEANS_CONTAINERS_DDVCS_KINEMATIC_H
#define BEANS_CONTAINERS_DDVCS_KINEMATIC_H

/**
 * @file DDVCSObservableKinematic.h
 * @author: Pawel Sznajder (NCBJ)
 * @date 19 June 2021
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace PARTONS {
class DDVCSObservableKinematic;
} /* namespace PARTONS */

namespace EPIC {

/**
 * @class DDVCSKinematic
 *
 * @brief Class representing single observable kinematics for DDVCS process.
 *
 * This class represents a single observable kinematics for DDVCS process
 * (y, \f$Q^{2}\f$, t, \f$Q'^{2}\f$, E_{b}, \f$\phi\f$, \f$\phi_{S}\f$, \f$\phi_{L}\f$, \f$\theta_{L}\f$).
 */
class DDVCSKinematic: public PARTONS::BaseObject {

public:

    /**
     * Default constructor.
     */
    DDVCSKinematic();

    /**
     * Assignment constructor.
     */
    DDVCSKinematic(double y, double Q2, double t, double Q2Prim, double E,
            double phi, double phiS, double phiL, double thetaL);

    /**
     * Copy constructor.
     */
    DDVCSKinematic(const DDVCSKinematic &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSKinematic();

    virtual std::string toString() const;

    /**
     * Convert to PARTONS::DDVCSObservableKinematic.
     */
    PARTONS::DDVCSObservableKinematic toPARTONSDDVCSObservableKinematic() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get inelasticity.
     */
    double getY() const;

    /**
     * Set inelasticity.
     */
    void setY(double y);

    /**
     * Get incoming virtual-photon virtuality.
     */
    double getQ2() const;

    /**
     * Set incoming virtual-photon virtuality.
     */
    void setQ2(double Q2);

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    double getT() const;

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t);

    /**
     * Get outgoing virtual-photon virtuality.
     */
    double getQ2Prim() const;

    /**
     * Set outgoing virtual-photon virtuality.
     */
    void setQ2Prim(double Q2Prim);

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

    /**
     * Get azimuthal angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    double getPhiL() const;

    /**
     * Set azimuthal angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    void setPhiL(double thetaL);

    /**
     * Get polar angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    double getThetaL() const;

    /**
     * Set polar angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    void setThetaL(double thetaL);

protected:

    /**
     * Inelasticity.
     */
    double m_y;

    /**
     * Initial virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    double m_Q2;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    double m_Q2Prim;

    /**
     * Beam energy (in GeV).
     */
    double m_E;

    /**
     * Angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    double m_phi;

    /**
     * Angle between leptonic and transverse component of target polarization (in radians, Trento convention).
     */
    double m_phiS;

    /**
     * Azimuthal angle between positively charged lepton and scattered proton in lepton CMS (in radians, BDP convention).
     */
    double m_phiL;

    /**
     * Polar angle between positively charged lepton and scattered proton in lepton CMS (in radians, BDP convention).
     */
    double m_thetaL;
};

}

#endif /* BEANS_CONTAINERS_DDVCS_KINEMATIC_H */

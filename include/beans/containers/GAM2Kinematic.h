#ifndef BEANS_CONTAINERS_GAM2_KINEMATIC_H
#define BEANS_CONTAINERS_GAM2_KINEMATIC_H

/**
 * @file GAM2ObservableKinematic.h
 * @author: Pawel Sznajder (NCBJ)
 * @date 19 June 2021
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace PARTONS {
class GAM2ObservableKinematic;
} /* namespace PARTONS */

namespace EPIC {

/**
 * @class GAM2Kinematic
 *
 * @brief Class representing single observable kinematics for GAM2 process.
 *
 * This class represents a single observable kinematics for GAM2 process (\f$x_{B}\f$, t, \f$Q^{2}\f$, t, E_{b}, \f$\phi\f$, \f$\phi_{S}\f$).
 */
class GAM2Kinematic: public PARTONS::BaseObject {

public:

    /**
     * Default constructor.
     */
    GAM2Kinematic();

    /**
     * Assignment constructor.
     */
    GAM2Kinematic(double y, double Q2, double t, double uPrim, double Mgg2,
            double E, double phi);

    /**
     * Copy constructor.
     */
    GAM2Kinematic(const GAM2Kinematic &other);

    /**
     * Destructor.
     */
    virtual ~GAM2Kinematic();

    virtual std::string toString() const;

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
     * Get initial virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    double getQ2() const;

    /**
     * Set initial virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    void setQ2(double Q2);

    /**
     * Get four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double getT() const;

    /**
     * Set four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    void setT(double t);

    /**
     * Get four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     */
    double getUPrim() const;

    /**
     * Set four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     */
    void setUPrim(double uPrim);

    /**
     * Get invariant mass of two photons (in \f$GeV^{2}\f$).
     */
    double getMgg2() const;

    /**
     * Set invariant mass of two photons (in \f$GeV^{2}\f$).
     */
    void setMgg2(double mgg2);

    /**
     * Get beam energy (in GeV).
     */
    double getE() const;

    /**
     * Set beam energy (in GeV).
     */
    void setE(double e);

    /**
     * Get angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    double getPhi() const;

    /**
     * Set angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    void setPhi(double phi);

    /**
     * Convert to PARTONS::GAM2ObservableKinematic.
     */
    PARTONS::GAM2ObservableKinematic toPARTONSGAM2ObservableKinematic() const;

private:

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
     * Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     */
    double m_uPrim;

    /**
     * Invariant mass of two photons (in \f$GeV^{2}\f$).
     */
    double m_Mgg2;

    /**
     * Beam energy (in GeV).
     */
    double m_E;

    /**
     * Angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    double m_phi;
};

}

#endif /* BEANS_CONTAINERS_GAM2_KINEMATIC_H */

#ifndef DVCS_CFF_MILOU3DTABLES_ARBITRARY_T_PROFILE_H
#define DVCS_CFF_MILOU3DTABLES_ARBITRARY_T_PROFILE_H

/**
 * @file DVCSCFFCMILOU3DTablesArbitraryTProfile.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date 06 April 2021
 * @version 1.0
 */

#include "DVCSCFFMILOU3DTables.h"

#include <TFormula.h>

namespace PARTONS {

/**
 * @class DVCSCFFCMILOU3DTablesArbitraryTProfile
 *
 * @brief Module for evaluation of CFF values from lookup tables with arbitrary t-profile.
 *
 * Module for evaluation of CFF values from lookup tables (xB, t, Q2) with arbitrary t-profile.
 *
 * The module works as follows. The imaginary part of CFF H is evaluated from DVCSCFFCMILOU3DTables at t=0. Then,
 * the result is multiplied by the (x,t)-dependent profile function, where the code assumes xi = x.
 * The real part of CFF H and other GPDs are set to 0.
 * The profile function can be set via "t_profile" parameter, e.g.:
 * <param name="t_profile" value="exp(5*(1-x)*t)" />
 * The formula is parsed via TFormula ROOT class.
 *
 */
class DVCSCFFCMILOU3DTablesArbitraryTProfile: public DVCSCFFCMILOU3DTables {

public:

    static const std::string PARAMETER_NAME_T_PROFILE; ///< String used to switch between available CFF sets.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFCMILOU3DTablesArbitraryTProfile(const std::string &className);

    virtual DVCSCFFCMILOU3DTablesArbitraryTProfile* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVCSCFFCMILOU3DTablesArbitraryTProfile();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::complex<double> computeCFF();

    /**
     * Get CFF set file.
     */
    const std::string& getTProfile() const;

    /**
     * Set CFF set file.
     */
    void setTProfile(const std::string& tProfile);

protected:

    /**
     * Copy constructor.
     @param other Object to be copied.
     */
    DVCSCFFCMILOU3DTablesArbitraryTProfile(const DVCSCFFCMILOU3DTablesArbitraryTProfile &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    std::string m_tProfile; ///< Current t profile.
    TFormula m_tFormula;
};

} /* namespace PARTONS */

#endif /* DVCS_CFF_MILOU3DTABLES_ARBITRARY_T_PROFILE_H */

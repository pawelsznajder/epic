#ifndef DVCS_CFF_MILOU3DTABLES_H
#define DVCS_CFF_MILOU3DTABLES_H

/**
 * @file DVCSCFFCMILOU3DTables.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date 06 April 2021
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h>
#include <complex>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../../../algorithms/interpolation/linterp/linterp.h"

namespace PARTONS {

/**
 * @class DVCSCFFCMILOU3DTables
 *
 * @brief Module for evaluation of CFF values from lookup tables.
 *
 * Module for evaluation of CFF values from lookup tables (xB, t, Q2). Available tables are imported from MILOU MC generator (https://eic.github.io/software/milou.html)
 * variation for 3D tables (https://drf-gitlab.cea.fr/monte-carlo/milou).
 *
 * Since the tables are for xB and Q2 it is explicitly assumed that xi=xB/(2-xB) and muF2 = muR2 = Q2.
 *
 * User may change between available sets of tables with DVCSCFFCMILOU3DTables::setCFFSetFile(),
 * or DVCSCFFCMILOU3DTables::configure() using DVCSCFFCMILOU3DTables::PARAMETER_NAME_SET_FILE tag.
 * E.g. DVCSCFFCMILOU3DTables::setCFFSetFile("data/DVCSCFFCMILOU3DTables/tables_KM20.root") will probe tables
 * stored in "data/DVCSCFFCMILOU3DTables/tables_KM20.root" file.
 *
 * By default "data/DVCSCFFCMILOU3DTables/tables_GK.root" tables are probed.
 *
 * The interpolation over lookup table is done with linterp library (https://rncarpio.github.io/linterp).
 */
class DVCSCFFCMILOU3DTables: public DVCSConvolCoeffFunctionModule {

public:

    static const std::string PARAMETER_NAME_SET_FILE; ///< String used to switch between available CFF sets.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFCMILOU3DTables(const std::string &className);

    virtual DVCSCFFCMILOU3DTables* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVCSCFFCMILOU3DTables();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::complex<double> computeCFF();

    /**
     * Get CFF set file.
     */
    const std::string& getCFFSetFile() const;

    /**
     * Set CFF set file.
     */
    void setCFFSetFile(const std::string& cffSetFile);

protected:

    /**
     * Copy constructor.
     @param other Object to be copied.
     */
    DVCSCFFCMILOU3DTables(const DVCSCFFCMILOU3DTables &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /*
     * Load grids from default location and given set.
     */
    void loadGrids();

    /*
     * Load single grid.
     */
    std::pair<std::shared_ptr<NDInterpolator_3_ML>,
            std::shared_ptr<NDInterpolator_3_ML> > loadGrid(
            const std::string& type, const std::string& flavor);

    /**
     * Read grid.
     */
    std::vector<std::vector<double> > readGrid(const std::string& type,
            const std::string& flavor) const;

    /**
     * Check and set range.
     */
    void checkAndSetRange(std::pair<double, double>& target,
            const std::pair<double, double>& source);

    /**
     * Check range.
     */
    bool checkRange(double& value, const std::pair<double, double>& range,
            const std::pair<bool, bool>& freeze);

    /**
     * Find index.
     */
    int findIndex(const std::vector<double>& vec, double val) const;

    /**
     * Check if value already stored in vector.
     */
    bool checkIfUnique(const std::vector<double>& vec, double val) const;

    /**
     * Evaluate xB for given xi.
     */
    double xiToxB(double xi) const;

    std::string m_cffSetFile; ///< Current set file.

    bool m_tablesLoaded; ///< True is tables loaded.

    std::vector<std::pair<double, double> > m_ranges; ///< Kinematic ranges

    std::map<GPDType::Type, std::vector<std::shared_ptr<NDInterpolator_3_ML> > > m_map; ///< Map containing pointers to interpolation grids.

    bool m_printOutsideGridWarnings; ///< Print more warnings.
};

} /* namespace PARTONS */

#endif /* DVCS_CFF_MILOU3DTABLES_H */

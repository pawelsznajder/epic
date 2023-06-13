#ifndef DDVCS_CFFTABLES_H
#define DDVCS_CFFTABLES_H

/**
 * @file DDVCSCFFTables.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date 06 April 2021
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/modules/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionModule.h>
#include <complex>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../../../algorithms/interpolation/linterp/linterp.h"

namespace PARTONS {

/**
 * @class DDVCSCFFTables
 *
 * @brief Module for evaluation of CFF values from lookup tables.
 *
 * Module for evaluation of CFF values from lookup tables (xi, t, Q2, Q'2).
 *
 * Since the tables are for xi, Q2, Q'2 it is explicitly assumed that muF2 = muR2 = Q2 + Q'2 and eta = xi * ((Q2 - Q'2) / (Q2 + Q'2))
 *
 * User may change between available sets of tables with DDVCSCFFTables::setCFFSetFile(),
 * or DDVCSCFFTables::configure() using DDVCSCFFTables::PARAMETER_NAME_SET_FILE tag.
 * E.g. DDVCSCFFTables::setCFFSetFile("data/DDVCSCFFTables/tables_GK.root") will probe tables
 * stored in "data/DDVCSCFFTables/tables_GK.root" file.
 *
 * By default "data/DDVCSCFFTables/tables_GK.root" tables are probed.
 *
 * The interpolation over lookup table is done with linterp library (https://rncarpio.github.io/linterp).
 */
class DDVCSCFFTables: public DDVCSConvolCoeffFunctionModule {

public:

    static const std::string PARAMETER_NAME_SET_FILE; ///< String used to switch between available CFF sets.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DDVCSCFFTables(const std::string &className);

    virtual DDVCSCFFTables* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DDVCSCFFTables();

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
    DDVCSCFFTables(const DDVCSCFFTables &other);

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
    std::pair<std::shared_ptr<NDInterpolator_4_ML>,
            std::shared_ptr<NDInterpolator_4_ML> > loadGrid(
            const std::string& type);

    /**
     * Read grid.
     */
    std::vector<std::vector<double> > readGrid(const std::string& type) const;

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

    std::string m_cffSetFile; ///< Current set file.

    bool m_tablesLoaded; ///< True is tables loaded.

    std::vector<std::pair<double, double> > m_ranges; ///< Kinematic ranges

    std::map<GPDType::Type, std::vector<std::shared_ptr<NDInterpolator_4_ML> > > m_map; ///< Map containing pointers to interpolation grids.

    bool m_printOutsideGridWarnings; ///< Print more warnings.
};

} /* namespace PARTONS */

#endif /* DDVCS_CFFTABLES_H */

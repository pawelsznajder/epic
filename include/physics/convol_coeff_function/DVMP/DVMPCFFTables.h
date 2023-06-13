#ifndef DVMP_CFF_TABLES_H
#define DVMP_CFF_TABLES_H

/**
 * @file DVMPCFFTables.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date 06 April 2021
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/modules/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionModule.h>
#include <complex>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../../../algorithms/interpolation/linterp/linterp.h"

namespace PARTONS {

/**
 * @class DVMPCFFTables
 *
 * @brief Module for evaluation of CFF values from lookup tables.
 *
 * Module for evaluation of CFF values from lookup tables (xi, t, Q2). Available tables are generated with PARTONS. 
 *
 * User may change between available sets of tables with DVMPCFFTables::setCFFSetFile(),
 * or DVMPCFFTables::configure() using DVMPCFFTables::PARAMETER_NAME_SET_FILE tag.
 *
 * By default "data/DVMPCFFTables/pi0_table_GK.root" tables are probed.
 *
 * The interpolation over lookup table is done with linterp library (https://rncarpio.github.io/linterp).
 */
class DVMPCFFTables: public DVMPConvolCoeffFunctionModule {

public:

    static const std::string PARAMETER_NAME_SET_FILE; ///< String used to switch between available CFF sets.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVMPCFFTables(const std::string &className);

    virtual DVMPCFFTables* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVMPCFFTables();

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
    DVMPCFFTables(const DVMPCFFTables &other);

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

    std::map<GPDType::Type, std::pair<std::shared_ptr<NDInterpolator_3_ML>, std::shared_ptr<NDInterpolator_3_ML> > > m_map; ///< Map containing pointers to interpolation grids.

    bool m_printOutsideGridWarnings; ///< Print more warnings.
};

} /* namespace PARTONS */

#endif /* DVMP_CFF_TABLES_H */

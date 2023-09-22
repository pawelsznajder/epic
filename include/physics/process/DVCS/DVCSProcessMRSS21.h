#ifndef DVCS_PROCESS_MRSS21_H
#define DVCS_PROCESS_MRSS21_H

#include <NumA/linear_algebra/vector/Vector4D.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <partons/utils/type/PhysicalType.h>
#include <partons/modules/process/DVCS/DVCSProcessModule.h>

#include "../../../algorithms/interpolation/linterp/linterp.h"

namespace PARTONS {

/**
 * @class DVCSProcessMRSS21
 *
 * Module for the DVCS process using Phys. Rev. D 103, 094026 (2021).
 */
class DVCSProcessMRSS21: public DVCSProcessModule {

public:

    static const std::string PARAMETER_NAME_GRID_FILE; ///< String used to select grid file.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     */
    DVCSProcessMRSS21(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~DVCSProcessMRSS21();

    virtual DVCSProcessMRSS21* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Get grid file.
     */
    const std::string& getGridFile() const;

    /**
     * Set grid file.
     */
    void setGridFile(const std::string& gridFile);

protected:
    /**
     * Copy constructor
     */
    DVCSProcessMRSS21(const DVCSProcessMRSS21& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    // Cross sections
    virtual PhysicalType<double> CrossSectionBH();
    virtual PhysicalType<double> CrossSectionVCS();
    virtual PhysicalType<double> CrossSectionInterf();

private:

    /*
     * Load grids from default location and given set.
     */
    void loadGrids();

    /**
     * Load single grid.
     */
    std::shared_ptr<NDInterpolator_3_ML> loadGrid(size_t xsIndex);

    /**
     * Read grid.
     */
    std::vector<std::vector<double> > readGrid(size_t csIndex) const;

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

    std::string m_gridFile; ///< Current grid file.

    bool m_tablesLoaded; ///< True is tables loaded.

    std::vector<std::pair<double, double> > m_ranges; ///< Kinematic ranges

    std::vector<std::shared_ptr<NDInterpolator_3_ML> > m_interpolators; ///< Vector of interpolators.

    bool m_printOutsideGridWarnings; ///< Print more warnings.
};

} /* namespace PARTONS */

#endif /* DVCS_PROCESS_MRSS21_H */


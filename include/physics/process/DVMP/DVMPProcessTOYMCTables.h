#ifndef DVMP_PROCESS_TOYMC_TABLES_H
#define DVMP_PROCESS_TOYMC_TABLES_H

/**
 * @file DVMPProcessTOYMCTables.h
 * @author Pawel Sznajder (NCBJ)
 * @date August 21, 2021
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/modules/process/DVMP/DVMPProcessModule.h>
#include <partons/utils/type/PhysicalType.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../../../algorithms/interpolation/linterp/linterp.h"

namespace PARTONS {

/**
 * @class DVMPProcessTOYMCTables
 *
 * TODO
 */
class DVMPProcessTOYMCTables: public DVMPProcessModule {

public:

    static const std::string PARAMETER_NAME_GRID_FILE; ///< String used to select grid file.
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     */
    DVMPProcessTOYMCTables(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPProcessTOYMCTables();

    virtual DVMPProcessTOYMCTables* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Get grid file path.
     */
    const std::string& getGridPath() const;

    /**
     * Set grid file path.
     */
    void setGridPath(const std::string& gridFile);

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    DVMPProcessTOYMCTables(const DVMPProcessTOYMCTables& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSection();

private:

    /**
     * Read grid.
     */
    std::vector<std::vector<double> > readGrid() const;

    /**
     * Load grid.
     */
    void loadGrid();

    /**
     * Check range.
     */
    bool checkRange(double& value, const std::pair<double, double>& range,
            const std::pair<bool, bool>& freeze);

    /**
     * Check if value already stored in vector.
     */
    bool checkIfUnique(const std::vector<double>& vec, double val) const;

    /**
     * Find index.
     */
    int findIndex(const std::vector<double>& vec, double val) const;

    /**
     * Get beam energy in fixed target frame.
     */
    double getLeptonEnergyFixedTargetEquivalent(double E1, double E2) const;

    std::string m_gridPath; ///< Current grid file.
    bool m_tablesLoaded; ///< True if table is loaded.
    std::vector<std::pair<double, double> > m_ranges; ///< Kinematic ranges
    std::shared_ptr<NDInterpolator_4_ML> m_interpolator; ///< Interpolation.
    double m_beamEnergy; ///< Beam energy for loaded table.

    double m_threshold; ///< Threshold for cross-section.
    double m_beamEnergyTolerance; ///< Beam energy tolerance.

    bool m_printOutsideGridWarnings; ///< Print more warnings.
};

} /* namespace PARTONS */

#endif /* DVMP_PROCESS_TOYMC_TABLES_H */

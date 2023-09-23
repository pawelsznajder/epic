/*
 * DVMPGeneratorService.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_DVMPGENERATORSERVICE_H_
#define SERVICES_DVMPGENERATORSERVICE_H_

#include <partons/beans/PolarizationType.h>
#include <partons/modules/process/DVMP/DVMPProcessModule.h>
#include <string>
#include <vector>

#include "../beans/containers/DVMPKinematicRanges.h"
#include "../beans/types/ParticleType.h"
#include "../modules/kinematic/DVMP/DVMPKinematicModule.h"
#include "../modules/radiative_corrections/DVMP/DVMPRCModule.h"
#include "GeneratorService.h"

namespace EPIC {

/**
 * @class DVMPGeneratorService
 *
 * @brief Service used to perform generation of DVMP MC events.
 *
 * This class handles generation process of MC events for DVMP. It is intended
 * to have a
 * unique instance accessed trough DVMPGeneratorService::getInstance() method.
 */
class DVMPGeneratorService: public GeneratorService<DVMPKinematicRanges,
        PARTONS::DVMPProcessModule, DVMPKinematicModule,
        DVMPRCModule> {

public:

    static const std::string DVMP_GENERATOR_SERVICE_MESON_TYPE; ///< String used to set meson type via XML scenario.
    static const std::string DVMP_GENERATOR_SERVICE_MESON_POLARIZATION; ///< String used to set polarization type via XML scenario.
    static const std::string DVMP_GENERATOR_SERVICE_IS_PHOTOPRODUCTION; ///< String used to set switch to turn on photoproduction via XML scenario.

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DVMPGeneratorService();

    virtual double getEventDistribution(std::vector<double> &kin) const;
    virtual void run();

private:

    /**
     * Default constructor.
     */
    DVMPGeneratorService(const std::string &className);

    /**
     * Copy constructor.
     */
    DVMPGeneratorService(const DVMPGeneratorService &other);

    virtual DVMPGeneratorService *clone() const;

    virtual void getAdditionalGeneralConfigurationFromTask(
            const MonteCarloTask &task);

    virtual void getKinematicRangesFromTask(const MonteCarloTask &task);
    virtual void getProcessModuleFromTask(const MonteCarloTask &task);
    virtual void getKinematicModuleFromTask(const MonteCarloTask &task);
    virtual void getRCModuleFromTask(const MonteCarloTask &task);
    virtual void isServiceWellConfigured() const;
    virtual void transformVariables(std::vector<double>& variables) const;
    virtual void transformRanges(std::vector<KinematicRange>& ranges) const;
    virtual double getJacobian(const std::vector<double>& variables) const;

    virtual void bookHistograms();
    void fillHistograms(const std::vector<double>& variables);

    /**
     * Get flux of photons.
     */
    double getFlux(const DVMPKinematic& kin) const;

    ParticleType::Type m_mesonType; ///< Meson type.
    PARTONS::PolarizationType::Type m_mesonPolarization; ///< Polarization type.
    bool m_isPhotoproduction; ///< If true, generation includes flux of virtual photons.
};

} /* namespace EPIC */

#endif /* SERVICES_DVMPGENERATORSERVICE_H_ */

/*
 * TCSGeneratorService.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_TCSGENERATORSERVICE_H_
#define SERVICES_TCSGENERATORSERVICE_H_

#include <partons/beans/process/VCSSubProcessType.h>
#include <partons/modules/process/TCS/TCSProcessModule.h>
#include <string>
#include <vector>

#include "../beans/containers/TCSKinematic.h"
#include "../beans/containers/TCSKinematicRanges.h"
#include "../modules/kinematic/TCS/TCSKinematicModule.h"
#include "../modules/radiative_corrections/TCS/TCSRCModule.h"
#include "GeneratorService.h"

namespace EPIC {

/**
 * @class TCSGeneratorService
 *
 * @brief Service used to perform generation of TCS MC events.
 *
 * This class handles generation process of MC events for TCS. It is intended
 * to have a
 * unique instance accessed trough TCSGeneratorService::getInstance() method.
 */
class TCSGeneratorService: public GeneratorService<TCSKinematicRanges,
        PARTONS::TCSProcessModule, TCSKinematicModule, 
        TCSRCModule> {

public:

    static const std::string TCS_GENERATOR_SERVICE_SUBPROCESSTYPE; ///< String used to set subprocess type via XML scenario.

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Destructor.
     */
    virtual ~TCSGeneratorService();

    virtual double getEventDistribution(std::vector<double> &kin) const;
    virtual void run();

private:

    /**
     * Default constructor.
     */
    TCSGeneratorService(const std::string &className);

    /**
     * Copy constructor.
     */
    TCSGeneratorService(const TCSGeneratorService &other);

    virtual TCSGeneratorService *clone() const;

    virtual void getAdditionalGeneralConfigurationFromTask(
            const MonteCarloTask &task);

    virtual void getKinematicRangesFromTask(const MonteCarloTask &task);
    virtual void getProcessModuleFromTask(const MonteCarloTask &task);
    virtual void getKinematicModuleFromTask(const MonteCarloTask &task);
    virtual void getRCModuleFromTask(const MonteCarloTask &task);
    virtual void isServiceWellConfigured() const;
    virtual void addAdditionalGenerationConfiguration(GenerationInformation& generationInformation);
    virtual void transformVariables(std::vector<double>& variables) const;
    virtual void transformRanges(std::vector<KinematicRange>& ranges) const;
    virtual double getJacobian(const std::vector<double>& variables) const;

    virtual void bookHistograms();
    void fillHistograms(const std::vector<double>& variables);

    /**
     * Get flux of photons.
     */
    double getFlux(const TCSKinematic& kin) const;

    PARTONS::VCSSubProcessType::Type m_subProcessType; ///< subprocess types.
};

} /* namespace EPIC */

#endif /* SERVICES_TCSGENERATORSERVICE_H_ */

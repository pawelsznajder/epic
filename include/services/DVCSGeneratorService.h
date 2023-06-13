/*
 * DVCSGeneratorService.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_DVCSGENERATORSERVICE_H_
#define SERVICES_DVCSGENERATORSERVICE_H_

#include <partons/beans/process/VCSSubProcessType.h>
#include <partons/modules/process/DVCS/DVCSProcessModule.h>
#include <string>
#include <vector>

#include "../beans/containers/DVCSKinematicRanges.h"
#include "../modules/kinematic/DVCS/DVCSKinematicModule.h"
#include "../modules/radiative_corrections/DVCS/DVCSRCModule.h"
#include "GeneratorService.h"

namespace EPIC {

/**
 * @class DVCSGeneratorService
 *
 * @brief Service used to perform generation of DVCS MC events.
 *
 * This class handles generation process of MC events for DVCS. It is intended
 * to have a
 * unique instance accessed trough DVCSGeneratorService::getInstance() method.
 */
class DVCSGeneratorService: public GeneratorService<DVCSKinematicRanges,
        PARTONS::DVCSProcessModule, DVCSKinematicModule,
        DVCSRCModule> {

public:

    static const std::string DVCS_GENERATOR_SERVICE_SUBPROCESSTYPE; ///< String used to set subprocess type via XML scenario.

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DVCSGeneratorService();

    virtual double getEventDistribution(std::vector<double> &kin) const;
    virtual void run();

private:

    /**
     * Default constructor.
     */
    DVCSGeneratorService(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSGeneratorService(const DVCSGeneratorService &other);

    virtual DVCSGeneratorService *clone() const;

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

    PARTONS::VCSSubProcessType::Type m_subProcessType; ///< subprocess types.
};

} /* namespace EPIC */

#endif /* SERVICES_DVCSGENERATORSERVICE_H_ */

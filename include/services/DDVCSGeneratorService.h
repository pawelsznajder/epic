/*
 * DDVCSGeneratorService.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_DDVCSGENERATORSERVICE_H_
#define SERVICES_DDVCSGENERATORSERVICE_H_

#include <partons/beans/process/VCSSubProcessType.h>
#include <partons/modules/process/DDVCS/DDVCSProcessModule.h>
#include <string>
#include <vector>

#include "../beans/containers/DDVCSKinematic.h"
#include "../beans/containers/DDVCSKinematicRanges.h"
#include "../modules/kinematic/DDVCS/DDVCSKinematicModule.h"
#include "../modules/radiative_corrections/DDVCS/DDVCSRCModule.h"
#include "GeneratorService.h"

namespace EPIC {

/**
 * @class DDVCSGeneratorService
 *
 * @brief Service used to perform generation of DDVCS MC events.
 *
 * This class handles generation process of MC events for DDVCS. It is intended
 * to have a
 * unique instance accessed trough DDVCSGeneratorService::getInstance() method.
 */
class DDVCSGeneratorService: public GeneratorService<DDVCSKinematicRanges,
        PARTONS::DDVCSProcessModule, DDVCSKinematicModule, DDVCSKinematic,
        DDVCSRCModule> {

public:

    static const std::string DDVCS_GENERATOR_SERVICE_SUBPROCESSTYPE; ///< String used to set subprocess type via XML scenario.

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DDVCSGeneratorService();

    virtual double getEventDistribution(const std::vector<double> &kin) const;
    virtual void run();

private:

    /**
     * Default constructor.
     */
    DDVCSGeneratorService(const std::string &className);

    /**
     * Copy constructor.
     */
    DDVCSGeneratorService(const DDVCSGeneratorService &other);

    virtual DDVCSGeneratorService *clone() const;

    virtual void getAdditionalGeneralConfigurationFromTask(
            const MonteCarloTask &task);

    virtual void getKinematicRangesFromTask(const MonteCarloTask &task);
    virtual void getProcessModuleFromTask(const MonteCarloTask &task);
    virtual void getKinematicModuleFromTask(const MonteCarloTask &task);
    virtual void getRCModuleFromTask(const MonteCarloTask &task);
    virtual void isServiceWellConfigured() const;
    virtual void addAdditionalGenerationConfiguration(GenerationInformation& generationInformation);

    virtual void bookHistograms();
    void fillHistograms(const std::vector<double>& variables);

    PARTONS::VCSSubProcessType::Type m_subProcessType; ///< subprocess types.

    double getDxDyJacobian(const DDVCSKinematic& kin) const;
};

} /* namespace EPIC */

#endif /* SERVICES_DDVCSGENERATORSERVICE_H_ */

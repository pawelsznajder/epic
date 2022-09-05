/*
 * GAM2GeneratorService.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_GAM2GENERATORSERVICE_H_
#define SERVICES_GAM2GENERATORSERVICE_H_

#include <partons/beans/process/VCSSubProcessType.h>
#include <partons/modules/process/GAM2/GAM2ProcessModule.h>
#include <string>
#include <vector>

#include "../beans/containers/GAM2Kinematic.h"
#include "../beans/containers/GAM2KinematicRanges.h"
#include "../modules/kinematic/GAM2/GAM2KinematicModule.h"
#include "../modules/radiative_corrections/GAM2/GAM2RCModule.h"
#include "GeneratorService.h"

namespace EPIC {

/**
 * @class GAM2GeneratorService
 *
 * @brief Service used to perform generation of GAM2 MC events.
 *
 * This class handles generation process of MC events for GAM2. It is intended
 * to have a
 * unique instance accessed trough GAM2GeneratorService::getInstance() method.
 */
class GAM2GeneratorService: public GeneratorService<GAM2KinematicRanges,
        PARTONS::GAM2ProcessModule, GAM2KinematicModule, GAM2Kinematic,
        GAM2RCModule> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Destructor.
     */
    virtual ~GAM2GeneratorService();

    virtual double getEventDistribution(const std::vector<double> &kin) const;
    virtual void run();

private:

    /**
     * Default constructor.
     */
    GAM2GeneratorService(const std::string &className);

    /**
     * Copy constructor.
     */
    GAM2GeneratorService(const GAM2GeneratorService &other);

    virtual GAM2GeneratorService *clone() const;

    virtual void getAdditionalGeneralConfigurationFromTask(
            const MonteCarloTask &task);

    virtual void getProcessModuleFromTask(const MonteCarloTask &task);
    virtual void getKinematicModuleFromTask(const MonteCarloTask &task);
    virtual void getRCModuleFromTask(const MonteCarloTask &task);
    virtual void isServiceWellConfigured() const;
    virtual void addAdditionalGenerationConfiguration(GenerationInformation& generationInformation);

    /**
     * Get flux of photons.
     */
    double getFlux(const GAM2Kinematic& kin) const;
};

} /* namespace EPIC */

#endif /* SERVICES_GAM2GENERATORSERVICE_H_ */

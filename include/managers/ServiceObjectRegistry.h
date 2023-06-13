/*
 * ServiceObjectRegistry.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICEOBJECTREGISTRY_H
#define SERVICEOBJECTREGISTRY_H

namespace EPIC {
class DVCSGeneratorService;
class TCSGeneratorService;
class DVMPGeneratorService;
class GAM2GeneratorService;
class DDVCSGeneratorService;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class ServiceObjectRegistry
 *
 * @brief Registry to obtain pointer to a given service.
 *
 * This registry is used to obtain pointers to given services.
 */

class ServiceObjectRegistry {

public:
    /**
     * Destructor.
     */
    virtual ~ServiceObjectRegistry();

    /**
     * Get DVCSGeneratorService.
     */
    DVCSGeneratorService *getDVCSGeneratorService() const;

    /**
     * Get TCSGeneratorService.
     */
    TCSGeneratorService *getTCSGeneratorService() const;

    /**
     * Get DVMPGeneratorService.
     */
    DVMPGeneratorService *getDVMPGeneratorService() const;

    /**
     * Get GAM2GeneratorService.
     */
    GAM2GeneratorService *getGAM2GeneratorService() const;

    /**
     * Get DDVCSGeneratorService.
     */
    DDVCSGeneratorService *getDDVCSGeneratorService() const;

private:
    friend class Epic; ///< Only Epic can make an instance of this class.

    /**
     * Default constructor.
     */
    ServiceObjectRegistry();
};

} /* namespace EPIC */

#endif /* SERVICEOBJECTREGISTRY_H */

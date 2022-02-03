/*
 * TestModuleObjectFactory.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef TESTMODULEOBJECTFACTORY_H_
#define TESTMODULEOBJECTFACTORY_H_

#include <memory>
#include <string>

namespace EPIC {
class DVCSKinematicModule;
class DVMPKinematicModule;
class TCSKinematicModule;
class EventGeneratorModule;
class DVCSRCModule;
class DVMPRCModule;
class TCSRCModule;
class RandomNumberModule;
class EpicModuleObject;
class WriterModule;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class TestModuleObjectFactory
 *
 * @brief Factory to obtain pointer to a given module.
 *
 * This factory is used to obtain modules of a given type.
 */

class ModuleObjectFactory {

public:
    /**
     * Destructor.
     */
    virtual ~ModuleObjectFactory();

    /**
     * New EventGeneratorModule by id.
     */
    std::shared_ptr<EventGeneratorModule>
    newEventGeneratorModule(unsigned int classId);

    /**
     * New EventGeneratorModule by class name.
     */
    std::shared_ptr<EventGeneratorModule>
    newEventGeneratorModule(const std::string &className);

    /**
     * New DVCSKinematicModule by id.
     */
    std::shared_ptr<DVCSKinematicModule>
    newDVCSKinematicModule(unsigned int classId);

    /**
     * New DVCSKinematicModule by class name.
     */
    std::shared_ptr<DVCSKinematicModule>
    newDVCSKinematicModule(const std::string &className);

    /**
     * New DVMPKinematicModule by id.
     */
    std::shared_ptr<DVMPKinematicModule>
    newDVMPKinematicModule(unsigned int classId);

    /**
     * New DVMPKinematicModule by class name.
     */
    std::shared_ptr<DVMPKinematicModule>
    newDVMPKinematicModule(const std::string &className);

    /**
     * New TCSKinematicModule by id.
     */
    std::shared_ptr<TCSKinematicModule>
    newTCSKinematicModule(unsigned int classId);

    /**
     * New TCSKinematicModule by class name.
     */
    std::shared_ptr<TCSKinematicModule>
    newTCSKinematicModule(const std::string &className);

    /**
     * New DVCSRCModule by id.
     */
    std::shared_ptr<DVCSRCModule>
    newDVCSRCModule(unsigned int classId);

    /**
     * New DVCSRCModule by class name.
     */
    std::shared_ptr<DVCSRCModule>
    newDVCSRCModule(const std::string &className);

    /**
     * New DVMPRCModule by id.
     */
    std::shared_ptr<DVMPRCModule>
    newDVMPRCModule(unsigned int classId);

    /**
     * New DVMPRCModule by class name.
     */
    std::shared_ptr<DVMPRCModule>
    newDVMPRCModule(const std::string &className);

    /**
     * New TCSRCModule by id.
     */
    std::shared_ptr<TCSRCModule>
    newTCSRCModule(unsigned int classId);

    /**
     * New TCSRCModule by class name.
     */
    std::shared_ptr<TCSRCModule>
    newTCSRCModule(const std::string &className);

    /**
     * New WriterModule by id.
     */
    std::shared_ptr<WriterModule> newWriterModule(unsigned int classId);

    /**
     * New WriterModule by class name.
     */
    std::shared_ptr<WriterModule> newWriterModule(const std::string &className);

    /**
     * New RandomNumberModule by id.
     */
    std::shared_ptr<RandomNumberModule>
    newRandomNumberModule(unsigned int classId);

    /**
     * New RandomNumberModule by class name.
     */
    std::shared_ptr<RandomNumberModule>
    newRandomNumberModule(const std::string &className);

    /**
     * New module by id.
     */
    std::shared_ptr<EpicModuleObject> newTestModuleObject(unsigned int classId);

    /**
     * New module by class name.
     */
    std::shared_ptr<EpicModuleObject>
    newTestModuleObject(const std::string &className);

private:
    friend class Epic; ///< Only Epic can make an instance of this class.

    /**
     * Default constructor.
     */
    ModuleObjectFactory();
};

} /* namespace EPIC */

#endif /* TESTMODULEOBJECTFACTORY_H_ */

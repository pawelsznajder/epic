/*
 * TestModuleObjectFactory.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/managers/ModuleObjectFactory.h"

#include <partons/BaseObjectFactory.h>
#include <partons/Partons.h>

#include "../../include/modules/event_generator/EventGeneratorModule.h"
#include "../../include/modules/kinematic/DVCS/DVCSKinematicModule.h"
#include "../../include/modules/kinematic/DVMP/DVMPKinematicModule.h"
#include "../../include/modules/kinematic/TCS/TCSKinematicModule.h"
#include "../../include/modules/kinematic/GAM2/GAM2KinematicModule.h"
#include "../../include/modules/radiative_corrections/DVCS/DVCSRCModule.h"
#include "../../include/modules/radiative_corrections/DVMP/DVMPRCModule.h"
#include "../../include/modules/radiative_corrections/TCS/TCSRCModule.h"
#include "../../include/modules/random_generator/RandomNumberModule.h"
#include "../../include/modules/writer/WriterModule.h"

namespace EPIC {

ModuleObjectFactory::ModuleObjectFactory() {
}

ModuleObjectFactory::~ModuleObjectFactory() {
}

std::shared_ptr<EventGeneratorModule> ModuleObjectFactory::newEventGeneratorModule(
        unsigned int classId) {
    return std::static_pointer_cast<EventGeneratorModule>(
            newTestModuleObject(classId));
}

std::shared_ptr<EventGeneratorModule> ModuleObjectFactory::newEventGeneratorModule(
        const std::string &className) {
    return std::static_pointer_cast<EventGeneratorModule>(
            newTestModuleObject(className));
}

std::shared_ptr<DVCSKinematicModule> ModuleObjectFactory::newDVCSKinematicModule(
        unsigned int classId) {
    return std::static_pointer_cast<DVCSKinematicModule>(
            newTestModuleObject(classId));
}

std::shared_ptr<DVCSKinematicModule> ModuleObjectFactory::newDVCSKinematicModule(
        const std::string &className) {
    return std::static_pointer_cast<DVCSKinematicModule>(
            newTestModuleObject(className));
}

std::shared_ptr<DVMPKinematicModule> ModuleObjectFactory::newDVMPKinematicModule(
        unsigned int classId) {
    return std::static_pointer_cast<DVMPKinematicModule>(
            newTestModuleObject(classId));
}

std::shared_ptr<DVMPKinematicModule> ModuleObjectFactory::newDVMPKinematicModule(
        const std::string &className) {
    return std::static_pointer_cast<DVMPKinematicModule>(
            newTestModuleObject(className));
}

std::shared_ptr<TCSKinematicModule> ModuleObjectFactory::newTCSKinematicModule(
        unsigned int classId) {
    return std::static_pointer_cast<TCSKinematicModule>(
            newTestModuleObject(classId));
}

std::shared_ptr<TCSKinematicModule> ModuleObjectFactory::newTCSKinematicModule(
        const std::string &className) {
    return std::static_pointer_cast<TCSKinematicModule>(
            newTestModuleObject(className));
}

std::shared_ptr<GAM2KinematicModule> ModuleObjectFactory::newGAM2KinematicModule(
        unsigned int classId) {
    return std::static_pointer_cast<GAM2KinematicModule>(
            newTestModuleObject(classId));
}

std::shared_ptr<GAM2KinematicModule> ModuleObjectFactory::newGAM2KinematicModule(
        const std::string &className) {
    return std::static_pointer_cast<GAM2KinematicModule>(
            newTestModuleObject(className));
}

std::shared_ptr<DVCSRCModule> ModuleObjectFactory::newDVCSRCModule(
        unsigned int classId) {
    return std::static_pointer_cast<DVCSRCModule>(newTestModuleObject(classId));
}

std::shared_ptr<DVCSRCModule> ModuleObjectFactory::newDVCSRCModule(
        const std::string &className) {
    return std::static_pointer_cast<DVCSRCModule>(
            newTestModuleObject(className));
}

std::shared_ptr<DVMPRCModule> ModuleObjectFactory::newDVMPRCModule(
        unsigned int classId) {
    return std::static_pointer_cast<DVMPRCModule>(newTestModuleObject(classId));
}

std::shared_ptr<DVMPRCModule> ModuleObjectFactory::newDVMPRCModule(
        const std::string &className) {
    return std::static_pointer_cast<DVMPRCModule>(
            newTestModuleObject(className));
}

std::shared_ptr<TCSRCModule> ModuleObjectFactory::newTCSRCModule(
        unsigned int classId) {
    return std::static_pointer_cast<TCSRCModule>(newTestModuleObject(classId));
}

std::shared_ptr<TCSRCModule> ModuleObjectFactory::newTCSRCModule(
        const std::string &className) {
    return std::static_pointer_cast<TCSRCModule>(newTestModuleObject(className));
}

std::shared_ptr<WriterModule> ModuleObjectFactory::newWriterModule(
        unsigned int classId) {
    return std::static_pointer_cast<WriterModule>(newTestModuleObject(classId));
}

std::shared_ptr<WriterModule> ModuleObjectFactory::newWriterModule(
        const std::string &className) {
    return std::static_pointer_cast<WriterModule>(
            newTestModuleObject(className));
}

std::shared_ptr<RandomNumberModule> ModuleObjectFactory::newRandomNumberModule(
        unsigned int classId) {
    return std::static_pointer_cast<RandomNumberModule>(
            newTestModuleObject(classId));
}

std::shared_ptr<RandomNumberModule> ModuleObjectFactory::newRandomNumberModule(
        const std::string &className) {
    return std::static_pointer_cast<RandomNumberModule>(
            newTestModuleObject(className));
}

std::shared_ptr<EpicModuleObject> ModuleObjectFactory::newTestModuleObject(
        unsigned int classId) {

    std::shared_ptr<EpicModuleObject> pModuleObject(
            static_cast<EpicModuleObject *>(PARTONS::Partons::getInstance()->getBaseObjectFactory()->newBaseObject(
                    classId)));

    return pModuleObject;
}

std::shared_ptr<EpicModuleObject> ModuleObjectFactory::newTestModuleObject(
        const std::string &className) {

    std::shared_ptr<EpicModuleObject> pModuleObject(
            static_cast<EpicModuleObject *>(PARTONS::Partons::getInstance()->getBaseObjectFactory()->newBaseObject(
                    className)));

    return pModuleObject;
}

} /* namespace EPIC */

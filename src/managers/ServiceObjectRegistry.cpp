/*
 * ServiceObjectRegistry.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/managers/ServiceObjectRegistry.h"

#include <partons/BaseObjectFactory.h>
#include <partons/Partons.h>

#include "../../include/services/DVCSGeneratorService.h"
#include "../../include/services/TCSGeneratorService.h"
#include "../../include/services/DVMPGeneratorService.h"


namespace EPIC {

ServiceObjectRegistry::ServiceObjectRegistry() {
}

ServiceObjectRegistry::~ServiceObjectRegistry() {
}

DVCSGeneratorService *ServiceObjectRegistry::getDVCSGeneratorService() const {
	return static_cast<DVCSGeneratorService *>(PARTONS::Partons::getInstance()->getBaseObjectFactory()->newBaseObject(
			DVCSGeneratorService::classId));
}

TCSGeneratorService *ServiceObjectRegistry::getTCSGeneratorService() const {
	return static_cast<TCSGeneratorService *>(PARTONS::Partons::getInstance()->getBaseObjectFactory()->newBaseObject(
			TCSGeneratorService::classId));
}

DVMPGeneratorService *ServiceObjectRegistry::getDVMPGeneratorService() const {
    return static_cast<DVMPGeneratorService *>(PARTONS::Partons::getInstance()->getBaseObjectFactory()->newBaseObject(
            DVMPGeneratorService::classId));
}

} /* namespace EPIC */

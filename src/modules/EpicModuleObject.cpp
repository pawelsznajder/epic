/*
 * EpicModuleObject.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/modules/EpicModuleObject.h"

#include "../../include/Epic.h"
#include "../../include/managers/RandomSeedManager.h"
#include "../../include/services/TestingService.h"

namespace EPIC {

EpicModuleObject::EpicModuleObject(const std::string &className,
		PARTONS::ChannelType::Type channelType) :
		PARTONS::ModuleObject(className, channelType) {

	m_seed = Epic::getInstance()->getRandomSeedManager()->getSeed();

	TestingService::getInstance()->registerModuleObject(className);
}

EpicModuleObject::EpicModuleObject(const EpicModuleObject &other) :
		PARTONS::ModuleObject(other) {
	m_seed = Epic::getInstance()->getRandomSeedManager()->getSeed();
}

EpicModuleObject::~EpicModuleObject() {
}

size_t EpicModuleObject::getSeed() const {
	return m_seed;
}

} /* namespace EPIC */

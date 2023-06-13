/*
 * Epic.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../include/Epic.h"

#include <ElementaryUtils/ElementaryUtilsVersion.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/NumAVersion.h>
#include <partons/Partons.h>
#include <partons/PartonsVersion.h>

#include "../include/EpicVersion.h"
#include "../include/managers/ModuleObjectFactory.h"
#include "../include/managers/RandomSeedManager.h"
#include "../include/managers/ServiceObjectRegistry.h"

namespace EPIC {

Epic *Epic::m_pInstance = nullptr;

Epic::Epic() {

	m_pPartons = PARTONS::Partons::getInstance();

	printVersion();

	m_pServiceObjectRegistry = new ServiceObjectRegistry();
	m_pModuleObjectFactory = new ModuleObjectFactory();
	m_pRandomSeedManager = new RandomSeedManager();
}

Epic::~Epic() {

	if (m_pServiceObjectRegistry != nullptr) {
		delete m_pServiceObjectRegistry;
		m_pServiceObjectRegistry = nullptr;
	}

	if (m_pModuleObjectFactory != nullptr) {
		delete m_pModuleObjectFactory;
		m_pModuleObjectFactory = nullptr;
	}

	if (m_pRandomSeedManager != nullptr) {
		delete m_pRandomSeedManager;
		m_pRandomSeedManager = nullptr;
	}

	if (m_pInstance != nullptr) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

Epic *Epic::getInstance() {

	if (m_pInstance == nullptr) {
		m_pInstance = new Epic();
	}

	return m_pInstance;
}

void Epic::init(int argc, char **argv) {
	m_pPartons->init(argc, argv);
}

void Epic::close() {
	m_pPartons->close();
}

ServiceObjectRegistry *Epic::getServiceObjectRegistry() const {
	return m_pServiceObjectRegistry;
}

ModuleObjectFactory *Epic::getModuleObjectFactory() const {
	return m_pModuleObjectFactory;
}

RandomSeedManager *Epic::getRandomSeedManager() const {
	return m_pRandomSeedManager;
}

void Epic::printVersion() const {
	m_pPartons->getLoggerManager()->info("Epic", __func__,
			ElemUtils::Formatter() << "EPIC " << EPIC_VERSION_MAJOR << "."
					<< EPIC_VERSION_MINOR << "." << EPIC_VERSION_PATCH
					<< " (https://pawelsznajder.github.io/epic) distributed under GNU Public License");
}

} /* namespace EPIC */

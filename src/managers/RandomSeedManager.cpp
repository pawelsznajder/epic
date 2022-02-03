/*
 * RandomSeedManager.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/managers/RandomSeedManager.h"

#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/Partons.h>

namespace EPIC {

RandomSeedManager::RandomSeedManager() :
		m_seedCount(0) {
}

RandomSeedManager::~RandomSeedManager() {
}

size_t RandomSeedManager::getSeed() {
	return ++m_seedCount;
}

void RandomSeedManager::setSeedCount(size_t seedCount) {

	PARTONS::Partons::getInstance()->getLoggerManager()->info(
			"RandomSeedManager", __func__,
			ElemUtils::Formatter() << "Initial random seed set to: "
					<< seedCount);

	m_seedCount = seedCount;
}

} /* namespace EPIC */

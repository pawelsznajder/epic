/*
 * GeneralConfiguration.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/GeneralConfiguration.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"

namespace EPIC {

const std::string GeneralConfiguration::GENERAL_CONFIGURATION_NUMBER_OF_EVENTS =
		"number_of_events";

GeneralConfiguration::GeneralConfiguration() :
		PARTONS::BaseObject("GeneralConfiguration"), m_nEvents(0) {
}

GeneralConfiguration::GeneralConfiguration(size_t nEvents) :
		PARTONS::BaseObject("GeneralConfiguration"), m_nEvents(nEvents) {
}

GeneralConfiguration::GeneralConfiguration(const GeneralConfiguration &other) :
		PARTONS::BaseObject(other), m_nEvents(other.m_nEvents) {
}

GeneralConfiguration::~GeneralConfiguration() {
}

std::string GeneralConfiguration::toString() const {

	ElemUtils::Formatter formatter;

	formatter << '\n';
	formatter << "Number of events: " << m_nEvents << '\n';

	return formatter.str();
}

GeneralConfiguration GeneralConfiguration::fromTask(
		const MonteCarloTask &task) {

	// result
	GeneralConfiguration result;

	// data
	const ElemUtils::Parameters &data =
			task.getGeneralConfiguration().getParameters();

	// number of events
	result.setNEvents(
			ContainerUtils::findAndParseUInt("GeneralConfiguration", data,
					GeneralConfiguration::GENERAL_CONFIGURATION_NUMBER_OF_EVENTS));

	return result;
}

size_t GeneralConfiguration::getNEvents() const {
	return m_nEvents;
}

void GeneralConfiguration::setNEvents(size_t nEvents) {
	m_nEvents = nEvents;
}

} /* namespace EPIC */

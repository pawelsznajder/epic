/*
 * EventGeneratorModule.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/modules/event_generator/EventGeneratorModule.h"

#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string EventGeneratorModule::EVENT_GENERATOR_MODULE_CLASS_NAME =
		"EventGeneratorModule";
const std::string EventGeneratorModule::EVENT_GENERATOR_MODULE_INIT_STATE_PATH =
		"state_file_path";

EventGeneratorModule::EventGeneratorModule(const std::string &className) :
		EpicModuleObject(className, PARTONS::ChannelType::UNDEFINED) {
}

EventGeneratorModule::EventGeneratorModule(const EventGeneratorModule &other) :
		EpicModuleObject(other), m_initStatePath(other.m_initStatePath) {
}

EventGeneratorModule::~EventGeneratorModule() {
}

void EventGeneratorModule::configure(const ElemUtils::Parameters &parameters) {

	if (parameters.isAvailable(
			EventGeneratorModule::EVENT_GENERATOR_MODULE_INIT_STATE_PATH)) {

		m_initStatePath = parameters.getLastAvailable().getString();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorModule::EVENT_GENERATOR_MODULE_INIT_STATE_PATH
						<< " changed to " << m_initStatePath);
	}
}

bool EventGeneratorModule::runTest() const {

	// TODO
	return true;
}

void EventGeneratorModule::initModule() {
}

void EventGeneratorModule::isModuleWellConfigured() {
}

const std::string& EventGeneratorModule::getInitStatePath() const {
	return m_initStatePath;
}

void EventGeneratorModule::setInitStatePath(const std::string& initStatePath) {
	m_initStatePath = initStatePath;
}

} /* namespace EPIC */

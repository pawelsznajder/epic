/*
 * MonteCarloTask.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/automation/MonteCarloTask.h"

namespace EPIC {

MonteCarloTask::MonteCarloTask() :
		PARTONS::BaseObject("MonteCarloTask") {
}

MonteCarloTask::MonteCarloTask(const MonteCarloTask &other) :
		PARTONS::BaseObject(other) {

	m_serviceName = other.m_serviceName;
	m_methodName = other.m_methodName;

	m_generalConfiguration = other.m_generalConfiguration;
	m_kinematicRange = other.m_kinematicRange;
	m_experimentalConditions = other.m_experimentalConditions;
	m_computationConfiguration = other.m_computationConfiguration;
	m_generatorConfiguration = other.m_generatorConfiguration;
	m_kinematicConfiguration = other.m_kinematicConfiguration;
	m_rcConfiguration = other.m_rcConfiguration;
	m_writerConfiguration = other.m_writerConfiguration;
}

MonteCarloTask::~MonteCarloTask() {
}

const PARTONS::BaseObjectData &
MonteCarloTask::getComputationConfiguration() const {
	return m_computationConfiguration;
}

void MonteCarloTask::setComputationConfiguration(
		const PARTONS::BaseObjectData &computationConfiguration) {
	m_computationConfiguration = computationConfiguration;
}

const PARTONS::BaseObjectData &
MonteCarloTask::getExperimentalConditions() const {
	return m_experimentalConditions;
}

void MonteCarloTask::setExperimentalConditions(
		const PARTONS::BaseObjectData &experimentalConditions) {
	m_experimentalConditions = experimentalConditions;
}

const PARTONS::BaseObjectData &MonteCarloTask::getGeneralConfiguration() const {
	return m_generalConfiguration;
}

void MonteCarloTask::setGeneralConfiguration(
		const PARTONS::BaseObjectData &generalConfiguration) {
	m_generalConfiguration = generalConfiguration;
}

const PARTONS::BaseObjectData &
MonteCarloTask::getKinematicConfiguration() const {
	return m_kinematicConfiguration;
}

void MonteCarloTask::setKinematicConfiguration(
		const PARTONS::BaseObjectData &kinematicConfiguration) {
	m_kinematicConfiguration = kinematicConfiguration;
}

const PARTONS::BaseObjectData &
MonteCarloTask::getRCConfiguration() const {
	return m_rcConfiguration;
}

void MonteCarloTask::setRCConfiguration(
		const PARTONS::BaseObjectData &rcConfiguration) {
	m_rcConfiguration = rcConfiguration;
}

const PARTONS::BaseObjectData &MonteCarloTask::getKinematicRange() const {
	return m_kinematicRange;
}

void MonteCarloTask::setKinematicRange(
		const PARTONS::BaseObjectData &kinematicRange) {
	m_kinematicRange = kinematicRange;
}

const std::string &MonteCarloTask::getMethodName() const {
	return m_methodName;
}

void MonteCarloTask::setMethodName(const std::string &methodName) {
	m_methodName = methodName;
}

const std::string &MonteCarloTask::getServiceName() const {
	return m_serviceName;
}

void MonteCarloTask::setServiceName(const std::string &serviceName) {
	m_serviceName = serviceName;
}

const PARTONS::BaseObjectData &MonteCarloTask::getWriterConfiguration() const {
	return m_writerConfiguration;
}

void MonteCarloTask::setWriterConfiguration(
		const PARTONS::BaseObjectData &writerConfiguration) {
	m_writerConfiguration = writerConfiguration;
}

void MonteCarloTask::setGeneratorConfiguration(
		const PARTONS::BaseObjectData &generatorConfiguration) {
	m_generatorConfiguration = generatorConfiguration;
}

const PARTONS::BaseObjectData &
MonteCarloTask::getGeneratorConfiguration() const {
	return m_generatorConfiguration;
}

} /* namespace EPIC */

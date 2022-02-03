/*
 * GenerationInformation.cpp
 *
 *  Created on: Mar 24, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../include/beans/containers/GenerationInformation.h"

#include <ElementaryUtils/string_utils/Formatter.h>

namespace EPIC {

GenerationInformation::GenerationInformation() :
		PARTONS::BaseObject("GenerationInformation"), m_nEvents(0), m_initializationTime(
				0.), m_generationTime(0.) {
}

GenerationInformation::GenerationInformation(const GenerationInformation &other) :
		PARTONS::BaseObject(other), m_nEvents(other.m_nEvents), m_initializationTime(
				other.m_initializationTime), m_generationTime(
				other.m_generationTime) {

	m_generatorName = other.m_generatorName;
	m_generatorVersion = other.m_generatorVersion;
	m_description = other.m_description;

	m_serviceName = other.m_serviceName;
	m_integratedCrossSection = other.m_integratedCrossSection;
	m_generationDate = other.m_generationDate;
}

GenerationInformation::~GenerationInformation() {
}

std::string GenerationInformation::toString() const {

	ElemUtils::Formatter formatter;

	formatter << '\n';
	formatter << "Generator name: " << m_generatorName << '\n';
	formatter << "Generator version: " << m_generatorVersion << '\n';
	formatter << "Description: " << m_description << '\n';

	formatter << "Service name: " << m_serviceName << '\n';
	formatter << "Number of generated events: " << m_nEvents << '\n';
	formatter << "Integrated cross-section (value) [nb]: "
			<< m_integratedCrossSection.first << '\n';
	formatter << "Integrated cross-section (uncertainty) [nb]: "
			<< m_integratedCrossSection.second << '\n';
	formatter << "Generation date: " << m_generationDate << '\n';

	formatter << "Initialization time [s]: " << m_initializationTime << '\n';
	formatter << "Generation time (total) [s]: " << m_generationTime << '\n';
	formatter << "Generation time (per event) [s]: " << m_generationTime/double(m_nEvents);

	return formatter.str();
}

const std::pair<double, double>& GenerationInformation::getIntegratedCrossSection() const {
	return m_integratedCrossSection;
}

void GenerationInformation::setIntegratedCrossSection(
		const std::pair<double, double>& integratedCrossSection) {
	m_integratedCrossSection = integratedCrossSection;
}

const std::string& GenerationInformation::getDescription() const {
	return m_description;
}

void GenerationInformation::setDescription(const std::string& description) {
	m_description = description;
}

const std::string& GenerationInformation::getGeneratorName() const {
	return m_generatorName;
}

void GenerationInformation::setGeneratorName(const std::string& generatorName) {
	m_generatorName = generatorName;
}

const std::string& GenerationInformation::getGeneratorVersion() const {
	return m_generatorVersion;
}

void GenerationInformation::setGeneratorVersion(
		const std::string& generatorVersion) {
	m_generatorVersion = generatorVersion;
}

const std::string& GenerationInformation::getServiceName() const {
	return m_serviceName;
}

void GenerationInformation::setServiceName(const std::string& serviceName) {
	m_serviceName = serviceName;
}

int GenerationInformation::getNEvents() const {
	return m_nEvents;
}

void GenerationInformation::setNEvents(int nEvents) {
	m_nEvents = nEvents;
}

const std::string& GenerationInformation::getGenerationDate() const {
	return m_generationDate;
}

void GenerationInformation::setGenerationDate(
		const std::string& generationDate) {
	m_generationDate = generationDate;
}

double GenerationInformation::getGenerationTime() const {
	return m_generationTime;
}

void GenerationInformation::setGenerationTime(double generationTime) {
	m_generationTime = generationTime;
}

double GenerationInformation::getInitializationTime() const {
	return m_initializationTime;
}

void GenerationInformation::setInitializationTime(double initializationTime) {
	m_initializationTime = initializationTime;
}

}
/* namespace EPIC */

/*
 * KinematicRange.cpp
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/KinematicRange.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <stddef.h>
#include <iostream>
#include <vector>

#include "../../../include/automation/MonteCarloTask.h"

namespace EPIC {

KinematicRange::KinematicRange() :
		PARTONS::BaseObject("KinematicRange") {
	m_minMax = std::make_pair(0., 0.);
}

KinematicRange::KinematicRange(double min, double max) :
		PARTONS::BaseObject("KinematicRange") {
	setMinMax(min, max);
}

KinematicRange::KinematicRange(const std::pair<double, double> &minMax) :
		PARTONS::BaseObject("KinematicRange") {
	setMinMax(minMax);
}

KinematicRange::KinematicRange(const std::vector<double> &minMax) :
		PARTONS::BaseObject("KinematicRange") {
	setMinMax(minMax);
}

KinematicRange::KinematicRange(const KinematicRange &other) :
		PARTONS::BaseObject(other) {
	m_minMax = other.m_minMax;
}

KinematicRange::~KinematicRange() {
}

std::string KinematicRange::toString() const {

	ElemUtils::Formatter formatter;

	formatter << "min: " << m_minMax.first << " max: " << m_minMax.second;

	return formatter.str();
}

bool KinematicRange::inRange(double value) const {
	return ((value >= m_minMax.first) && (value <= m_minMax.second));
}

void KinematicRange::checkIfValid() const {
	if (m_minMax.first > m_minMax.second) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter() << "Kinematic range is not valid: "
						<< toString());
	}
}

const std::pair<double, double> &KinematicRange::getMinMax() const {
	return m_minMax;
}

double KinematicRange::getMin() const {
	return m_minMax.first;
}

double KinematicRange::getMax() const {
	return m_minMax.second;
}

void KinematicRange::setMinMax(double min, double max) {

	m_minMax = std::make_pair(min, max);
	checkIfValid();
}

void KinematicRange::setMinMax(const std::pair<double, double> &minMax) {

	m_minMax = minMax;
	checkIfValid();
}

void KinematicRange::setMinMax(const std::vector<double> &minMax) {

	if (minMax.size() != 2) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter() << "Wrong vector size: "
						<< minMax.size());
	}

	setMinMax(std::make_pair(minMax.at(0), minMax.at(1)));
}

} /* namespace EPIC */

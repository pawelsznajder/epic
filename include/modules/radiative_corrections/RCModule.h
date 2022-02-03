/*
 * RCModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RADIATIVECORRECTIONS_RCMODULE_H_
#define MODULES_RADIATIVECORRECTIONS_RCMODULE_H_

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>
#include <stddef.h>
#include <string>
#include <tuple>
#include <vector>

#include "../../beans/containers/ExperimentalConditions.h"
#include "../../beans/containers/KinematicRange.h"
#include "../EpicModuleObject.h"

namespace EPIC {
class Event;
} /* namespace EPIC */

namespace EPIC {
class ExperimentalConditions;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class RCModule
 *
 * @brief Template for radiative correction module.
 *
 * This is a template for radiative correction modules.
 */
template<typename ObsKinType>
class RCModule: public EpicModuleObject {

public:

	/**
	 * Destructor.
	 */
	virtual ~RCModule() {
	}

	/**
	 * Get number of variables.
	 */
	size_t getNVariables() const;

	/**
	 * Get ranges of variables.
	 */
	const std::vector<KinematicRange>& getVariableRanges() const {
		return m_variableRanges;
	}

	/**
	 * Get radiative function, true experimental conditions and kinematics.
	 */
	virtual std::tuple<double, ExperimentalConditions, ObsKinType> evaluate(
			const ExperimentalConditions& experimentalConditions,
			const ObsKinType& obsKin, const std::vector<double>& par) const = 0;

	/**
	 * Update event.
	 */
	virtual void updateEvent(Event& event, const std::vector<double>& par) const = 0;

protected:

	/**
	 * Constructor.
	 * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
	 *
	 * @param className name of child class.
	 * @param channelType Channel type.
	 */
	RCModule(const std::string &className,
			PARTONS::ChannelType::Type channelType) :
			EpicModuleObject(className, channelType), m_nVariables(0) {
	}

	/**
	 * Copy constructor
	 * @param other Object to be copied.
	 */
	RCModule(const RCModule &other) :
			EpicModuleObject(other), m_nVariables(other.m_nVariables), m_variableRanges(
					other.m_variableRanges) {
	}

protected:

	/**
	 * Check parameters.
	 */
	virtual void checkParameters(const std::vector<double>& par) const {

		if (par.size() != m_nVariables) {
			throw ElemUtils::CustomException(getClassName(), __func__,
					ElemUtils::Formatter() << "Wrong number of variables, is: "
							<< par.size() << ", expected: " << m_nVariables);
		}
	}

	/**
	 * Get ranges of variables.
	 * Number of variables is deduced from the input.
	 */
	void setVariableRanges(const std::vector<KinematicRange>& variableRanges) {

		m_nVariables = variableRanges.size();
		m_variableRanges = variableRanges;
	}

private:

	size_t m_nVariables; ///< Number of variables module depends on.
	std::vector<KinematicRange> m_variableRanges; ///< Ranges of variables.
};
}

#endif /* MODULES_RADIATIVECORRECTIONS_RCMODULE_H_ */

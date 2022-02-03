/*
 * EventGeneratorModule.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_EVENTGENERATOR_EVENTGENERATORMODULE_H_
#define MODULES_EVENTGENERATOR_EVENTGENERATORMODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <utility>
#include <vector>

#include "../../beans/containers/KinematicRange.h"
#include "../EpicModuleObject.h"

namespace EPIC {
class EventGeneratorInterface;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class EventGeneratorModule
 *
 * @brief Definition of module for generation of events from a given
 * distribution.
 *
 * This class provides a definition of modules used to generate events from
 * given distributions.
 */
class EventGeneratorModule: public EpicModuleObject {

public:

	static const std::string EVENT_GENERATOR_MODULE_CLASS_NAME; ///< Class name used in parsing XML.
	static const std::string EVENT_GENERATOR_MODULE_INIT_STATE_PATH; ///< String used to set path to init state file type via XML scenario.

	/**
	 * Constructor.
	 */
	EventGeneratorModule(const std::string &className);

	/**
	 * Copy constructor.
	 */
	EventGeneratorModule(const EventGeneratorModule &other);

	/**
	 * Destructor.
	 */
	virtual ~EventGeneratorModule();

	virtual void configure(const ElemUtils::Parameters &parameters);

	virtual bool runTest() const;

	/**
	 * Initialize.
	 */
	virtual void initialise(const std::vector<KinematicRange> &kinematicRanges,
			const EventGeneratorInterface &service) = 0;

	/**
	 * Generate event.
	 */
	virtual std::pair<std::vector<double>, double> generateEvent() = 0;

	/**
	 * Get integral over distribution and integrated error.
	 */
	virtual std::pair<double, double> getIntegral() = 0;

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

	/**
	 * Get path to state file.
	 */
	const std::string& getInitStatePath() const;

	/**
	 * Set path to state file.
	 */
	void setInitStatePath(const std::string& initStatePath);

protected:

	virtual void initModule();
	virtual void isModuleWellConfigured();

	std::string m_initStatePath; ///< Path to file to either load (if exist) or save (if not) initial state of generator.
};

} /* namespace EPIC */

#endif /* MODULES_EVENTGENERATOR_EVENTGENERATORMODULE_H_ */

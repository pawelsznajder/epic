/*
 * MonteCarloTask.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef AUTOMATION_MONTECARLOTASK_H_
#define AUTOMATION_MONTECARLOTASK_H_

#include <partons/beans/automation/BaseObjectData.h>
#include <string>

namespace EPIC {

/**
 * @class MonteCarloTask
 *
 * @brief Container to store information from xml scenario for a single task.
 *
 * This class contains
 */
class MonteCarloTask: public PARTONS::BaseObject {

public:
	/**
	 * Default constructor.
	 */
	MonteCarloTask();

	/**
	 * Copy constructor.
	 */
	MonteCarloTask(const MonteCarloTask &other);

	/**
	 * Destructor.
	 */
	virtual ~MonteCarloTask();

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

	/**
	 * Get MonteCarloTask::m_serviceName.
	 */
	const std::string &getServiceName() const;

	/**
	 * Set MonteCarloTask::m_serviceName.
	 */
	void setServiceName(const std::string &serviceName);

	/**
	 * Get MonteCarloTask::m_methodName.
	 */
	const std::string &getMethodName() const;

	/**
	 * Set MonteCarloTask::m_methodName.
	 */
	void setMethodName(const std::string &methodName);

	/**
	 * Get MonteCarloTask::m_generalConfiguration.
	 */
	const PARTONS::BaseObjectData &getGeneralConfiguration() const;

	/**
	 * Set MonteCarloTask::m_generalConfiguration.
	 */
	void
	setGeneralConfiguration(
			const PARTONS::BaseObjectData &generalConfiguration);

	/**
	 * Get MonteCarloTask::m_kinematicRange.
	 */
	const PARTONS::BaseObjectData &getKinematicRange() const;

	/**
	 * Set MonteCarloTask::m_kinematicRange.
	 */
	void setKinematicRange(const PARTONS::BaseObjectData &kinematicRange);

	/**
	 * Get MonteCarloTask::m_experimentalConditions.
	 */
	const PARTONS::BaseObjectData &getExperimentalConditions() const;

	/**
	 * Set MonteCarloTask::m_experimentalConditions.
	 */
	void setExperimentalConditions(
			const PARTONS::BaseObjectData &experimentalConditions);

	/**
	 * Get MonteCarloTask::m_computationConfiguration.
	 */
	const PARTONS::BaseObjectData &getComputationConfiguration() const;

	/**
	 * Set MonteCarloTask::m_generatorConfiguration.
	 */
	void setGeneratorConfiguration(
			const PARTONS::BaseObjectData &generatorConfiguration);

	/**
	 * Get MonteCarloTask::m_generatorConfiguration.
	 */
	const PARTONS::BaseObjectData &getGeneratorConfiguration() const;

	/**
	 * Set MonteCarloTask::m_computationConfiguration.
	 */
	void setComputationConfiguration(
			const PARTONS::BaseObjectData &computationConfiguration);

	/**
	 * Get MonteCarloTask::m_kinematicConfiguration.
	 */
	const PARTONS::BaseObjectData &getKinematicConfiguration() const;

	/**
	 * Set MonteCarloTask::m_kinematicConfiguration.
	 */
	void setKinematicConfiguration(
			const PARTONS::BaseObjectData &kinematicConfiguration);

	/**
	 * Get MonteCarloTask::m_rcConfiguration.
	 */
	const PARTONS::BaseObjectData &getRCConfiguration() const;

	/**
	 * Set MonteCarloTask::m_rcConfiguration.
	 */
	void setRCConfiguration(const PARTONS::BaseObjectData &rcConfiguration);

	/**
	 * Get MonteCarloTask::m_writerConfiguration.
	 */
	const PARTONS::BaseObjectData &getWriterConfiguration() const;

	/**
	 * Set MonteCarloTask::m_writerConfiguration.
	 */
	void
	setWriterConfiguration(const PARTONS::BaseObjectData &writerConfiguration);

private:
	std::string m_serviceName; ///<  Name of service responsible for running this
							   /// scenario.
	std::string m_methodName;  ///< Name of method to be evaluated by this task.

	PARTONS::BaseObjectData m_generalConfiguration; ///< Base object data stored in xml block given by
	/// MonteCarloTask::GENERAL_CONFIGURATION_NODE_NAME
	/// tag.
	PARTONS::BaseObjectData m_kinematicRange; ///< Base object data stored in xml block given by
											  /// MonteCarloTask::KINEMATIC_RANGE_NODE_NAME tag.
	PARTONS::BaseObjectData m_experimentalConditions; ///< Base object data stored in xml block given
													  /// by
	/// MonteCarloTask::EXPERIMENTAL_CONDIDTION_NODE_NAME
	/// tag.
	PARTONS::BaseObjectData m_computationConfiguration; ///< Base object data stored in xml block
														/// given by
	/// MonteCarloTask::COMPUTATION_CONFIGURATIOM_NODE_NAME
	/// tag.

	PARTONS::BaseObjectData m_generatorConfiguration; ///< Base object data stored in xml block given
													  /// by
	/// MonteCarloTask::GENERATOR_CONFIGURATION_NODE_NAME
	/// tag.

	PARTONS::BaseObjectData m_kinematicConfiguration; ///< Base object data stored in xml block given
													  /// by
	/// MonteCarloTask::KINEMATIC_CONFIGURATION_NODE_NAME
	/// tag.

	PARTONS::BaseObjectData m_rcConfiguration; ///< Base object data stored in xml block given
											   /// by
	/// MonteCarloTask::RC_CONFIGURATION_NODE_NAME
	/// tag.

	PARTONS::BaseObjectData m_writerConfiguration; ///< Base object data stored in xml block given by
												   /// MonteCarloTask::WRITER_CONFIGURATION_NODE_NAME
	/// tag.
};

} /* namespace EPIC */

#endif /* AUTOMATION_MONTECARLOTASK_H_ */

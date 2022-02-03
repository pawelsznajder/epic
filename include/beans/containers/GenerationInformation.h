/*
 * GenerationInformation.h
 *
 *  Created on: Mar 24, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef BEANS_GENERATIONINFORMATION_H_
#define BEANS_GENERATIONINFORMATION_H_

#include <partons/BaseObject.h>
#include <string>
#include <utility>

namespace EPIC {

/**
 * @class GenerationInformation
 *
 * @brief Container to store run related information.
 *
 * This class acts as a container to store information related to the run, like e.g. cross-section over probed phase-space or software version.
 */
class GenerationInformation: public PARTONS::BaseObject {

public:

	/**
	 * Default constructor.
	 */
	GenerationInformation();

	/**
	 * Copy constructor.
	 */
	GenerationInformation(const GenerationInformation &other);

	/**
	 * Destructor.
	 */
	virtual ~GenerationInformation();

	virtual std::string toString() const;

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

	/**
	 * Get generator name.
	 */
	const std::string& getGeneratorName() const;

	/**
	 * Set generator name.
	 */
	void setGeneratorName(const std::string& generatorName);

	/**
	 * Get generator version.
	 */
	const std::string& getGeneratorVersion() const;

	/**
	 * Set generator version.
	 */
	void setGeneratorVersion(const std::string& generatorVersion);

	/**
	 * Get description.
	 */
	const std::string& getDescription() const;

	/**
	 * Set description.
	 */
	void setDescription(const std::string& description);

	/**
	 * Get service name.
	 */
	const std::string& getServiceName() const;

	/**
	 * Set service name.
	 */
	void setServiceName(const std::string& serviceName);

	/**
	 * Get number of generated events.
	 */
	int getNEvents() const;

	/**
	 * Set number of generated events.
	 */
	void setNEvents(int nEvents);

	/**
	 * Get integrated cross-section.
	 */
	const std::pair<double, double>& getIntegratedCrossSection() const;

	/**
	 * Set integrated cross-section.
	 */
	void setIntegratedCrossSection(
			const std::pair<double, double>& integratedCrossSection);

	/**
	 * Get generation date.
	 */
	const std::string& getGenerationDate() const;

	/**
	 * Set generation date.
	 */
	void setGenerationDate(const std::string& generationDate);

	/**
	 * Get generation time (in seconds).
	 */
	double getGenerationTime() const;

	/**
	 * Set generation time (in seconds).
	 */
	void setGenerationTime(double generationTime);

	/**
	 * Get initialization time (in seconds).
	 */
	double getInitializationTime() const;

	/**
	 * Set initialization time (in seconds).
	 */
	void setInitializationTime(double initializationTime);

private:

	std::string m_generatorName; ///< Generator name.
	std::string m_generatorVersion; ///< Generator version.
	std::string m_description; ///< Generator description.

	std::string m_serviceName; ///< Service name.
	int m_nEvents; ///< Number of generated events.
	std::pair<double, double> m_integratedCrossSection; ///< Integrated cross-section (value and unc.).
	std::string m_generationDate; ///< Generation date.

	double m_initializationTime; ///< Time spent for initialization (in seconds).
	double m_generationTime; ///< Time spent for generation (in seconds).
};

} /* namespace EPIC */

#endif /* BEANS_GENERATIONINFORMATION_H_ */

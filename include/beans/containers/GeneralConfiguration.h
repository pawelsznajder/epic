/*
 * GeneralConfiguration.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_GENERALCONFIGURATION_H_
#define BEANS_GENERALCONFIGURATION_H_

#include <partons/BaseObject.h>
#include <stddef.h>
#include <string>

namespace EPIC {
class MonteCarloTask;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class GeneralConfiguration
 *
 * @brief Container to store general configuration of generator.
 *
 * This class acts as a container to store general configuration of generator,
 * like number of events to be generated.
 */
class GeneralConfiguration: public PARTONS::BaseObject {

    static const std::string GENERAL_CONFIGURATION_NUMBER_OF_EVENTS; ///< Key to set number of events.
    static const std::string GENERAL_CONFIGURATION_HISTOGRAM_FILE_PATH; ///< Key to set path to file storing debugging histograms.

public:
    /**
     * Default constructor.
     */
    GeneralConfiguration();

    /**
     * Assignment constructor.
     */
    GeneralConfiguration(size_t nEvents, const std::string& histogramFilePath);

    /**
     * Copy constructor.
     */
    GeneralConfiguration(const GeneralConfiguration &other);

    /**
     * Destructor.
     */
    virtual ~GeneralConfiguration();

    virtual std::string toString() const;

    /**
     * Build from task.
     */
    static GeneralConfiguration fromTask(const MonteCarloTask &task);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get number of events.
     */
    size_t getNEvents() const;

    /**
     * Set number of events.
     */
    void setNEvents(size_t nEvents);

    /**
     * Get path to file storing debugging histograms.
     */
    const std::string& getHistogramFilePath() const;

    /**
     * Set path to file storing debugging histograms.
     */
    void setHistogramFilePath(const std::string& histogramFilePath);

private:

    size_t m_nEvents; ///< Number of events.
    std::string m_histogramFilePath; ///< Path to file storing debugging histograms.
};

} /* namespace EPIC */

#endif /* BEANS_GENERALCONFIGURATION_H_ */

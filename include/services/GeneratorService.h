/*
 * GeneratorService.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_GENERATORSERVICE_H_
#define SERVICES_GENERATORSERVICE_H_

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <ctime>
#include <memory>
#include <string>
#include <chrono>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "../automation/MonteCarloTask.h"
#include "../beans/containers/ExperimentalConditions.h"
#include "../beans/containers/GeneralConfiguration.h"
#include "../beans/containers/GenerationInformation.h"
#include "../beans/other/EventGeneratorInterface.h"
#include "../Epic.h"
#include "../EpicVersion.h"
#include "../managers/ModuleObjectFactory.h"
#include "../modules/event_generator/EventGeneratorModule.h"
#include "../modules/writer/WriterModule.h"
#include "../modules/random_generator/RandomNumberGSL.h"

namespace EPIC {

/**
 * @class GeneratorService
 *
 * @brief Template for services handling generation of MC events.
 *
 * This is a template used to create services handling generation of MC events.
 */
template<class kinRange, class procModule, class kinModule,
        class rcModule>
class GeneratorService: public PARTONS::BaseObject,
        public EventGeneratorInterface {

public:
    /**
     * Default constructor.
     */
    GeneratorService(const std::string &className) :
            PARTONS::BaseObject(className), EventGeneratorInterface(), m_pProcessModule(
                    nullptr), m_pEventGeneratorModule(nullptr), m_pKinematicModule(
                    nullptr), m_pRCModule(nullptr), m_pWriterModule(nullptr), m_histogramFile(
                    nullptr) {
        m_randomNumberModule =
                Epic::getInstance()->getModuleObjectFactory()->newRandomNumberModule(
                        RandomNumberGSL::classId);
    }

    /**
     * Copy constructor.
     */
    GeneratorService(const GeneratorService &other) :
            PARTONS::BaseObject(other), EventGeneratorInterface(other), m_pProcessModule(
                    other.m_pProcessModule), m_pEventGeneratorModule(
                    other.m_pEventGeneratorModule), m_pKinematicModule(
                    other.m_pKinematicModule), m_pRCModule(other.m_pRCModule), m_pWriterModule(
                    other.m_pWriterModule) {

        m_generalConfiguration = other.m_generalConfiguration;
        m_kinematicRanges = other.m_kinematicRanges;
        m_experimentalConditions = other.m_experimentalConditions;

        m_debugTimeInitialization = other.m_debugTimeInitialization;
        m_debugTimeGeneration = other.m_debugTimeGeneration;

        if (other.m_randomNumberModule == nullptr) {
            m_randomNumberModule = nullptr;
        } else {
            m_randomNumberModule = std::make_shared<RandomNumberGSL>(
                    RandomNumberGSL(
                            *(std::static_pointer_cast<RandomNumberGSL>(
                                    other.m_randomNumberModule))));
        }

        if (other.m_histogramFile == nullptr) {
            m_histogramFile = nullptr;
        } else {
            m_histogramFile = TFile::Open(other.m_histogramFile->GetName(),
                    "RECREATE", "EpIC debugging histograms");
        }
    }

    virtual GeneratorService *clone() const = 0;

    /**
     * Destructor.
     */
    virtual ~GeneratorService() {
    }

    /**
     * Configure and run whole computation.
     */
    void computeTask(const MonteCarloTask &task) {

        getGeneralConfigurationFromTask(task);
        getAdditionalGeneralConfigurationFromTask(task);
        getExperimentalConditionsFromTask(task);
        getKinematicRangesFromTask(task);
        getProcessModuleFromTask(task);
        getEventGeneratorModuleFromTask(task);
        getKinematicModuleFromTask(task);
        getRCModuleFromTask(task);
        getWriterModuleFromTask(task);

        openHistogramFile();

        run();

        closeHistogramFile();
    }

    /**
     * Run/
     */
    virtual void run() = 0;

    /**
     * Save generation configuration. Must be invoked when writer is open.
     */
    void printAndSaveGenerationConfiguration() {

        GenerationInformation generationInformation;

        generationInformation.setGeneratorName("EPIC");
        generationInformation.setGeneratorVersion(
                ElemUtils::Formatter() << EPIC_VERSION_MAJOR << "."
                << EPIC_VERSION_MINOR << "." << EPIC_VERSION_PATCH);
        generationInformation.setDescription(
                m_scenarioDate + " | " + m_scenarioDescription);

        generationInformation.setServiceName(getClassName());
        generationInformation.setNEvents(m_generalConfiguration.getNEvents());
        generationInformation.setIntegratedCrossSection(
                m_pEventGeneratorModule->getIntegral());
        generationInformation.setGenerationDate(getCurrentDate());

        generationInformation.setInitializationTime(
                1.E-6
                        * std::chrono::duration_cast<std::chrono::microseconds>(
                                m_debugTimeInitialization.second
                                        - m_debugTimeInitialization.first).count());
        generationInformation.setGenerationTime(
                1.E-6
                        * std::chrono::duration_cast<std::chrono::microseconds>(
                                m_debugTimeGeneration.second
                                        - m_debugTimeGeneration.first).count());

        addAdditionalGenerationConfiguration(generationInformation);

        m_pWriterModule->saveGenerationInformation(generationInformation);

        info(__func__,
                ElemUtils::Formatter() << "Generation information:\n"
                        << generationInformation.toString() << '\n');
    }

    /**
     * Add additional information to GenerationInformation printout.
     */
    virtual void addAdditionalGenerationConfiguration(
            GenerationInformation& generationInformation) {

        std::stringstream ss;

        ss << m_experimentalConditions.getLeptonHelicity();
        generationInformation.addAdditionalInfo(
                std::make_pair("lepton_polarisation", ss.str()));

        ss.str("");
        ss.clear();
        ss << m_experimentalConditions.getHadronPolarisation().getX() << '|'
                << m_experimentalConditions.getHadronPolarisation().getY()
                << '|'
                << m_experimentalConditions.getHadronPolarisation().getZ();
        generationInformation.addAdditionalInfo(
                std::make_pair("hadron_polarisation", ss.str()));
    }

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get general configurations.
     */
    const GeneralConfiguration &getGeneralConfiguration() const {
        return m_generalConfiguration;
    }

    /**
     * Set general configuration.
     */
    void setGeneralConfiguration(
            const GeneralConfiguration &generalConfiguration) {
        m_generalConfiguration = generalConfiguration;
    }

    /**
     * Get kinematic ranges.
     */
    const kinRange &getKinematicRanges() const {
        return m_kinematicRanges;
    }

    /**
     * Set kinematic ranges.
     */
    void setKinematicRanges(const kinRange &kinematicRanges) {
        m_kinematicRanges = kinematicRanges;
    }

    /**
     * Get experimental conditions.
     */
    const ExperimentalConditions &getExperimentalConditions() const {
        return m_experimentalConditions;
    }

    /**
     * Set experimental conditions.
     */
    void setExperimentalConditions(
            const ExperimentalConditions &experimentalConditions) {
        m_experimentalConditions = experimentalConditions;
    }

    /**
     * Get process module.
     *
     */
    procModule *getProcessModule() const {
        return m_pProcessModule;
    }

    /**
     * Set process module.
     */
    void setProcessModule(procModule *processModule) {
        m_pProcessModule = processModule;
    }

    /**
     * Get event generator module.
     */
    const std::shared_ptr<EventGeneratorModule> &getEventGeneratorModule() const {
        return m_pEventGeneratorModule;
    }

    /**
     * Set event generator module.
     */
    void setEventGeneratorModule(
            const std::shared_ptr<EventGeneratorModule> &pEventGeneratorModule) {
        m_pEventGeneratorModule = pEventGeneratorModule;
    }

    /**
     * Get kinematic module.
     */
    const std::shared_ptr<kinModule> &getKinematicModule() const {
        return m_pKinematicModule;
    }

    /**
     * Set kinematic module.
     */
    void setKinematicModule(
            const std::shared_ptr<kinModule> &pKinematicModule) {
        m_pKinematicModule = pKinematicModule;
    }

    /**
     * Get radiative correction module.
     */
    const std::shared_ptr<rcModule> &getRCModule() const {
        return m_pRCModule;
    }

    /**
     * Set radiative correction module.
     */
    void setRCModule(const std::shared_ptr<rcModule> &pRCModule) {
        m_pRCModule = pRCModule;
    }

    /**
     * Get writer module.
     */
    const std::shared_ptr<WriterModule> &getWriterModule() const {
        return m_pWriterModule;
    }

    /**
     * Set writer module.
     */
    void setWriterModule(const std::shared_ptr<WriterModule> &pWriterModule) {
        m_pWriterModule = pWriterModule;
    }

    /**
     * Get scenario description.
     */
    const std::string& getScenarioDescription() const {
        return m_scenarioDescription;
    }

    /**
     * Set scenario description.
     */
    void setScenarioDescription(const std::string& scenarioDescription) {
        m_scenarioDescription = scenarioDescription;
    }

    /**
     * Get scenario date.
     */
    const std::string& getScenarioDate() const {
        return m_scenarioDate;
    }

    /**
     * Set scenario date.
     */
    void setScenarioDate(const std::string& scenarioDate) {
        m_scenarioDate = scenarioDate;
    }

protected:

    /**
     * Get general configuration from XML.
     */
    void getGeneralConfigurationFromTask(const MonteCarloTask &task) {

        m_generalConfiguration = GeneralConfiguration::fromTask(task);

        info(__func__,
                ElemUtils::Formatter() << "General configuration:\n"
                        << m_generalConfiguration.toString());
    }

    /**
     * Get additional general configuration from XML.
     */
    virtual void getAdditionalGeneralConfigurationFromTask(
            const MonteCarloTask &task) = 0;

    /**
     * Get experimental conditions from XML.
     */
    void getExperimentalConditionsFromTask(const MonteCarloTask &task) {

        m_experimentalConditions = ExperimentalConditions::fromTask(task);

        info(__func__,
                ElemUtils::Formatter() << "Experimental conditions:\n"
                        << m_experimentalConditions.toString());
    }

    /**
     * Get kinematic range from XML.
     */
    virtual void getKinematicRangesFromTask(const MonteCarloTask &task) = 0;

    /**
     * Get process module from XML.
     */
    virtual void getProcessModuleFromTask(const MonteCarloTask &task) = 0;

    /**
     * Get event generator module from XML.
     */
    void getEventGeneratorModuleFromTask(const MonteCarloTask &task) {

        // check if available
        if (ElemUtils::StringUtils::equals(
                task.getGeneratorConfiguration().getModuleType(),
                EventGeneratorModule::EVENT_GENERATOR_MODULE_CLASS_NAME)) {

            // configure
            m_pEventGeneratorModule =
                    Epic::getInstance()->getModuleObjectFactory()->newEventGeneratorModule(
                            task.getGeneratorConfiguration().getModuleClassName());

            m_pEventGeneratorModule->configure(
                    task.getGeneratorConfiguration().getParameters());

            m_pEventGeneratorModule->prepareSubModules(
                    task.getGeneratorConfiguration().getSubModules());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "You have not provided any "
                            << EventGeneratorModule::EVENT_GENERATOR_MODULE_CLASS_NAME);
        }

        info(__func__,
                ElemUtils::Formatter() << "Event generator module: "
                        << m_pEventGeneratorModule->toString());
    }

    /**
     * Get kinematic module from XML.
     */
    virtual void getKinematicModuleFromTask(const MonteCarloTask &task) = 0;

    /**
     * Get radiative correction module from XML.
     */
    virtual void getRCModuleFromTask(const MonteCarloTask &task) = 0;

    /**
     * Get writer module from XML.
     */
    void getWriterModuleFromTask(const MonteCarloTask &task) {

        // check if available
        if (ElemUtils::StringUtils::equals(
                task.getWriterConfiguration().getModuleType(),
                WriterModule::WRITER_MODULE_CLASS_NAME)) {

            // configure
            m_pWriterModule =
                    Epic::getInstance()->getModuleObjectFactory()->newWriterModule(
                            task.getWriterConfiguration().getModuleClassName());

            m_pWriterModule->configure(
                    task.getWriterConfiguration().getParameters());

            m_pWriterModule->prepareSubModules(
                    task.getWriterConfiguration().getSubModules());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "You have not provided any "
                            << WriterModule::WRITER_MODULE_CLASS_NAME);
        }

        info(__func__,
                ElemUtils::Formatter() << "Writer module: "
                        << m_pWriterModule->toString());
    }

    /**
     * Check if well configured.
     */
    virtual void isServiceWellConfigured() const {

        if (m_pProcessModule == nullptr) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Pointer to process module in null");
        }

        if (m_pKinematicModule == nullptr) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Pointer to kineamtic module in null");
        }

        if (m_pRCModule == nullptr) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Pointer to radiative correction module in null");
        }

        if (m_pEventGeneratorModule == nullptr) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Pointer to event generator module in null");
        }

        if (m_pWriterModule == nullptr) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Pointer to writer module in null");
        }

        if (m_generalConfiguration.getNEvents() == 0) {
            warn(__func__,
                    "Number of events to be generated is: 0");
        }
    }

    /**
     * Get current date.
     */
    std::string getCurrentDate() const {

        time_t now = time(0);
        return std::string(ctime(&now));
    }

    /**
     * Check target polarisation state. If not longitidinal and transverse, generate randomly polarisation state (in any direction).
     */
    void checkTargetPolarisation(
            ExperimentalConditions& experimentalConditions) const {

        //get
        const NumA::Vector3D& currentHadronPolarisation =
                experimentalConditions.getHadronPolarisation();

        //check
        if (fabs(currentHadronPolarisation.getX()) == 1.
                || fabs(currentHadronPolarisation.getY()) == 1.
                || fabs(currentHadronPolarisation.getZ()) == 1.)
            return;

        //generate
        double cosThetaPol = m_randomNumberModule->diceFlat(-1., 1.);
        double thetaPol = acos(cosThetaPol);
        double phiPol = m_randomNumberModule->diceFlat(0., 2 * M_PI);

        //set
        experimentalConditions.setHadronPolarisation(
                NumA::Vector3D(sin(thetaPol) * cos(phiPol),
                        sin(thetaPol) * sin(phiPol), cos(thetaPol)));
    }

    /**
     * Transform variables.
     */
    virtual void transformVariables(std::vector<double>& variables) const = 0;  

    /**
     * Transform ranges.
     */
    virtual void transformRanges(std::vector<KinematicRange>& ranges) const = 0;

    /**
     * Get Jacobian related to the transformation of kinematic variables.
     */
    virtual double getJacobian(const std::vector<double>& variables) const = 0; 

    /**
     * Open histogram file.
     */
    void openHistogramFile() {

        m_histogramFile = TFile::Open(
                m_generalConfiguration.getHistogramFilePath().c_str(),
                "RECREATE", "EpIC debugging histograms");

        if (m_histogramFile->IsOpen()) {
            info(__func__,
                    ElemUtils::Formatter()
                            << "Histogram file created successfully: "
                            << m_generalConfiguration.getHistogramFilePath());

            bookHistograms();
        } else {
            warn(__func__,
                    ElemUtils::Formatter()
                            << "Failed to create histogram file: "
                            << m_generalConfiguration.getHistogramFilePath());
        }
    }

    /**
     * Book histograms.
     */
    virtual void bookHistograms() {
    }

    /**
     * Close histogram file.
     */
    void closeHistogramFile() {

        if (m_histogramFile->IsOpen()) {

            std::vector<TH1*>::const_iterator it;

            for (it = m_histograms.begin(); it != m_histograms.end(); it++) {
                m_histogramFile->WriteTObject(*it);
            }

            m_histogramFile->Close();

            info(__func__,
                    ElemUtils::Formatter() << "Histogram file closed after saving "
                            << m_histograms.size() << " histograms");
        }
    }

    kinRange m_kinematicRanges;                    ///< Kinematic ranges.
    procModule *m_pProcessModule;                  ///< Process module.
    std::shared_ptr<kinModule> m_pKinematicModule; ///< Kinematic module.
    std::shared_ptr<rcModule> m_pRCModule; ///< Radiative correction module.

    GeneralConfiguration m_generalConfiguration;     ///< General configuration.
    ExperimentalConditions m_experimentalConditions; ///< Experimental conditions.
    std::shared_ptr<EventGeneratorModule> m_pEventGeneratorModule; ///< Event generator module.
    std::shared_ptr<WriterModule> m_pWriterModule; ///< Writer module.

    std::string m_scenarioDate; ///< Scenario date.
    std::string m_scenarioDescription; ///< Scenario description.

    std::pair<std::chrono::steady_clock::time_point,
            std::chrono::steady_clock::time_point> m_debugTimeInitialization; ///< Timer spent for initialization.
    std::pair<std::chrono::steady_clock::time_point,
            std::chrono::steady_clock::time_point> m_debugTimeGeneration; ///< Timer spent for generation.

    std::shared_ptr<RandomNumberModule> m_randomNumberModule; ///< Random number module.

    TFile* m_histogramFile; ///< File containing histograms.
    std::vector<TH1*> m_histograms;
};

} /* namespace EPIC */

#endif /* SERVICES_GENERATORSERVICE_H_ */

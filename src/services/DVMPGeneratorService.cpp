/*
 * DVMPGeneratorService.cpp
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/services/DVMPGeneratorService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/utils/type/PhysicalType.h>
#include <partons/utils/type/PhysicalUnit.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <stddef.h>
#include <chrono>
#include <cmath>
#include <iterator>
#include <memory>
#include <tuple>
#include <utility>

#include "../../include/automation/MonteCarloTask.h"
#include "../../include/beans/containers/KinematicRange.h"
#include "../../include/beans/types/EventAttributeType.h"
#include "../../include/Epic.h"
#include "../../include/managers/ModuleObjectFactory.h"
#include "../../include/modules/event_generator/EventGeneratorModule.h"
#include "../../include/modules/radiative_corrections/RCModule.h"
#include "../../include/modules/writer/WriterModule.h"

namespace EPIC {

const std::string DVMPGeneratorService::DVMP_GENERATOR_SERVICE_MESON_TYPE =
        "meson";
const std::string DVMPGeneratorService::DVMP_GENERATOR_SERVICE_MESON_POLARIZATION =
        "meson_polarisation";

const unsigned int DVMPGeneratorService::classId =
        PARTONS::Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DVMPGeneratorService("DVMPGeneratorService"));

DVMPGeneratorService::DVMPGeneratorService(const std::string &className) :
        GeneratorService<DVMPKinematicRanges, PARTONS::DVMPProcessModule,
                DVMPKinematicModule, DVMPRCModule>(className) {

    m_mesonType = ParticleType::UNDEFINED;
    m_mesonPolarization = PARTONS::PolarizationType::UNDEFINED;
}

DVMPGeneratorService::DVMPGeneratorService(const DVMPGeneratorService &other) :
        GeneratorService<DVMPKinematicRanges, PARTONS::DVMPProcessModule,
                DVMPKinematicModule, DVMPRCModule>(other) {

    m_mesonType = other.m_mesonType;
    m_mesonPolarization = other.m_mesonPolarization;
}

DVMPGeneratorService *DVMPGeneratorService::clone() const {
    return new DVMPGeneratorService(*this);
}

DVMPGeneratorService::~DVMPGeneratorService() {
}

double DVMPGeneratorService::getEventDistribution(
        std::vector<double> &kin) const {

    //transform
    transformVariables(kin);

    //observed kinematics
    DVMPKinematic partonsKinObs(kin.at(0), kin.at(1), kin.at(2),
            m_experimentalConditions.getLeptonEnergyFixedTargetEquivalent(),
            kin.at(3), kin.at(4), m_mesonType, m_mesonPolarization);

    //check range
    if (!m_kinematicRanges.inRange(m_experimentalConditions, partonsKinObs))
        return 0.;

    //rc
    std::vector<double> rcVariables;
    rcVariables.insert(std::end(rcVariables), std::begin(kin) + 5,
            std::end(kin));

    std::tuple<double, ExperimentalConditions, DVMPKinematic> rcTrue =
            m_pRCModule->evaluate(m_experimentalConditions, partonsKinObs,
                    rcVariables);

    //check if zero
    if (std::get<0>(rcTrue) == 0.)
        return 0.;

    //check if valid
    if (!m_pKinematicModule->checkIfValid(std::get<1>(rcTrue),
            std::get<2>(rcTrue))) {
        return 0.;
    }

    //PARTONS kinematics
    PARTONS::DVMPObservableKinematic dvmpObservableKinematic =
        std::get<2>(rcTrue).toPARTONSDVMPObservableKinematic();

    //evaluate
    double result =
            std::get<0>(rcTrue)
                    * m_pProcessModule->compute(
                            std::get<1>(rcTrue).getLeptonHelicity(),
                            ParticleType(std::get<1>(rcTrue).getLeptonType()).getCharge(),
                            std::get<1>(rcTrue).getHadronPolarisation(),
                            m_mesonPolarization,
                            dvmpObservableKinematic,
                            m_pProcessModule->getListOfAvailableGPDTypeForComputation()).getValue().makeSameUnitAs(
                            PARTONS::PhysicalUnit::NB).getValue();

    //jacobian
    result *= getJacobian(kin);
    result /= dvmpObservableKinematic.getQ2().getValue() / (2 * PARTONS::Constant::PROTON_MASS * dvmpObservableKinematic.getE().getValue() * pow(dvmpObservableKinematic.getXB().getValue(), 2));

    if ((! std::isfinite(result)) || (result < 0.)) {

        warn(__func__, ElemUtils::Formatter() <<
                "Value is " << result << ", setting zero instead, look for previous messages for a reason");
        result = 0.;
    }

    return result;
}

void DVMPGeneratorService::isServiceWellConfigured() const {

    GeneratorService<DVMPKinematicRanges, PARTONS::DVMPProcessModule,
            DVMPKinematicModule, DVMPRCModule>::isServiceWellConfigured();

    if (m_mesonType == ParticleType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Meson type is: "
                        << ParticleType(m_mesonType).toString());
    }
}

void DVMPGeneratorService::run() {

    //check if well configured
    isServiceWellConfigured();

    //kinematic ranges
    std::vector<KinematicRange> ranges = m_pKinematicModule->getKinematicRanges(m_experimentalConditions, m_kinematicRanges);

    transformRanges(ranges);

    ranges.insert(std::end(ranges),
            std::begin(m_pRCModule->getVariableRanges()),
            std::end(m_pRCModule->getVariableRanges()));

    //Initialize generator
    m_debugTimeInitialization.first = std::chrono::steady_clock::now();
    m_pEventGeneratorModule->initialise(ranges, *this);
    m_debugTimeInitialization.second = std::chrono::steady_clock::now();

    //open writer
    m_pWriterModule->open();

    //loop over events
    m_debugTimeGeneration.first = std::chrono::steady_clock::now();

    for (size_t i = 0; i < m_generalConfiguration.getNEvents(); i++) {

        //generate kinematics
        std::pair<std::vector<double>, double> eventVec =
                m_pEventGeneratorModule->generateEvent();

        //transform variables
        transformVariables(eventVec.first);

        //histogram
        fillHistograms(eventVec.first);

        //create kinematics object
        DVMPKinematic partonsKinObs(eventVec.first.at(0), eventVec.first.at(1),
                eventVec.first.at(2),
                m_experimentalConditions.getLeptonEnergyFixedTargetEquivalent(),
                eventVec.first.at(3), eventVec.first.at(4), m_mesonType,
                m_mesonPolarization);

        //rc
        std::vector<double> rcVariables;
        rcVariables.insert(std::end(rcVariables),
                std::begin(eventVec.first) + 5, std::end(eventVec.first));

        std::tuple<double, ExperimentalConditions, DVMPKinematic> rcTrue =
                m_pRCModule->evaluate(m_experimentalConditions, partonsKinObs,
                        rcVariables);

        //target polarisation
        checkTargetPolarisation(std::get<1>(rcTrue));

        //create event
        Event event = m_pKinematicModule->evaluate(std::get<1>(rcTrue),
                std::get<2>(rcTrue));

        //rc
        m_pRCModule->updateEvent(event, rcVariables);

        //add attributes
        event.addAttribute(EventAttributeType::ID, i);
        event.addAttribute(EventAttributeType::WEIGHT, eventVec.second);

        //write
        m_pWriterModule->write(event);
    }

    m_debugTimeGeneration.second = std::chrono::steady_clock::now();

    //general configuration
    printAndSaveGenerationConfiguration();

    //close
    m_pWriterModule->close();
}

void DVMPGeneratorService::getAdditionalGeneralConfigurationFromTask(
        const MonteCarloTask &task) {

    //formatter
    ElemUtils::Formatter formatter;

    formatter << "Additional general configuration:\n";
    formatter << '\n';

    // meson type
    if (task.getGeneralConfiguration().getParameters().isAvailable(
            DVMP_GENERATOR_SERVICE_MESON_TYPE)) {
        m_mesonType =
                ParticleType::fromString(
                        task.getGeneralConfiguration().getParameters().getLastAvailable().getString());
    }

    formatter << "Meson type: " << ParticleType(m_mesonType).toString() << '\n';

    // meson polarization
    if (task.getGeneralConfiguration().getParameters().isAvailable(
            DVMP_GENERATOR_SERVICE_MESON_POLARIZATION)) {
        m_mesonPolarization =
                PARTONS::PolarizationType::fromString(
                        task.getGeneralConfiguration().getParameters().getLastAvailable().getString());
    }

    formatter << "Meson polarisation: "
            << PARTONS::PolarizationType(m_mesonPolarization).toString()
            << '\n';

    info(__func__, formatter.str());
}

void DVMPGeneratorService::getKinematicRangesFromTask(
        const MonteCarloTask &task) {

    m_kinematicRanges = DVMPKinematicRanges::getDVMPKinematicRangesfromTask(task);

    info(__func__,
            ElemUtils::Formatter() << "Kinematic ranges:\n"
                    << m_kinematicRanges.toString() << '\n');
}

void DVMPGeneratorService::getProcessModuleFromTask(
        const MonteCarloTask &task) {

    // check if available
    if (ElemUtils::StringUtils::equals(
            task.getComputationConfiguration().getModuleType(),
            PARTONS::DVMPProcessModule::DVMP_PROCESS_MODULE_CLASS_NAME)) {

        // configure
        m_pProcessModule =
                PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPProcessModule(
                        task.getComputationConfiguration().getModuleClassName());

        m_pProcessModule->configure(
                task.getComputationConfiguration().getParameters());

        m_pProcessModule->prepareSubModules(
                task.getComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << PARTONS::DVMPProcessModule::DVMP_PROCESS_MODULE_CLASS_NAME);
    }

    info(__func__,
            ElemUtils::Formatter() << "Process module: "
                    << m_pProcessModule->getClassName());
}

void DVMPGeneratorService::getKinematicModuleFromTask(
        const MonteCarloTask &task) {

    // check if available
    if (ElemUtils::StringUtils::equals(
            task.getKinematicConfiguration().getModuleType(),
            DVMPKinematicModule::DVMP_KINEMATIC_MODULE_CLASS_NAME)) {

        // configure
        m_pKinematicModule =
                Epic::getInstance()->getModuleObjectFactory()->newDVMPKinematicModule(
                        task.getKinematicConfiguration().getModuleClassName());

        m_pKinematicModule->configure(
                task.getKinematicConfiguration().getParameters());

        m_pKinematicModule->prepareSubModules(
                task.getKinematicConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DVMPKinematicModule::DVMP_KINEMATIC_MODULE_CLASS_NAME);
    }

    info(__func__,
            ElemUtils::Formatter() << "Kinematic module: "
                    << m_pKinematicModule->getClassName());
}

void DVMPGeneratorService::getRCModuleFromTask(const MonteCarloTask &task) {

    // check if available
    if (ElemUtils::StringUtils::equals(
            task.getRCConfiguration().getModuleType(),
            DVMPRCModule::DVMP_RC_MODULE_CLASS_NAME)) {

        // configure
        m_pRCModule =
                Epic::getInstance()->getModuleObjectFactory()->newDVMPRCModule(
                        task.getRCConfiguration().getModuleClassName());

        m_pRCModule->configure(task.getRCConfiguration().getParameters());

        m_pRCModule->prepareSubModules(
                task.getRCConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DVMPRCModule::DVMP_RC_MODULE_CLASS_NAME);
    }

    info(__func__,
            ElemUtils::Formatter() << "Radiative correction module: "
                    << m_pRCModule->getClassName());
}

void DVMPGeneratorService::bookHistograms(){

    m_histograms.push_back(
            new TH1D("h_y", "y variable", 100,
                    m_kinematicRanges.getRangeY().getMin(),
                    m_kinematicRanges.getRangeY().getMax()));
    m_histograms.push_back(
            new TH1D("h_Q2", "Q^{2} variable", 100,
                    m_kinematicRanges.getRangeQ2().getMin(),
                    m_kinematicRanges.getRangeQ2().getMax()));
    m_histograms.push_back(
            new TH1D("h_t", "t variable", 100,
                    m_kinematicRanges.getRangeT().getMin(),
                    m_kinematicRanges.getRangeT().getMax()));
    m_histograms.push_back(
            new TH1D("h_phi", "#phi angle", 100,
                    m_kinematicRanges.getRangePhi().getMin(),
                    m_kinematicRanges.getRangePhi().getMax()));
    m_histograms.push_back(
            new TH1D("h_phiS", "#phi_{S} angle", 100,
                    m_kinematicRanges.getRangePhiS().getMin(),
                    m_kinematicRanges.getRangePhiS().getMax()));

    const std::vector<KinematicRange>& rcRanges = m_pRCModule->getVariableRanges();
    std::vector<KinematicRange>::const_iterator it;

    for(it = rcRanges.begin(); it != rcRanges.end(); it++){

        std::stringstream ssA;
        ssA << "h_rc" << int(it - rcRanges.begin());

        std::stringstream ssB;
        ssB << "RC variable " << int(it - rcRanges.begin());

        m_histograms.push_back(
                new TH1D(ssA.str().c_str(), ssB.str().c_str(), 100,
                        it->getMin(), it->getMax()));
    }
}

void DVMPGeneratorService::fillHistograms(const std::vector<double>& variables){

    std::vector<double>::const_iterator it;

    for(it = variables.begin(); it != variables.end(); it++){
        m_histograms.at(int(it - variables.begin()))->Fill(*it);
    }
}

void DVMPGeneratorService::transformVariables(std::vector<double>& variables) const{

    variables.at(0) = exp(variables.at(0));
    variables.at(1) = exp(variables.at(1));
    variables.at(2) = -1 * exp(variables.at(2));
}

void DVMPGeneratorService::transformRanges(std::vector<KinematicRange>& ranges) const{

    ranges.at(0).setMinMax(log(ranges.at(0).getMin()), log(ranges.at(0).getMax())); 
    ranges.at(1).setMinMax(log(ranges.at(1).getMin()), log(ranges.at(1).getMax()));
    ranges.at(2).setMinMax(log(-1 * ranges.at(2).getMax()), log(-1 * ranges.at(2).getMin()));
}

double DVMPGeneratorService::getJacobian(const std::vector<double>& variables) const{
    return -1 * variables.at(0) * variables.at(1) * variables.at(2);
}

} /* namespace EPIC */

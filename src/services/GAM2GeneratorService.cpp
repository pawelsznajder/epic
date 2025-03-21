/*
 * GAM2GeneratorService.cpp
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/services/GAM2GeneratorService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/beans/PolarizationType.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/utils/type/PhysicalType.h>
#include <partons/utils/type/PhysicalUnit.h>
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
#include "../../include/beans/types/ParticleType.h"
#include "../../include/Epic.h"
#include "../../include/managers/ModuleObjectFactory.h"
#include "../../include/modules/event_generator/EventGeneratorModule.h"
#include "../../include/modules/radiative_corrections/RCModule.h"
#include "../../include/modules/writer/WriterModule.h"

namespace EPIC {

const unsigned int GAM2GeneratorService::classId =
        PARTONS::Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new GAM2GeneratorService("GAM2GeneratorService"));

GAM2GeneratorService::GAM2GeneratorService(const std::string &className) :
        GeneratorService<GAM2KinematicRanges, PARTONS::GAM2ProcessModule,
                GAM2KinematicModule, GAM2RCModule>(className) {
}

GAM2GeneratorService::GAM2GeneratorService(const GAM2GeneratorService &other) :
        GeneratorService<GAM2KinematicRanges, PARTONS::GAM2ProcessModule,
                GAM2KinematicModule, GAM2RCModule>(other) {
}

GAM2GeneratorService *GAM2GeneratorService::clone() const {
    return new GAM2GeneratorService(*this);
}

GAM2GeneratorService::~GAM2GeneratorService() {
}

void GAM2GeneratorService::getAdditionalGeneralConfigurationFromTask(
    const MonteCarloTask &task) {
}

double GAM2GeneratorService::getEventDistribution(
        std::vector<double> &kin) const {

    //transform
    transformVariables(kin);

    //observed kinematics
    GAM2Kinematic partonsKinObs(kin.at(4), kin.at(5), kin.at(0), kin.at(1),
            kin.at(2),
            m_experimentalConditions.getLeptonEnergyFixedTargetEquivalent(),
            kin.at(3));

    //check range
    if (!m_kinematicRanges.inRange(m_experimentalConditions, partonsKinObs))
        return 0.;

    //rc
    std::vector<double> rcVariables;
    rcVariables.insert(std::end(rcVariables), std::begin(kin) + 6,
            std::end(kin));

    std::tuple<double, ExperimentalConditions, GAM2Kinematic> rcTrue =
            m_pRCModule->evaluate(m_experimentalConditions, partonsKinObs,
                    rcVariables);

    //check if zero
    if (std::get < 0 > (rcTrue) == 0.)
        return 0.;

    //check if valid
    if (!m_pKinematicModule->checkIfValid(std::get < 1 > (rcTrue),
            std::get < 2 > (rcTrue))) {
        return 0.;
    }

    //PARTONS kinematics
    PARTONS::GAM2ObservableKinematic gam2ObservableKinematic = 
        std::get<2>(rcTrue).toPARTONSGAM2ObservableKinematic();

    //polarisations
    PARTONS::PolarizationType::Type pol0;

    if (std::get < 1 > (rcTrue).getLeptonHelicity() == 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Polarisation of beam electron undefined");
    }

    if (std::get < 1 > (rcTrue).getLeptonHelicity() > 0.) {
        pol0 = PARTONS::PolarizationType::LIN_TRANS_X_PLUS;
    } else {
        pol0 = PARTONS::PolarizationType::LIN_TRANS_Y_MINUS;
    }

    //evaluate
    double result = 0.;

    result +=
            m_pProcessModule->compute(pol0,
                    PARTONS::PolarizationType::LIN_TRANS_X_PLUS,
                    PARTONS::PolarizationType::LIN_TRANS_X_PLUS,
                    std::get < 1 > (rcTrue).getHadronPolarisation(),
                    gam2ObservableKinematic,
                    m_pProcessModule->getListOfAvailableGPDTypeForComputation()).getValue().makeSameUnitAs(
                    PARTONS::PhysicalUnit::NB).getValue();

    result +=
            m_pProcessModule->compute(pol0,
                    PARTONS::PolarizationType::LIN_TRANS_X_PLUS,
                    PARTONS::PolarizationType::LIN_TRANS_Y_MINUS,
                    std::get < 1 > (rcTrue).getHadronPolarisation(),
                    std::get < 2 > (rcTrue).toPARTONSGAM2ObservableKinematic(),
                    m_pProcessModule->getListOfAvailableGPDTypeForComputation()).getValue().makeSameUnitAs(
                    PARTONS::PhysicalUnit::NB).getValue();

    result +=
            m_pProcessModule->compute(pol0,
                    PARTONS::PolarizationType::LIN_TRANS_Y_MINUS,
                    PARTONS::PolarizationType::LIN_TRANS_X_PLUS,
                    std::get < 1 > (rcTrue).getHadronPolarisation(),
                    std::get < 2 > (rcTrue).toPARTONSGAM2ObservableKinematic(),
                    m_pProcessModule->getListOfAvailableGPDTypeForComputation()).getValue().makeSameUnitAs(
                    PARTONS::PhysicalUnit::NB).getValue();

    result +=
            m_pProcessModule->compute(pol0,
                    PARTONS::PolarizationType::LIN_TRANS_Y_MINUS,
                    PARTONS::PolarizationType::LIN_TRANS_Y_MINUS,
                    std::get < 1 > (rcTrue).getHadronPolarisation(),
                    std::get < 2 > (rcTrue).toPARTONSGAM2ObservableKinematic(),
                    m_pProcessModule->getListOfAvailableGPDTypeForComputation()).getValue().makeSameUnitAs(
                    PARTONS::PhysicalUnit::NB).getValue();

    //flux
    result *= getFlux(std::get < 2 > (rcTrue)) * std::get < 0 > (rcTrue);

    //jacobian
    result *= getJacobian(kin);

    if (std::isnan(result)) {

        warn(__func__,
                "Value is NaN, setting zero instead, look for previous messages for a reason");
        result = 0.;
    }

    return result;
}

void GAM2GeneratorService::isServiceWellConfigured() const {
    GeneratorService<GAM2KinematicRanges, PARTONS::GAM2ProcessModule,
            GAM2KinematicModule, GAM2RCModule>::isServiceWellConfigured();
}

void GAM2GeneratorService::run() {

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
        GAM2Kinematic partonsKinObs(eventVec.first.at(4), eventVec.first.at(5),
                eventVec.first.at(0), eventVec.first.at(1),
                eventVec.first.at(2),
                m_experimentalConditions.getLeptonEnergyFixedTargetEquivalent(),
                eventVec.first.at(3));

        //rc
        std::vector<double> rcVariables;
        rcVariables.insert(std::end(rcVariables),
                std::begin(eventVec.first) + 6, std::end(eventVec.first));

        std::tuple<double, ExperimentalConditions, GAM2Kinematic> rcTrue =
                m_pRCModule->evaluate(m_experimentalConditions, partonsKinObs,
                        rcVariables);

        //create event
        Event event = m_pKinematicModule->evaluate(std::get < 1 > (rcTrue),
                std::get < 2 > (rcTrue));

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

double GAM2GeneratorService::getFlux(const GAM2Kinematic& kin) const {

    double Q2 = kin.getQ2();
    double y = kin.getY();

    double Q2Min =
            pow(
                    y
                            * ParticleType(
                                    m_experimentalConditions.getLeptonType()).getMass(),
                    2) / (1. - y);

    return PARTONS::Constant::FINE_STRUCTURE_CONSTANT / (2 * M_PI * Q2)
            * ((1. + pow(1. - y, 2)) / y - 2 * (1. - y) * Q2Min / (y * Q2));
}

void GAM2GeneratorService::getKinematicRangesFromTask(
        const MonteCarloTask &task) {

    m_kinematicRanges = GAM2KinematicRanges::getGAM2KinematicRangesfromTask(task);

    info(__func__,
            ElemUtils::Formatter() << "Kinematic ranges:\n"
                    << m_kinematicRanges.toString() << '\n');
}

void GAM2GeneratorService::getProcessModuleFromTask(
        const MonteCarloTask &task) {

    // check if available
    if (ElemUtils::StringUtils::equals(
            task.getComputationConfiguration().getModuleType(),
            PARTONS::GAM2ProcessModule::GAM2_PROCESS_MODULE_CLASS_NAME)) {

        // configure
        m_pProcessModule =
                PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2ProcessModule(
                        task.getComputationConfiguration().getModuleClassName());

        m_pProcessModule->configure(
                task.getComputationConfiguration().getParameters());

        m_pProcessModule->prepareSubModules(
                task.getComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << PARTONS::GAM2ProcessModule::GAM2_PROCESS_MODULE_CLASS_NAME);
    }

    info(__func__,
            ElemUtils::Formatter() << "Process module: "
                    << m_pProcessModule->getClassName());
}

void GAM2GeneratorService::getKinematicModuleFromTask(
        const MonteCarloTask &task) {

    // check if available
    if (ElemUtils::StringUtils::equals(
            task.getKinematicConfiguration().getModuleType(),
            GAM2KinematicModule::GAM2_KINEMATIC_MODULE_CLASS_NAME)) {

        // configure
        m_pKinematicModule =
                Epic::getInstance()->getModuleObjectFactory()->newGAM2KinematicModule(
                        task.getKinematicConfiguration().getModuleClassName());

        m_pKinematicModule->configure(
                task.getKinematicConfiguration().getParameters());

        m_pKinematicModule->prepareSubModules(
                task.getKinematicConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << GAM2KinematicModule::GAM2_KINEMATIC_MODULE_CLASS_NAME);
    }

    info(__func__,
            ElemUtils::Formatter() << "Kinematic module: "
                    << m_pKinematicModule->getClassName());
}

void GAM2GeneratorService::getRCModuleFromTask(const MonteCarloTask &task) {

    // check if available
    if (ElemUtils::StringUtils::equals(
            task.getRCConfiguration().getModuleType(),
            GAM2RCModule::GAM2_RC_MODULE_CLASS_NAME)) {

        // configure
        m_pRCModule =
                Epic::getInstance()->getModuleObjectFactory()->newGAM2RCModule(
                        task.getRCConfiguration().getModuleClassName());

        m_pRCModule->configure(task.getRCConfiguration().getParameters());

        m_pRCModule->prepareSubModules(
                task.getRCConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << GAM2RCModule::GAM2_RC_MODULE_CLASS_NAME);
    }

    info(__func__,
            ElemUtils::Formatter() << "Radiative correction module: "
                    << m_pRCModule->getClassName());
}

void GAM2GeneratorService::bookHistograms(){

    m_histograms.push_back(
            new TH1D("h_t", "t variable", 100,
                    m_kinematicRanges.getRangeT().getMin(),
                    m_kinematicRanges.getRangeT().getMax()));
    m_histograms.push_back(
            new TH1D("h_uPrim", "u' variable", 100,
                    m_kinematicRanges.getRangeUPrim().getMin(),
                    m_kinematicRanges.getRangeUPrim().getMax()));
    m_histograms.push_back(
            new TH1D("h_Mgg2", "M_{#gamma#gamma}^{2} variable", 100,
                    m_kinematicRanges.getRangeMgg2().getMin(),
                    m_kinematicRanges.getRangeMgg2().getMax()));
    m_histograms.push_back(
            new TH1D("h_phi", "#phi angle", 100,
                    m_kinematicRanges.getRangePhi().getMin(),
                    m_kinematicRanges.getRangePhi().getMax()));
    m_histograms.push_back(
            new TH1D("h_y", "y variable", 100,
                    m_kinematicRanges.getRangeY().getMin(),
                    m_kinematicRanges.getRangeY().getMax()));
    m_histograms.push_back(
            new TH1D("h_Q2", "Q^{2} variable", 100,
                    m_kinematicRanges.getRangeQ2().getMin(),
                    m_kinematicRanges.getRangeQ2().getMax()));

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

void GAM2GeneratorService::fillHistograms(const std::vector<double>& variables){

    std::vector<double>::const_iterator it;

    for(it = variables.begin(); it != variables.end(); it++){
        m_histograms.at(int(it - variables.begin()))->Fill(*it);
    }
}

void GAM2GeneratorService::transformVariables(std::vector<double>& variables) const{

    variables.at(0) = -1 * exp(variables.at(0));
    variables.at(2) = exp(variables.at(2));
    variables.at(4) = exp(variables.at(4));
    variables.at(5) = exp(variables.at(5));
}

void GAM2GeneratorService::transformRanges(std::vector<KinematicRange>& ranges) const{

    ranges.at(0).setMinMax(log(-1 * ranges.at(0).getMax()), log(-1 * ranges.at(0).getMin()));
    ranges.at(2).setMinMax(log(ranges.at(2).getMin()), log(ranges.at(2).getMax()));
    ranges.at(4).setMinMax(log(ranges.at(4).getMin()), log(ranges.at(4).getMax()));
    ranges.at(5).setMinMax(log(ranges.at(5).getMin()), log(ranges.at(5).getMax()));
}

double GAM2GeneratorService::getJacobian(const std::vector<double>& variables) const{
    return -1 * variables.at(0) * variables.at(2) * variables.at(4) * variables.at(5);
}

} /* namespace EPIC */

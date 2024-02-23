/*
 * EventGeneratorROOT.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: Laurent Forthomme (AGH)
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <TFile.h>
#include <TRandom3.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/Partons.h>
#include <partons/ServiceObjectRegistry.h>
#include <partons/services/hash_sum/CryptographicHashService.h>
#include <sys/stat.h>

#include "../../../include/beans/other/EventGeneratorInterface.h"
#include "../../../include/modules/event_generator/EventGeneratorROOT.h"

namespace EPIC {

  const std::string EventGeneratorROOT::PARAMETER_NAME_TYPE = "type";
  const std::string EventGeneratorROOT::PARAMETER_NAME_ABSTOL = "absTol";
  const std::string EventGeneratorROOT::PARAMETER_NAME_RELTOL = "relTol";
  const std::string EventGeneratorROOT::PARAMETER_NAME_NBIN = "nBins";

  const unsigned int EventGeneratorROOT::classId =
      PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new EventGeneratorROOT("EventGeneratorROOT"));

  EventGeneratorROOT::EventGeneratorROOT(const std::string &className) : EventGeneratorModule(className) {}

  EventGeneratorROOT::EventGeneratorROOT(const EventGeneratorROOT &other) : EventGeneratorModule(other) {
    if (other.m_pROOT != nullptr)
      warn(__func__,
           "Not able to copy ROOT object, you need to run "
           "initialization for new object");

    m_type = other.m_type;
    m_absTol = other.m_absTol;
    m_relTol = other.m_relTol;
    m_nBin = other.m_nBin;
  }

  EventGeneratorROOT::~EventGeneratorROOT() {
    if (m_pROOT)
      delete m_pROOT;
  }

  EventGeneratorROOT *EventGeneratorROOT::clone() const { return new EventGeneratorROOT(*this); }

  void EventGeneratorROOT::configure(const ElemUtils::Parameters &parameters) {
    EventGeneratorModule::configure(parameters);

    if (parameters.isAvailable(EventGeneratorROOT::PARAMETER_NAME_TYPE)) {
      m_type = parameters.getLastAvailable().getString();
      info(__func__,
           ElemUtils::Formatter() << "Parameter " << EventGeneratorROOT::PARAMETER_NAME_TYPE << " changed to "
                                  << m_type);
    }

    if (parameters.isAvailable(EventGeneratorROOT::PARAMETER_NAME_ABSTOL)) {
      m_absTol = parameters.getLastAvailable().toDouble();
      info(__func__,
           ElemUtils::Formatter() << "Parameter " << EventGeneratorROOT::PARAMETER_NAME_ABSTOL << " changed to "
                                  << m_absTol);
    }

    if (parameters.isAvailable(EventGeneratorROOT::PARAMETER_NAME_RELTOL)) {
      m_relTol = parameters.getLastAvailable().toDouble();
      info(__func__,
           ElemUtils::Formatter() << "Parameter " << EventGeneratorROOT::PARAMETER_NAME_RELTOL << " changed to "
                                  << m_relTol);
    }

    if (parameters.isAvailable(EventGeneratorROOT::PARAMETER_NAME_NBIN)) {
      m_nBin = parameters.getLastAvailable().toUInt();
      info(__func__,
           ElemUtils::Formatter() << "Parameter " << EventGeneratorROOT::PARAMETER_NAME_NBIN << " changed to "
                                  << m_nBin);
    }
  }

  void EventGeneratorROOT::initialise(const std::vector<KinematicRange> &kinematicRanges,
                                      const EventGeneratorInterface &service) {
    //scenario
    size_t scenario = 0;

    if (!m_initStatePath.empty()) {
      struct stat buffer;
      if (!stat(m_initStatePath.c_str(), &buffer) == 0)  // file does not exist
        scenario = 1;
      else  // file exists
        scenario = 2;
    }

    m_kinematicRanges = kinematicRanges;
    m_pEventGeneratorInterface = &service;

    // initialize
    if (scenario == 0 || scenario == 1) {
      info(__func__, "Creating new ROOT integrator object");
      // parameters
      ROOT::Math::IntegratorMultiDim::Type type;
      if (m_type == "default")
        type = ROOT::Math::IntegratorMultiDim::Type::kDEFAULT;
      else if (m_type == "adaptive")
        type = ROOT::Math::IntegratorMultiDim::Type::kADAPTIVE;
      else if (m_type == "plain")
        type = ROOT::Math::IntegratorMultiDim::Type::kPLAIN;
      else if (m_type == "miser")
        type = ROOT::Math::IntegratorMultiDim::Type::kMISER;
      else if (m_type == "vegas")
        type = ROOT::Math::IntegratorMultiDim::Type::kVEGAS;
      else
        throw ElemUtils::CustomException(
            getClassName(), __func__, ElemUtils::Formatter() << "Invalid type retrieved: " << m_type);
      m_pROOT = new ROOT::Math::IntegratorMultiDim(type, m_absTol, m_relTol, m_nBin);

      if (m_pEventGeneratorInterface == nullptr)
        throw ElemUtils::CustomException(getClassName(), __func__, "Pointer to EventGenerator is null");
      if (m_kinematicRanges.size() != kinematicRanges.size())
        throw ElemUtils::CustomException(
            getClassName(), __func__, "Size of vector containing  kinematic ranges different than nDim");
      static const auto density = [&](const double *x) -> double {
        std::vector<double> v(kinematicRanges.size());
        for (size_t i = 0; i < kinematicRanges.size(); i++)
          v.at(i) = x[i];
        return m_pEventGeneratorInterface->getEventDistribution(v);
      };
      m_pROOT->SetFunction(density, kinematicRanges.size() /* number of dimensions */);
    }
  }

  std::pair<std::vector<double>, double> EventGeneratorROOT::generateEvent() {
    throw ElemUtils::CustomException(getClassName(), __func__, "Module is not yet ready for event generation");
  }

  std::pair<double, double> EventGeneratorROOT::getIntegral() {
    std::vector<double> xmin, xmax;
    for (const auto &range : m_kinematicRanges) {
      xmin.emplace_back(range.getMin());
      xmax.emplace_back(range.getMax());
    }
    if (!m_pROOT)
      throw ElemUtils::CustomException(getClassName(), __func__, "Pointer to ROOT integrator object is null");
    return std::make_pair(m_pROOT->Integral(xmin.data(), xmax.data()), m_pROOT->Error());
  }

} /* namespace EPIC */

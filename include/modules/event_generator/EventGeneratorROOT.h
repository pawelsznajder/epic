/*
 * EventGeneratorROOT.h
 *
 *  Created on: Feb 2, 2024
 *      Author: Laurent Forthomme (AGH)
 */

#ifndef MODULES_EVENT_GENERATOR_EVENTGENERATORROOT_H_
#define MODULES_EVENT_GENERATOR_EVENTGENERATORROOT_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <Math/IntegratorMultiDim.h>
#include <Rtypes.h>
#include <stddef.h>

#include <string>
#include <utility>
#include <vector>

#include "../../beans/containers/KinematicRange.h"
#include "EventGeneratorModule.h"

namespace EPIC {
  /**
   * @class EventGeneratorROOT
   *
   * @brief Event generator based on ROOT IntegratorMultiDim.
   */
  class EventGeneratorROOT : public EventGeneratorModule {
  public:
    static const std::string PARAMETER_NAME_TYPE;    ///< Key to set EventGeneratorROOT::m_type.
    static const std::string PARAMETER_NAME_ABSTOL;  ///< Key to set EventGeneratorROOT::m_absTol.
    static const std::string PARAMETER_NAME_RELTOL;  ///< Key to set EventGeneratorROOT::m_relTol.
    static const std::string PARAMETER_NAME_NBIN;    ///< Key to set EventGeneratorROOT::m_nBin.

    static const unsigned int classId;  ///< Unique ID to automatically register the class in the registry.

    EventGeneratorROOT(const std::string &className);     ///< Default constructor
    EventGeneratorROOT(const EventGeneratorROOT &other);  ///< Copy constructor
    virtual ~EventGeneratorROOT();                        ///< Destructor

    virtual EventGeneratorROOT *clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void initialise(const std::vector<KinematicRange> &kinematicRanges, const EventGeneratorInterface &service);
    virtual std::pair<std::vector<double>, double> generateEvent();
    virtual std::pair<double, double> getIntegral();

  private:
    ROOT::Math::IntegratorMultiDim *m_pROOT{nullptr};  ///< Pointer to ROOT integrator object.

    /// Pointer used during initialization of ROOT.
    const EventGeneratorInterface *m_pEventGeneratorInterface{nullptr};
    std::vector<KinematicRange> m_kinematicRanges;  ///< Kinematic ranges set during initialization.

    std::string m_type{"vegas"};  ///< Type of integration algorithm.
    double m_absTol{-1.};         ///< Maximum absolute uncertainty.
    double m_relTol{-1.};         ///< Maximum relative uncertainty.
    size_t m_nBin{0};             ///< Number of bins in build-up.

    std::pair<double, double> m_integral{0., 0.};  ///< Integrated cross section + uncertainty
  };
}  // namespace EPIC

#endif

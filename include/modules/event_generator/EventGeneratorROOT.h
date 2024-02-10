/*
 * EventGeneratorROOT.h
 *
 *  Created on: Feb 2, 2024
 *      Author: Laurent Forthomme (AGH)
 */

#ifndef MODULES_EVENT_GENERATOR_EVENTGENERATORROOT_H_
#define MODULES_EVENT_GENERATOR_EVENTGENERATORROOT_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <Rtypes.h>
#include <stddef.h>
#include <Math/IntegratorMultiDim.h>
#include <string>
#include <utility>
#include <vector>

#include "../../beans/containers/KinematicRange.h"
#include "EventGeneratorModule.h"

class TFile;

class TRandom;

namespace EPIC {

/**
 * @class EventGeneratorROOT
 *
 * @brief Event generator based on ROOT library.
 *
 * This is event generator module based on ROOT library.
 */
class EventGeneratorROOT: public EventGeneratorModule {
public:
	static const std::string PARAMETER_NAME_TYPE; ///< Key to set EventGeneratorROOT::m_type.
	static const std::string PARAMETER_NAME_ABSTOL; ///< Key to set EventGeneratorROOT::m_absTol.
	static const std::string PARAMETER_NAME_RELTOL; ///< Key to set EventGeneratorROOT::m_relTol.
	static const std::string PARAMETER_NAME_NBIN; ///< Key to set EventGeneratorROOT::m_nBin.

	static const unsigned int classId; ///< Unique ID to automatically register
									   /// the class in the registry.

	/**
	 * Default constructor.
	 */
	EventGeneratorROOT(const std::string &className);

	/**
	 * Copy constructor.
	 */
	EventGeneratorROOT(const EventGeneratorROOT &other);

	/**
	 * Destructor.
	 */
	virtual ~EventGeneratorROOT();

	virtual EventGeneratorROOT *clone() const;
	virtual void configure(const ElemUtils::Parameters &parameters);

	virtual void initialise(const std::vector<KinematicRange> &kinematicRanges,
			const EventGeneratorInterface &service);
	virtual std::pair<std::vector<double>, double> generateEvent();
	virtual std::pair<double, double> getIntegral();

private:

	/**
	 * Scale value to range.
	 */
	double scaleValue(const KinematicRange& range, double value) const;

	/**
	 * Unscale value from range.
	 */
	double unscaleValue(const KinematicRange& range, double value) const;

	/**
	 * Get volume based on kinematic ranges.
	 */
	double getVolume() const;

	ROOT::Math::IntegratorMultiDim *m_pROOT{nullptr};     ///< Pointer to ROOT integrator object.
	TRandom *m_pRandom{nullptr}; ///< Pointer to ROOT TRandom object.
	TFile* m_rootFile{nullptr}; ///< Pointer to ROOT file.

	const EventGeneratorInterface *m_pEventGeneratorInterface{nullptr}; ///< Pointer used
															   /// during
	/// initialization of ROOT.
	std::vector<KinematicRange> m_kinematicRanges; ///< Kinematic ranges set during initialization.

	std::string m_type{"vegas"}; ///< Type of integration algorithm.
	double m_absTol{-1.}; ///< Maximum absolute uncertainty.
	double m_relTol{-1.}; ///< Maximum relative uncertainty.
	size_t m_nBin{0};   ///< Number of bins in build-up.
};

} /* namespace EPIC */

#endif /* MODULES_EVENT_GENERATOR_EVENTGENERATORROOT_H_ */

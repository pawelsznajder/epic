/*
 * EventGeneratorFOAM.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_EVENT_GENERATOR_EVENTGENERATORFOAM_H_
#define MODULES_EVENT_GENERATOR_EVENTGENERATORFOAM_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <Rtypes.h>
#include <stddef.h>
#include <TFoamIntegrand.h>
#include <string>
#include <utility>
#include <vector>

#include "../../beans/containers/KinematicRange.h"
#include "EventGeneratorModule.h"

class TFile;

class TFoam;
class TRandom;

namespace EPIC {

/**
 * @class EventGeneratorFOAM
 *
 * @brief Event generator based on FOAM library.
 *
 * This is event generator module based on FOAM library as implemented in ROOT.
 */
class EventGeneratorFOAM: public EventGeneratorModule, public TFoamIntegrand {
public:
	static const std::string PARAMETER_NAME_NCELLS; ///< Key to set EventGeneratorFOAM::m_nCells.
	static const std::string PARAMETER_NAME_NSAMPL; ///< Key to set EventGeneratorFOAM::m_nSampl.
	static const std::string PARAMETER_NAME_NBIN; ///< Key to set EventGeneratorFOAM::m_nBin.
	static const std::string PARAMETER_NAME_OPTREJ; ///< Key to set EventGeneratorFOAM::m_OptRej.
	static const std::string PARAMETER_NAME_OPTDRIVE; ///< Key to set EventGeneratorFOAM::m_OptDrive.
	static const std::string PARAMETER_NAME_EVPREBIN; ///< Key to set EventGeneratorFOAM::m_EvPerBin.
	static const std::string PARAMETER_NAME_CHAT; ///< Key to set EventGeneratorFOAM::m_Chat.

	static const unsigned int classId; ///< Unique ID to automatically register
									   /// the class in the registry.

	/**
	 * Default constructor.
	 */
	EventGeneratorFOAM(const std::string &className);

	/**
	 * Copy constructor.
	 */
	EventGeneratorFOAM(const EventGeneratorFOAM &other);

	/**
	 * Destructor.
	 */
	virtual ~EventGeneratorFOAM();

	virtual EventGeneratorFOAM *clone() const;
	virtual void configure(const ElemUtils::Parameters &parameters);

	virtual void initialise(const std::vector<KinematicRange> &kinematicRanges,
			const EventGeneratorInterface &service);
	virtual std::pair<std::vector<double>, double> generateEvent();
	virtual std::pair<double, double> getIntegral();

	virtual Double_t Density(Int_t ndim, Double_t *);

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

	TFoam *m_pFoam;     ///< Pointer to ROOT FOAM object.
	TRandom *m_pRandom; ///< Pointer to ROOT TRandom object.
	TFile* m_rootFile; ///< Pointer to ROOT file.

	const EventGeneratorInterface *m_pEventGeneratorInterface; ///< Pointer used
															   /// during
	/// initialization
	/// of FOAM.
	std::vector<KinematicRange> m_kinematicRanges; ///< Kinematic ranges set during initialization.

	size_t m_nCells; ///< Number of cells.
	size_t m_nSampl; ///< Number of MC events per cell in build-up.
	size_t m_nBin;   ///< Number of bins in build-up.
	size_t m_OptRej; ///< Weighted events for OptRej=0; true MC events for OptRej=1.
	size_t m_OptDrive; ///< Type of Drive = 0,1,2 for TrueVol, Sigma, WtMax.
	size_t m_EvPerBin; ///< Maximum events (equiv.) per bin in buid-up.
	size_t m_Chat;     ///< Verbosity level.
};

} /* namespace EPIC */

#endif /* MODULES_EVENT_GENERATOR_EVENTGENERATORFOAM_H_ */

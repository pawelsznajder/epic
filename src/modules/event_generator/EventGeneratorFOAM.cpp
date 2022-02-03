/*
 * EventGeneratorFOAM.cpp
 *
 *  Created on: Feb 15, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../include/modules/event_generator/EventGeneratorFOAM.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/Partons.h>
#include <partons/services/hash_sum/CryptographicHashService.h>
#include <partons/ServiceObjectRegistry.h>
#include <sys/stat.h>
#include <TFile.h>
#include <TFoam.h>
#include <TRandom3.h>

#include "../../../include/beans/other/EventGeneratorInterface.h"

namespace EPIC {

const std::string EventGeneratorFOAM::PARAMETER_NAME_NCELLS = "nCells";
const std::string EventGeneratorFOAM::PARAMETER_NAME_NSAMPL = "nSamples";
const std::string EventGeneratorFOAM::PARAMETER_NAME_NBIN = "nBins";
const std::string EventGeneratorFOAM::PARAMETER_NAME_OPTREJ = "optRej";
const std::string EventGeneratorFOAM::PARAMETER_NAME_OPTDRIVE = "optDrive";
const std::string EventGeneratorFOAM::PARAMETER_NAME_EVPREBIN = "nEvPerBin";
const std::string EventGeneratorFOAM::PARAMETER_NAME_CHAT = "chatLevel";

const unsigned int EventGeneratorFOAM::classId =
		PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
				new EventGeneratorFOAM("EventGeneratorFOAM"));

EventGeneratorFOAM::EventGeneratorFOAM(const std::string &className) :
		EventGeneratorModule(className), m_pFoam(nullptr), m_pRandom(nullptr), m_rootFile(
				nullptr), m_pEventGeneratorInterface(nullptr) {

	m_nCells = 500;
	m_nSampl = 200;
	m_nBin = 8;
	m_OptRej = 1;
	m_OptDrive = 2;
	m_EvPerBin = 25;
	m_Chat = 0;
}

EventGeneratorFOAM::EventGeneratorFOAM(const EventGeneratorFOAM &other) :
		EventGeneratorModule(other), m_pFoam(nullptr), m_pRandom(nullptr), m_rootFile(
				nullptr), m_pEventGeneratorInterface(nullptr) {

	if (other.m_pFoam != nullptr) {
		warn(__func__, "Not able to copy FOAM object, you need to run "
				"initialization for new object");
	}

	m_nCells = other.m_nCells;
	m_nSampl = other.m_nSampl;
	m_nBin = other.m_nBin;
	m_OptRej = other.m_OptRej;
	m_OptDrive = other.m_OptDrive;
	m_EvPerBin = other.m_EvPerBin;
	m_Chat = other.m_Chat;
}

EventGeneratorFOAM::~EventGeneratorFOAM() {

	if (m_pFoam != nullptr) {
		delete m_pFoam;
		m_pFoam = nullptr;
	}

	if (m_pRandom != nullptr) {
		delete m_pRandom;
		m_pRandom = nullptr;
	}

	if (m_rootFile != nullptr) {
		m_rootFile->Close();
		m_rootFile = nullptr;
	}
}

EventGeneratorFOAM *EventGeneratorFOAM::clone() const {
	return new EventGeneratorFOAM(*this);
}

void EventGeneratorFOAM::configure(const ElemUtils::Parameters &parameters) {

	EventGeneratorModule::configure(parameters);

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_NCELLS)) {

		m_nCells = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_NCELLS
						<< " changed to " << m_nCells);
	}

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_NSAMPL)) {

		m_nSampl = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_NSAMPL
						<< " changed to " << m_nSampl);
	}

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_NBIN)) {

		m_nBin = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_NBIN
						<< " changed to " << m_nBin);
	}

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_OPTREJ)) {

		m_OptRej = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_OPTREJ
						<< " changed to " << m_OptRej);
	}

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_OPTDRIVE)) {

		m_OptDrive = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_OPTDRIVE
						<< " changed to " << m_OptDrive);
	}

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_EVPREBIN)) {

		m_EvPerBin = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_EVPREBIN
						<< " changed to " << m_EvPerBin);
	}

	if (parameters.isAvailable(EventGeneratorFOAM::PARAMETER_NAME_CHAT)) {

		m_Chat = parameters.getLastAvailable().toUInt();

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< EventGeneratorFOAM::PARAMETER_NAME_CHAT
						<< " changed to " << m_Chat);
	}
}

void EventGeneratorFOAM::initialise(
		const std::vector<KinematicRange> &kinematicRanges,
		const EventGeneratorInterface &service) {

	//scenario
	size_t scenario = 0;

	if (!m_initStatePath.empty()) {

		//check if file exist
		struct stat buffer;

		//no
		if (!stat(m_initStatePath.c_str(), &buffer) == 0) {
			scenario = 1;
		}
		//yes
		else {
			scenario = 2;
		}
	}

	// random number generator (create)
	m_pRandom = new TRandom3(m_seed);

	// create
	if (scenario == 0 || scenario == 1) {

		info(__func__, "Creating new TFoam object");

		std::string rootObjectName =
				PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
						ElemUtils::Formatter() << m_pRandom->Uniform());

		info(__func__,
				ElemUtils::Formatter() << "TFoam object name: "
						<< rootObjectName);

		m_pFoam = new TFoam(rootObjectName.c_str());

	} else {

		info(__func__,
				ElemUtils::Formatter() << "Reading TFoam state from: "
						<< m_initStatePath);

		m_rootFile = new TFile(m_initStatePath.c_str(), "READ");

		if (m_rootFile->IsZombie()) {
			throw ElemUtils::CustomException(getClassName(), __func__,
					ElemUtils::Formatter() << "Unable to open "
							<< m_initStatePath << " for reading");
		}

		m_pFoam = (TFoam*) m_rootFile->Get("EPIC_TFoam");
	}

	// kinematic ranges
	m_kinematicRanges = kinematicRanges;

	// pointer
	m_pEventGeneratorInterface = &service;

	// distribution
	m_pFoam->SetRho(this);

	// initialize
	if (scenario == 0 || scenario == 1) {

		// random number generator (set)
		m_pFoam->SetPseRan(m_pRandom);

		// number of dimensions
		m_pFoam->SetkDim(kinematicRanges.size());

		// parameters
		m_pFoam->SetnCells(m_nCells);
		m_pFoam->SetnSampl(m_nSampl);
		m_pFoam->SetnBin(m_nBin);
		m_pFoam->SetOptRej(m_OptRej);
		m_pFoam->SetOptDrive(m_OptDrive);
		m_pFoam->SetEvPerBin(m_EvPerBin);
		m_pFoam->SetChat(m_Chat);

		m_pFoam->Initialize();
	}

	//save
	if (scenario == 1) {

		info(__func__,
				ElemUtils::Formatter() << "Saving TFoam state in: "
						<< m_initStatePath);

		TFile rootFile(m_initStatePath.c_str(), "RECREATE");

		if (rootFile.IsZombie()) {
			throw ElemUtils::CustomException(getClassName(), __func__,
					ElemUtils::Formatter() << "Unable to open "
							<< m_initStatePath << " for writing");
		}

		m_pFoam->Write("EPIC_TFoam");
		rootFile.Write();
		rootFile.Close();
	}
}

Double_t EventGeneratorFOAM::Density(Int_t ndim, Double_t *kin) {

	if (m_pEventGeneratorInterface == nullptr) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"Pointer to EventGenerator is null");
	}

	if (m_kinematicRanges.size() != ndim) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"Size of vector containing  kinematic ranges different than nDim");
	}

	std::vector<double> v(ndim);

	for (size_t i = 0; i < ndim; i++) {
		v.at(i) = unscaleValue(m_kinematicRanges.at(i), kin[i]);
	}

	return m_pEventGeneratorInterface->getEventDistribution(v);
}

std::pair<std::vector<double>, double> EventGeneratorFOAM::generateEvent() {

	if (m_pFoam == nullptr) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"Pointer to TFoam is null");
	}

	size_t ndim = m_pFoam->GetTotDim();

	Double_t *result = new Double_t[ndim];
	std::vector<double> v(ndim);

	double weight;

	m_pFoam->MakeEvent();
	m_pFoam->GetMCvect(result);
	weight = m_pFoam->GetMCwt();

	for (size_t i = 0; i < ndim; i++) {
		v.at(i) = unscaleValue(m_kinematicRanges.at(i), result[i]);
	}

	delete[] result;

	return std::make_pair(v, weight);
}

std::pair<double, double> EventGeneratorFOAM::getIntegral() {

	if (m_pFoam == nullptr) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"Pointer to TFoam is null");
	}

	double volume = getVolume();

	double integral, error;

	m_pFoam->GetIntegMC(integral, error);

	return std::make_pair(volume * integral, volume * error);
}

double EventGeneratorFOAM::scaleValue(const KinematicRange& range,
		double value) const {

	double minScaled = range.getMin();
	double maxScaled = range.getMax();

	if (value < minScaled || value > maxScaled) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter() << "Value outside the limits: "
						<< value);
	}

	return (value - minScaled) / (maxScaled - minScaled);
}

double EventGeneratorFOAM::unscaleValue(const KinematicRange& range,
		double value) const {

	double minScaled = range.getMin();
	double maxScaled = range.getMax();

	if (value < 0. || value > 1.) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter() << "Value outside the limits: "
						<< value);
	}

	return minScaled + value * (maxScaled - minScaled);
}

double EventGeneratorFOAM::getVolume() const {

	double result = 1.;

	std::vector<KinematicRange>::const_iterator it;

	for (it = m_kinematicRanges.begin(); it != m_kinematicRanges.end(); it++) {
		result *= it->getMax() - it->getMin();
	}

	return result;
}

} /* namespace EPIC */

/*
 * DVCSRCCollinear.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: partons
 */

#include "../../../../include/modules/radiative_corrections/DVCS/DVCSRCCollinear.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <partons/utils/type/PhysicalType.h>
#include <cmath>
#include <iterator>
#include <memory>
#include <utility>

#include "../../../../include/beans/containers/KinematicRange.h"
#include "../../../../include/beans/physics/Event.h"
#include "../../../../include/beans/physics/Particle.h"
#include "../../../../include/beans/physics/Vertex.h"
#include "../../../../include/beans/types/ParticleCodeType.h"
#include "../../../../include/beans/types/ParticleType.h"
#include "../../../../include/modules/radiative_corrections/RCModule.h"

namespace EPIC {

const std::string DVCSRCCollinear::PARAMETER_NAME_EPSILON = "epsilon";

const unsigned int DVCSRCCollinear::classId =
		PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
				new DVCSRCCollinear("DVCSRCCollinear"));

DVCSRCCollinear::DVCSRCCollinear(const std::string &className) :
		DVCSRCModule(className), m_epsilon(1.E-4) {

	std::vector<KinematicRange> variableRanges(2);

	variableRanges.at(0) = KinematicRange(-8., 0.);	//z of ISR
	variableRanges.at(1) = KinematicRange(-8., 0.);	//z of FSR

	setVariableRanges(variableRanges);
}

DVCSRCCollinear::DVCSRCCollinear(const DVCSRCCollinear &other) :
		DVCSRCModule(other), m_epsilon(other.m_epsilon) {
}

DVCSRCCollinear::~DVCSRCCollinear() {
}

DVCSRCCollinear *DVCSRCCollinear::clone() const {
	return new DVCSRCCollinear(*this);
}

void DVCSRCCollinear::configure(const ElemUtils::Parameters &parameters) {

	DVCSRCModule::configure(parameters);

	if (parameters.isAvailable(DVCSRCCollinear::PARAMETER_NAME_EPSILON)) {

		double newEpsilon = parameters.getLastAvailable().toDouble();

		if (!getVariableRanges().at(0).inRange(log10(newEpsilon))) {
			throw ElemUtils::CustomException(getClassName(), __func__,
					ElemUtils::Formatter()
							<< "Log10 of epsilon must be in range: "
							<< getVariableRanges().at(0).toString());
		}

		m_epsilon = newEpsilon;

		info(__func__,
				ElemUtils::Formatter() << "Parameter "
						<< DVCSRCCollinear::PARAMETER_NAME_EPSILON
						<< " changed to " << m_epsilon);
	}
}

std::tuple<double, ExperimentalConditions, DVCSKinematic> DVCSRCCollinear::evaluate(
		const ExperimentalConditions& experimentalConditions,
		const DVCSKinematic& obsKin, const std::vector<double>& par) const {

	//check parameters
	checkParameters(par);

	//rc variables
	double z1 = 1. - pow(10, par.at(0));
	double z3 = 1. - pow(10, par.at(1));

	//variables
	double leptonMass =
			ParticleType(experimentalConditions.getLeptonType()).getMass();
	double hadronMass =
			ParticleType(experimentalConditions.getHadronType()).getMass();
	double x = obsKin.getXB();
	double Q2 = obsKin.getQ2();
	double y =
			Q2
					/ (2 * hadronMass
							* experimentalConditions.getLeptonEnergyFixedTargetEquivalent()
							* x);

	//check ranges
	if ((z1 < (1. - y) / (1. - x * y)) || (z3 < 1. - y * (1. - x))) {
		return std::make_tuple(0., experimentalConditions, obsKin);
	}

	//L
	double L = PARTONS::Constant::FINE_STRUCTURE_CONSTANT / (2 * M_PI)
			* log(Q2 / pow(leptonMass, 2));

	//ISR =======================================================================
	double radiativeFunctionISR = 0.;

	//no radiation
	if (z1 > 1. - m_epsilon) {

		radiativeFunctionISR = 1. + L * (2 * log(m_epsilon) + 1.5);
		radiativeFunctionISR /= log10(m_epsilon)
				- getVariableRanges().at(0).getMin();

		z1 = 1.;
	}
	//radiation
	else {
		radiativeFunctionISR = L * (1. + pow(z1, 2)) / (1. - z1);
		radiativeFunctionISR *= (1. - z1) * log(10.); //from: (dz/dz' where z = 1-10^'z)
	}

	//FSR =======================================================================
	double radiativeFunctionFSR = 0.;

	//no radiation
	if (z3 > 1. - m_epsilon) {

		radiativeFunctionFSR = 1. + L * (2 * log(m_epsilon) + 1.5);
		radiativeFunctionFSR /= log10(m_epsilon)
				- getVariableRanges().at(1).getMin();

		z3 = 1.;
	}
	//radiation
	else {
		radiativeFunctionFSR = L * (1. + pow(z3, 2)) / (1. - z3);
		radiativeFunctionFSR *= (1. - z3) * log(10.); //from: (dz/dz' where z = 1-10^'z)
	}

	//===========================================================================

	//true variables
	double xBHat = z1 * x * y / (z1 * z3 + y - 1.);
	double Q2Hat = z1 / z3 * Q2;
	double yHat = (z1 * z3 + y - 1.) / (z1 * z3);

	//total radiator
	double radiativeFunction = radiativeFunctionISR * radiativeFunctionFSR;
	radiativeFunction *= y / yHat * z1 / pow(z3, 2); //from: (dxHat dQ2Hat)/(dx dQ2)

	//set
	ExperimentalConditions rc_experimentalConditions = experimentalConditions;
	DVCSKinematic rc_obsKin = obsKin;

	rc_experimentalConditions.setLeptonEnergy(
			z1 * rc_experimentalConditions.getLeptonEnergy());

	rc_obsKin.setXB(xBHat);
	rc_obsKin.setQ2(Q2Hat);
	rc_obsKin.setE(
			rc_experimentalConditions.getLeptonEnergyFixedTargetEquivalent());

	// return
	return std::make_tuple(radiativeFunction, rc_experimentalConditions,
			rc_obsKin);
}

void DVCSRCCollinear::updateEvent(Event& event,
		const std::vector<double>& par) const {

	//check parameters
	checkParameters(par);

	//rc variables
	double z1 = 1. - pow(10, par.at(0));
	double z3 = 1. - pow(10, par.at(1));

	//check if do anything
	if ((z1 > 1. - m_epsilon) && (z3 > 1. - m_epsilon)) {
		return;
	}

	//find particles
	int idE = -1;
	int idEs = -1;

	//TODO all leptons
	for (std::vector<
			std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >::const_iterator it =
			event.getParticles().begin(); it != event.getParticles().end();
			it++) {

		if (it->first == ParticleCodeType::BEAM
				&&  (it->second->getType() == ParticleType::ELECTRON
                        || it->second->getType() == ParticleType::POSITRON
                        || it->second->getType() == ParticleType::MUON_MINUS
                        || it->second->getType() == ParticleType::MUON_PLUS
                        || it->second->getType() == ParticleType::TAU_MINUS
                        || it->second->getType() == ParticleType::TAU_PLUS)) {

			idE = int(it - event.getParticles().begin());
			continue;
		}

		if (it->first == ParticleCodeType::SCATTERED
				&&  (it->second->getType() == ParticleType::ELECTRON
                        || it->second->getType() == ParticleType::POSITRON
                        || it->second->getType() == ParticleType::MUON_MINUS
                        || it->second->getType() == ParticleType::MUON_PLUS
                        || it->second->getType() == ParticleType::TAU_MINUS
                        || it->second->getType() == ParticleType::TAU_PLUS)) {
			idEs = int(it - event.getParticles().begin());
			continue;
		}

		if (idE != -1 && idEs != -1)
			break;
	}

	if (idE == -1 || idEs == -1) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"Not able to find particles");
	}

	std::shared_ptr<Particle> ptrE = event.getParticles().at(idE).second;
	std::shared_ptr<Particle> ptrEs = event.getParticles().at(idEs).second;

	//ISR
	if (z1 <= 1. - m_epsilon) {

		//change status
		event.alterParticleCode(idE, ParticleCodeType::DOCUMENTATION);

		//create new particles
		std::shared_ptr<Particle> par1 = std::make_shared<Particle>(
				ptrE->getType(), ptrE->getMomentum().Unit(),
				ptrE->getEnergy() / z1);
		std::shared_ptr<Particle> par2 = std::make_shared<Particle>(
				ParticleType::PHOTON, ptrE->getMomentum().Unit(),
				ptrE->getEnergy() / z1 * (1. - z1));

		//vertex
		std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>();
		vertex->addParticleIn(par1);
		vertex->addParticleOut(par2);
		vertex->addParticleOut(ptrE);

		//add
		event.addParticle(std::make_pair(ParticleCodeType::BEAM, par1));
		event.addParticle(std::make_pair(ParticleCodeType::UNDECAYED, par2));

		event.addVertex(vertex);
	}

	//FSR
	if (z3 <= 1. - m_epsilon) {

        //change status
        event.alterParticleCode(idEs, ParticleCodeType::DOCUMENTATION);

		//create new particles
		std::shared_ptr<Particle> par1 = std::make_shared<Particle>(
				ptrEs->getType(), ptrEs->getMomentum().Unit(),
				ptrEs->getEnergy() * z3);
		std::shared_ptr<Particle> par2 = std::make_shared<Particle>(
				ParticleType::PHOTON, ptrEs->getMomentum().Unit(),
				ptrEs->getEnergy() * (1. - z3));

		//vertex
		std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>();
		vertex->addParticleIn(ptrEs);
		vertex->addParticleOut(par1);
		vertex->addParticleOut(par2);

		//add
		event.addParticle(std::make_pair(ParticleCodeType::UNDECAYED, par1));
		event.addParticle(std::make_pair(ParticleCodeType::UNDECAYED, par2));

		event.addVertex(vertex);
	}
}

} /* namespace EPIC */

/*
 * DVCSRCCollinearFull2ndOrder.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: partons
 */

#include "../../../../include/modules/radiative_corrections/DVCS/DVCSRCCollinearFull2ndOrder.h"

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

const unsigned int DVCSRCCollinearFull2ndOrder::classId =
		PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
				new DVCSRCCollinearFull2ndOrder("DVCSRCCollinearFull2ndOrder"));

DVCSRCCollinearFull2ndOrder::DVCSRCCollinearFull2ndOrder(const std::string &className) :
        DVCSRCCollinear(className) {
}

DVCSRCCollinearFull2ndOrder::DVCSRCCollinearFull2ndOrder(const DVCSRCCollinearFull2ndOrder &other) :
        DVCSRCCollinear(other){
}

DVCSRCCollinearFull2ndOrder::~DVCSRCCollinearFull2ndOrder() {
}

DVCSRCCollinearFull2ndOrder *DVCSRCCollinearFull2ndOrder::clone() const {
	return new DVCSRCCollinearFull2ndOrder(*this);
}

void DVCSRCCollinearFull2ndOrder::configure(const ElemUtils::Parameters &parameters) {
    DVCSRCCollinear::configure(parameters);
}

std::tuple<double, ExperimentalConditions, DVCSKinematic> DVCSRCCollinearFull2ndOrder::evaluate(
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
    double y = obsKin.getY();
    double Q2 = obsKin.getQ2();
    double x =
            Q2
                    / (2 * hadronMass
                            * experimentalConditions.getLeptonEnergyFixedTargetEquivalent()
                            * y);

    //check ranges
    double z1min = (1. - y) / (1. - x * y);
    double z3min =  1. - y + x * y;

    if (z1 < z1min || z3 < (x * y + (1. - y) / z1)) {
        return std::make_tuple(0., experimentalConditions, obsKin);
    }

    //L
    double L = PARTONS::Constant::FINE_STRUCTURE_CONSTANT / (2 * M_PI)
            * log(Q2 / pow(leptonMass, 2));

    //ISR =======================================================================
    double radiativeFunctionISR = 0.;
    double radiativeFunctionTwoISR = 0.;

    //no radiation
    if (z1 > 1. - m_epsilon) {

        radiativeFunctionISR = 1. + L * (2 * log(m_epsilon) + 1.5);
        radiativeFunctionISR /= log10(m_epsilon)
                - getVariableRanges().at(0).getMin();

        radiativeFunctionTwoISR = pow(L, 2) / 2 * (4 * pow(log(m_epsilon), 2) + 6 * log(m_epsilon) + 13./4. + 2 * pow(z1min, 2) * log(z1min) - pow(z1min, 2));
        radiativeFunctionTwoISR /= log10(m_epsilon)
                - getVariableRanges().at(0).getMin();

        z1 = 1.;
    }
    //radiation
    else {
        radiativeFunctionISR = L * (1. + pow(z1, 2)) / (1. - z1);
        radiativeFunctionISR *= (1. - z1) * log(10.); //from: (dz/dz' where z = 1-10^'z)

        radiativeFunctionTwoISR = pow(L, 2) / 2 * (2 * (1. + pow(z1, 2)) / (1. - z1) * (2 * log(1. - z1) - log(z1) + 1.5) + (1. + z1) * log(z1) - 2 * (1. - z1)) ;
        radiativeFunctionTwoISR *= (1. - z1) * log(10.); //from: (dz/dz' where z = 1-10^'z)
    }

    //FSR =======================================================================
    double radiativeFunctionFSR = 0.;
    double radiativeFunctionTwoFSR = 0.;

    //no radiation
    if (z3 > 1. - m_epsilon) {

        radiativeFunctionFSR = 1. + L * (2 * log(m_epsilon) + 1.5);
        radiativeFunctionFSR /= log10(m_epsilon)
                - getVariableRanges().at(1).getMin();

        radiativeFunctionTwoFSR = pow(L, 2) / 2 * (4 * pow(log(m_epsilon), 2) + 6 * log(m_epsilon) + 13./4. + 2 * pow(z3min, 2) * log(z3min) - pow(z3min, 2));
        radiativeFunctionTwoFSR /= log10(m_epsilon)
                - getVariableRanges().at(1).getMin();

        z3 = 1.;
    }
    //radiation
    else {
        radiativeFunctionFSR = L * (1. + pow(z3, 2)) / (1. - z3);
        radiativeFunctionFSR *= (1. - z3) * log(10.); //from: (dz/dz' where z = 1-10^'z)

        radiativeFunctionTwoFSR = pow(L, 2) / 2 * (2 * (1. + pow(z3, 2)) / (1. - z3) * (2 * log(1. - z3) - log(z3) + 1.5) + (1. + z3) * log(z3) - 2 * (1. - z3)) ;
        radiativeFunctionTwoFSR *= (1. - z3) * log(10.); //from: (dz/dz' where z = 1-10^'z)
    }

    //===========================================================================

    //true variables
    double xBHat = z1 * x * y / (z1 * z3 + y - 1.);
    double Q2Hat = z1 / z3 * Q2;
    double yHat = (z1 * z3 + y - 1.) / (z1 * z3);

    //total radiator
    double radiativeFunction = radiativeFunctionISR * radiativeFunctionFSR + radiativeFunctionTwoISR + radiativeFunctionTwoFSR;
    radiativeFunction *= y / yHat * z1 / pow(z3, 2); //from: (dxHat dQ2Hat)/(dx dQ2)

    //set
    ExperimentalConditions rc_experimentalConditions = experimentalConditions;
    DVCSKinematic rc_obsKin = obsKin;

    rc_experimentalConditions.setLeptonEnergy(
            z1 * rc_experimentalConditions.getLeptonEnergy());

    rc_obsKin.setY(yHat);
    rc_obsKin.setQ2(Q2Hat);
    rc_obsKin.setE(
            rc_experimentalConditions.getLeptonEnergyFixedTargetEquivalent());

    // return
    return std::make_tuple(radiativeFunction, rc_experimentalConditions,
            rc_obsKin);
}

void DVCSRCCollinearFull2ndOrder::updateEvent(Event& event,
		const std::vector<double>& par) const {
    DVCSRCCollinear::updateEvent(event, par);
}

} /* namespace EPIC */

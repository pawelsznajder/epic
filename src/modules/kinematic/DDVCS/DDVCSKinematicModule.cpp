/*
 * DDVCSKinematicModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/kinematic/DDVCS/DDVCSKinematicModule.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>
#include <cmath>

#include "../../../../include/beans/containers/ExperimentalConditions.h"

namespace EPIC {

const std::string DDVCSKinematicModule::DDVCS_KINEMATIC_MODULE_CLASS_NAME =
        "DDVCSKinematicModule";

const std::string DDVCSKinematicModule::DDVCS_KINEMATIC_MODULE_PARAMETER_DECAY_TYPE =
        "decay_type";

DDVCSKinematicModule::DDVCSKinematicModule(const std::string &className) :
        KinematicModule<DDVCSKinematicRanges, DDVCSKinematic>(className, PARTONS::ChannelType::DDVCS), m_decayType(
                ParticleType::UNDEFINED) {
}

DDVCSKinematicModule::DDVCSKinematicModule(const DDVCSKinematicModule &other) :
        KinematicModule<DDVCSKinematicRanges, DDVCSKinematic>(other), m_decayType(other.m_decayType) {
}

DDVCSKinematicModule::~DDVCSKinematicModule() {
}

void DDVCSKinematicModule::configure(const ElemUtils::Parameters &parameters) {

    // run for mother
    KinematicModule<DDVCSKinematicRanges, DDVCSKinematic>::configure(parameters);

    // look for name
    if (parameters.isAvailable(
            DDVCSKinematicModule::DDVCS_KINEMATIC_MODULE_PARAMETER_DECAY_TYPE)) {

        // get
        m_decayType = ParticleType::fromString(
                parameters.getLastAvailable().getString());

        // print status
        info(__func__,
                ElemUtils::Formatter() << "Decay type set to: "
                        << ParticleType(m_decayType).toString());
    }
}

bool DDVCSKinematicModule::runTest() const {

    // TODO
    return true;
}

std::vector<KinematicRange> DDVCSKinematicModule::getKinematicRanges(const ExperimentalConditions &conditions, const DDVCSKinematicRanges& ranges){

    std::vector<KinematicRange> result(8);

    result.at(0) = ranges.getRangeY();
    result.at(1) = ranges.getRangeQ2();
    result.at(2) = ranges.getRangeT();
    result.at(3) = ranges.getRangeQ2Prim();
    result.at(4) = ranges.getRangePhi();
    result.at(5) = ranges.getRangePhiS();
    result.at(6) = ranges.getRangePhiL();
    result.at(7) = ranges.getRangeThetaL();

    //estimate key limits of kinematic variables
    double mP = ParticleType(conditions.getHadronType()).getMass(); 
    double mE = ParticleType(conditions.getLeptonType()).getMass(); 
    double eInE = conditions.getLeptonEnergyFixedTargetEquivalent();

    double minQ2Prim = pow(2 * ParticleType(m_decayType).getMass(), 2);
        
    if(ranges.getRangeQ2Prim().getMin() <= 0.){
        changeKinematicRange(result.at(3), true, minQ2Prim, "Q2Prim");
    }

    minQ2Prim = result.at(3).getMin();

    double minT = (pow(minQ2Prim,2)*pow(2*eInE*mP + pow(mP,2),-1) - pow(-2*pow(mP*pow(eInE,2)*pow(2*eInE + mP,-1),0.5) + pow((-4*eInE*mP*minQ2Prim - 4*minQ2Prim*pow(mP,2) + 4*pow(eInE,2)*pow(mP,2) + pow(minQ2Prim,2))*pow(2*eInE*mP + pow(mP,2),-1),0.5),2))/4.; 

    if(ranges.getRangeT().getMax() >= 0.){
        changeKinematicRange(result.at(2), false, minT, "t");
    }

    double minY = (2*mP*sqrt(minQ2Prim) + minQ2Prim)/(2.*eInE*mP);

    if(ranges.getRangeY().getMin() <= 0.){
        changeKinematicRange(result.at(0), true, minY, "y");
    }

    minY = result.at(0).getMin();

    double minQ2 = pow(minY*mE, 2)/(1. - minY);

    if(ranges.getRangeQ2().getMin() <= 0.){
        changeKinematicRange(result.at(1), true, minQ2, "Q2");
    }

    return result;
}

bool DDVCSKinematicModule::checkIfValid(
        const ExperimentalConditions &conditions, const DDVCSKinematic &kin) {

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();

    double y = kin.getY();
    double t = kin.getT();
    double Q2 = kin.getQ2();
    double Q2Prim = kin.getQ2Prim();
    double phi = kin.getPhi();
    double phiS = kin.getPhiS();
    double phiL = kin.getPhiL();
    double thetaL = kin.getThetaL();

    if (std::isnan(Ee) || std::isnan(Ep) || std::isnan(y) || std::isnan(t)
            || std::isnan(Q2) || std::isnan(Q2Prim) || std::isnan(phi)
            || std::isnan(phiS) || std::isnan(phiL) || std::isnan(thetaL)) {

        warn(__func__,
                ElemUtils::Formatter() << "One or more of values in NaN: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
        return false;
    }

    if (Ee <= 0.) {
        return false;
    }

    if (Ep <= 0.) {
        return false;
    }

    if (y <= 0. || y >= 1.) {
        return false;
    }

    if (t >= 0.) {
        return false;
    }

    if (Q2 <= 0.) {
        return false;
    }

    if (Q2Prim <= 0.) {
        return false;
    }

    return true;
}

void DDVCSKinematicModule::initModule() {
}

void DDVCSKinematicModule::isModuleWellConfigured() {
}

ParticleType::Type DDVCSKinematicModule::getDecayType() const {
    return m_decayType;
}

void DDVCSKinematicModule::setDecayType(ParticleType::Type decayType) {
    m_decayType = decayType;
}

} /* namespace EPIC */

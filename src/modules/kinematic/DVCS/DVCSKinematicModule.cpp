/*
 * DVCSKinematicModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/kinematic/DVCS/DVCSKinematicModule.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>
#include <cmath>

#include "../../../../include/beans/containers/ExperimentalConditions.h"

namespace EPIC {

const std::string DVCSKinematicModule::DVCS_KINEMATIC_MODULE_CLASS_NAME =
        "DVCSKinematicModule";

DVCSKinematicModule::DVCSKinematicModule(const std::string &className) :
        KinematicModule<DVCSKinematicRanges, DVCSKinematic>(className, PARTONS::ChannelType::DVCS) {
}

DVCSKinematicModule::DVCSKinematicModule(const DVCSKinematicModule &other) :
        KinematicModule<DVCSKinematicRanges, DVCSKinematic>(other) {
}

DVCSKinematicModule::~DVCSKinematicModule() {
}

bool DVCSKinematicModule::runTest() const {

    // TODO
    return true;
}

std::vector<KinematicRange> DVCSKinematicModule::getKinematicRanges(const ExperimentalConditions &conditions, const DVCSKinematicRanges& ranges){

    std::vector<KinematicRange> result(5);

    result.at(0) = ranges.getRangeY();
    result.at(1) = ranges.getRangeQ2();
    result.at(2) = ranges.getRangeT();
    result.at(3) = ranges.getRangePhi();
    result.at(4) = ranges.getRangePhiS();

    //estimate key limits of kinematic variables
    double mP = ParticleType(conditions.getHadronType()).getMass(); 
    double mE = ParticleType(conditions.getLeptonType()).getMass(); 
    double eInE = conditions.getLeptonEnergyFixedTargetEquivalent();

    double minQ2 = pow(2 * mE, 2); 
        
    if(ranges.getRangeQ2().getMin() <= 0.){
        changeKinematicRange(result.at(1), true, minQ2, "Q2");
    }

    minQ2 = result.at(1).getMin();

    double minT = 0.125*pow(eInE,4)*pow(minQ2,2)*pow(1.*mP*pow(eInE,5) + pow(eInE,4)*(-0.5*minQ2 + 0.5*pow(mP,2)),-1) - pow(-2*pow((0.5*minQ2 + 0.5*pow(eInE,2))*pow(mP,2)*pow(1.*eInE*mP - 0.5*minQ2 + 0.5*pow(mP,2),-1),0.5) + pow(pow(eInE,2)*(-2.*eInE*mP*minQ2 + 2.*pow(eInE,2)*pow(mP,2) + 0.5*pow(minQ2,2))*pow(1.*mP*pow(eInE,3) + pow(eInE,2)*(-0.5*minQ2 + 0.5*pow(mP,2)),-1),0.5),2)/4.;

    if(ranges.getRangeT().getMax() >= 0.){
        changeKinematicRange(result.at(2), false, minT, "t");
    }

    double minY = minQ2/(2.*eInE*mP); 

    if(ranges.getRangeY().getMin() <= 0.){
        changeKinematicRange(result.at(0), true, minY, "y");
    }

    return result;
}

bool DVCSKinematicModule::checkIfValid(const ExperimentalConditions &conditions,
        const DVCSKinematic &kin) {

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();

    double y = kin.getY();
    double Q2 = kin.getQ2();
    double t = kin.getT();
    double phi = kin.getPhi();
    double phiS = kin.getPhiS();

    if (std::isnan(Ee) || std::isnan(Ep) || std::isnan(y) || std::isnan(Q2)
            || std::isnan(t) || std::isnan(phi) || std::isnan(phiS)) {

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

    if (Q2 <= 0.) {
        return false;
    }

    if (t >= 0.) {
        return false;
    }

    return true;
}

void DVCSKinematicModule::initModule() {
}

void DVCSKinematicModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

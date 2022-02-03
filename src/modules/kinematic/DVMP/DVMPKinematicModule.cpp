/*
 * DVMPKinematicModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/kinematic/DVMP/DVMPKinematicModule.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>
#include <partons/beans/MesonPolarization.h>
#include <cmath>

#include "../../../../include/beans/containers/ExperimentalConditions.h"
#include "../../../../include/beans/types/ParticleType.h"

namespace EPIC {

const std::string DVMPKinematicModule::DVMP_KINEMATIC_MODULE_CLASS_NAME =
        "DVMPKinematicModule";

DVMPKinematicModule::DVMPKinematicModule(const std::string &className) :
        KinematicModule<DVMPKinematic>(className, PARTONS::ChannelType::DVMP) {
}

DVMPKinematicModule::DVMPKinematicModule(const DVMPKinematicModule &other) :
        KinematicModule<DVMPKinematic>(other) {
}

DVMPKinematicModule::~DVMPKinematicModule() {
}

bool DVMPKinematicModule::runTest() const {

    // TODO
    return true;
}

bool DVMPKinematicModule::checkIfValid(const ExperimentalConditions &conditions,
        const DVMPKinematic &kin) {

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();

    double xB = kin.getXB();
    double t = kin.getT();
    double Q2 = kin.getQ2();
    double phi = kin.getPhi();
    double phiS = kin.getPhiS();
    ParticleType::Type mesonType = kin.getMesonType();
    PARTONS::MesonPolarization::Type mesonPolarization =
            kin.getMesonPolarization();

    if (std::isnan(Ee) || std::isnan(Ep) || std::isnan(xB) || std::isnan(t)
            || std::isnan(Q2) || std::isnan(phi) || std::isnan(phiS)) {

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

    if (xB <= 0. || xB >= 1.) {
        return false;
    }

    if (t >= 0.) {
        return false;
    }

    if (Q2 <= 0.) {
        return false;
    }

    if (mesonType == ParticleType::UNDEFINED) {
        return false;
    }

    return true;
}

void DVMPKinematicModule::initModule() {
}

void DVMPKinematicModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

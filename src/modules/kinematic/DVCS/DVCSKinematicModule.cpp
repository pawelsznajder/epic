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
        KinematicModule<DVCSKinematic>(className, PARTONS::ChannelType::DVCS) {
}

DVCSKinematicModule::DVCSKinematicModule(const DVCSKinematicModule &other) :
        KinematicModule<DVCSKinematic>(other) {
}

DVCSKinematicModule::~DVCSKinematicModule() {
}

bool DVCSKinematicModule::runTest() const {

    // TODO
    return true;
}

bool DVCSKinematicModule::checkIfValid(const ExperimentalConditions &conditions,
        const DVCSKinematic &kin) {

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();

    double xB = kin.getXB();
    double t = kin.getT();
    double Q2 = kin.getQ2();
    double phi = kin.getPhi();
    double phiS = kin.getPhiS();

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

    return true;
}

void DVCSKinematicModule::initModule() {
}

void DVCSKinematicModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

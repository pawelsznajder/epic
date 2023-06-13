/*
 * GAM2KinematicModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/kinematic/GAM2/GAM2KinematicModule.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>
#include <cmath>

#include "../../../../include/beans/containers/ExperimentalConditions.h"

namespace EPIC {

const std::string GAM2KinematicModule::GAM2_KINEMATIC_MODULE_CLASS_NAME =
        "GAM2KinematicModule";

GAM2KinematicModule::GAM2KinematicModule(const std::string &className) :
        KinematicModule<GAM2KinematicRanges, GAM2Kinematic>(className,
                PARTONS::ChannelType::GAM2) {
}

GAM2KinematicModule::GAM2KinematicModule(const GAM2KinematicModule &other) :
        KinematicModule<GAM2KinematicRanges, GAM2Kinematic>(other) {
}

GAM2KinematicModule::~GAM2KinematicModule() {
}

bool GAM2KinematicModule::runTest() const {

    // TODO
    return true;
}

std::vector<KinematicRange> GAM2KinematicModule::getKinematicRanges(const ExperimentalConditions &conditions, const GAM2KinematicRanges& ranges){

    //TODO implement correction of ranges
    std::vector<KinematicRange> result(6);

    result.at(0) = ranges.getRangeT();
    result.at(1) = ranges.getRangeUPrim();
    result.at(2) = ranges.getRangeMgg2();
    result.at(3) = ranges.getRangePhi();
    result.at(4) = ranges.getRangeY();
    result.at(5) = ranges.getRangeQ2();

    return result;
}

bool GAM2KinematicModule::checkIfValid(const ExperimentalConditions &conditions,
        const GAM2Kinematic &kin) {

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();

    double t = kin.getT();
    double uPrim = kin.getUPrim();
    double Mgg2 = kin.getMgg2();
    double phi = kin.getPhi();

    if (std::isnan(Ee) || std::isnan(Ep) || std::isnan(t) || std::isnan(uPrim)
            || std::isnan(Mgg2) || std::isnan(phi)) {

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

    if (t >= 0.) {
        return false;
    }

    if (uPrim >= 0.) {
        return false;
    }

    if (Mgg2 <= 0.) {
        return false;
    }

    return true;
}

void GAM2KinematicModule::initModule() {
}

void GAM2KinematicModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

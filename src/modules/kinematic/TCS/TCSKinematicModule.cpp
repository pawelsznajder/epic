/*
 * TCSKinematicModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/kinematic/TCS/TCSKinematicModule.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>
#include <cmath>

#include "../../../../include/beans/containers/ExperimentalConditions.h"

namespace EPIC {

const std::string TCSKinematicModule::TCS_KINEMATIC_MODULE_CLASS_NAME =
        "TCSKinematicModule";

const std::string TCSKinematicModule::TCS_KINEMATIC_MODULE_PARAMETER_DECAY_TYPE =
        "decay_type";

TCSKinematicModule::TCSKinematicModule(const std::string &className) :
        KinematicModule<DDVCSKinematic>(className, PARTONS::ChannelType::TCS), m_decayType(
                ParticleType::UNDEFINED) {
}

TCSKinematicModule::TCSKinematicModule(const TCSKinematicModule &other) :
        KinematicModule<DDVCSKinematic>(other), m_decayType(other.m_decayType) {
}

TCSKinematicModule::~TCSKinematicModule() {
}

void TCSKinematicModule::configure(const ElemUtils::Parameters &parameters) {

    // run for mother
    KinematicModule<DDVCSKinematic>::configure(parameters);

    // look for name
    if (parameters.isAvailable(
            TCSKinematicModule::TCS_KINEMATIC_MODULE_PARAMETER_DECAY_TYPE)) {

        // get
        m_decayType = ParticleType::fromString(
                parameters.getLastAvailable().getString());

        // print status
        info(__func__,
                ElemUtils::Formatter() << "Decay type set to: "
                        << ParticleType(m_decayType).toString());
    }
}

bool TCSKinematicModule::runTest() const {

    // TODO
    return true;
}

bool TCSKinematicModule::checkIfValid(const ExperimentalConditions &conditions,
        const DDVCSKinematic &kin) {

    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();

    double y = kin.getY();
    double Q2 = kin.getQ2();

    double t = kin.getT();
    double Q2Prim = kin.getQ2Prim();
    double phiS = kin.getPhiS();
    double phiL = kin.getPhiL();
    double thetaL = kin.getThetaL();

    if (std::isnan(Ee) || std::isnan(Ep) || std::isnan(y) || std::isnan(Q2)
            || std::isnan(t) || std::isnan(Q2Prim) || std::isnan(phiL)
            || std::isnan(phiS) || std::isnan(thetaL)) {

        warn(__func__,
                ElemUtils::Formatter() << "One or more of values in NaN: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
        return false;
    }

    if (Ee <= 0.)
        return false;

    if (Ep <= 0.)
        return false;

    if (y <= 0. || y >= 1.)
        return false;

    if (Q2 <= 0.)
        return false;

    if (t >= 0.)
        return false;

    if (Q2Prim <= 0.)
        return false;

    return true;
}

void TCSKinematicModule::initModule() {
}

void TCSKinematicModule::isModuleWellConfigured() {
}

ParticleType::Type TCSKinematicModule::getDecayType() const {
    return m_decayType;
}

void TCSKinematicModule::setDecayType(ParticleType::Type decayType) {
    m_decayType = decayType;
}

} /* namespace EPIC */


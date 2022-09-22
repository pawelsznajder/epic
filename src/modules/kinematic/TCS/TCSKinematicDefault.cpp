/*
 * TCSKinematicDefault.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/kinematic/TCS/TCSKinematicDefault.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <cmath>
#include <utility>
#include <vector>

#include "../../../../include/beans/containers/DDVCSKinematic.h"
#include "../../../../include/beans/containers/ExperimentalConditions.h"
#include "../../../../include/beans/physics/Particle.h"
#include "../../../../include/beans/physics/Vertex.h"
#include "../../../../include/beans/physics/Event.h"
#include "../../../../include/beans/types/AxisType.h"
#include "../../../../include/beans/types/ParticleCodeType.h"
#include "../../../../include/beans/types/ParticleType.h"
#include "../../../../include/Epic.h"
#include "../../../../include/managers/ModuleObjectFactory.h"
#include "../../../../include/modules/random_generator/RandomNumberGSL.h"

namespace EPIC {

const unsigned int TCSKinematicDefault::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSKinematicDefault("TCSKinematicDefault"));

TCSKinematicDefault::TCSKinematicDefault(const std::string &className) :
        TCSKinematicModule(className) {
}

TCSKinematicDefault::TCSKinematicDefault(const TCSKinematicDefault &other) :
        TCSKinematicModule(other) {
}

TCSKinematicDefault::~TCSKinematicDefault() {
}

TCSKinematicDefault *TCSKinematicDefault::clone() const {
    return new TCSKinematicDefault(*this);
}

bool TCSKinematicDefault::checkIfValid(const ExperimentalConditions &conditions,
        const DDVCSKinematic &kin) {

    //run for mother
    if (!TCSKinematicModule::checkIfValid(conditions, kin))
        return false;

    //variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();
    ParticleType::Type beamType = conditions.getLeptonType();
    ParticleType::Type targetType = conditions.getHadronType();

    double y = kin.getY();
    double Q2 = kin.getQ2();

    double t = kin.getT();
    double Q2Prim = kin.getQ2Prim();

    double Mp = ParticleType(targetType).getMass();

    Particle e_LAB(beamType, TVector3(0., 0., -1.), Ee);
    Particle p_LAB(targetType, TVector3(0., 0., 1.), Ep);

    TVector3 boost_LAB_to_TAR = p_LAB.getFourMomentum().BoostVector();

    Particle e_TAR = e_LAB;
    Particle p_TAR = p_LAB;

    e_TAR.boost(-boost_LAB_to_TAR);
    p_TAR.boost(-boost_LAB_to_TAR);

    // Q2
    if (Q2
            < pow(y * ParticleType(ParticleType::ELECTRON).getMass(), 2)
                    / (1. - y)) {
        return false;
    }

    double E_e_TAR = e_TAR.getEnergy();
    double E_eS_TAR = E_e_TAR * (1. - y);

    double cosTheta_eS_TAR = 1. - Q2 / (2 * E_e_TAR * E_eS_TAR);

    Particle eS_TAR(beamType, e_TAR.getMomentum().Unit(), E_eS_TAR);
    eS_TAR.rotate(AxisType::Y, acos(cosTheta_eS_TAR));

    Particle gamma_TAR(ParticleType::PHOTON,
            e_TAR.getFourMomentum() - eS_TAR.getFourMomentum());

    //s
    double s4 = (gamma_TAR.getFourMomentum() + p_TAR.getFourMomentum()).Mag2();

    if (pow(s4 - Q2Prim - pow(Mp, 2), 2) - 4 * Q2Prim * pow(Mp, 2) < 0.) {
        return false;
    }

    //tmin and tmax
    double m1_2 = -Q2;
    double m2_2 = pow(ParticleType(targetType).getMass(), 2);
    double m3_2 = Q2Prim;
    double m4_2 = pow(ParticleType(targetType).getMass(), 2);

    double E1cm = (s4 + m1_2 - m2_2) / (2 * sqrt(s4));
    double E3cm = (s4 + m3_2 - m4_2) / (2 * sqrt(s4));

    double p1cm = sqrt(pow(E1cm, 2) - m1_2);
    double p3cm = sqrt(pow(E3cm, 2) - m3_2);

    double tmin = pow((m1_2 - m3_2 - m2_2 + m4_2) / (2 * sqrt(s4)), 2)
            - pow(p1cm - p3cm, 2);
    double tmax = pow((m1_2 - m3_2 - m2_2 + m4_2) / (2 * sqrt(s4)), 2)
            - pow(p1cm + p3cm, 2);

    if (t > tmin || t < tmax) {
        return false;
    }

    //decay type
    if (sqrt(Q2Prim) < 2 * ParticleType(m_decayType).getMass()) {
        return false;
    }

    return true;
}

Event TCSKinematicDefault::evaluate(const ExperimentalConditions &conditions,
        const DDVCSKinematic &kin) {

    //variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();
    ParticleType::Type beamType = conditions.getLeptonType();
    ParticleType::Type targetType = conditions.getHadronType();
    ParticleType::Type exclusiveType = ParticleType::PHOTON;

    double y = kin.getY();
    double Q2 = kin.getQ2();

    double t = kin.getT();
    double Q2Prim = kin.getQ2Prim();
    double phi = kin.getPhi();
    double phiS = kin.getPhiS();
    double theta = kin.getTheta();

    // 0. Masses

    double Mp = ParticleType(targetType).getMass();

    // 1. Define particles for e and p in lab frame

    Particle e_LAB(beamType, TVector3(0., 0., -1.), Ee);
    Particle p_LAB(targetType, TVector3(0., 0., 1.), Ep);

    // 2. Boost to target rest frame

    TVector3 boost_LAB_to_TAR = p_LAB.getFourMomentum().BoostVector();

    Particle e_TAR = e_LAB;
    Particle p_TAR = p_LAB;

    e_TAR.boost(-boost_LAB_to_TAR);
    p_TAR.boost(-boost_LAB_to_TAR);

    // 3. Scattered electron

    if (Q2
            < pow(y * ParticleType(ParticleType::ELECTRON).getMass(), 2)
                    / (1. - y)) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Kinematics not valid, kinematics: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
    }

    //energy of scattered electron
    double E_e_TAR = e_TAR.getEnergy();
    double E_eS_TAR = E_e_TAR * (1. - y);

    //scattering angle
    double cosTheta_eS_TAR = 1. - Q2 / (2 * E_e_TAR * E_eS_TAR);

    Particle eS_TAR(beamType, e_TAR.getMomentum().Unit(), E_eS_TAR);
    eS_TAR.rotate(AxisType::Y, acos(cosTheta_eS_TAR));

    // 3. Initial photon

    Particle gamma_TAR(ParticleType::PHOTON,
            e_TAR.getFourMomentum() - eS_TAR.getFourMomentum());

    // 4. Evaluate tmin and tmax
    // see: http://pdg.lbl.gov/2018/reviews/rpp2018-rev-kinematics.pdf

    double s4 = (gamma_TAR.getFourMomentum() + p_TAR.getFourMomentum()).Mag2();

    if (pow(s4 - Q2Prim - pow(Mp, 2), 2) - 4 * Q2Prim * pow(Mp, 2) < 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Kinematics not valid, kinematics: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
    }

    double m1_2 = -Q2;
    double m2_2 = pow(ParticleType(targetType).getMass(), 2);
    double m3_2 = Q2Prim;
    double m4_2 = pow(ParticleType(targetType).getMass(), 2);

    double E1cm = (s4 + m1_2 - m2_2) / (2 * sqrt(s4));
    double E3cm = (s4 + m3_2 - m4_2) / (2 * sqrt(s4));

    double p1cm = sqrt(pow(E1cm, 2) - m1_2);
    double p3cm = sqrt(pow(E3cm, 2) - m3_2);

    double tmin = pow((m1_2 - m3_2 - m2_2 + m4_2) / (2 * sqrt(s4)), 2)
            - pow(p1cm - p3cm, 2);
    double tmax = pow((m1_2 - m3_2 - m2_2 + m4_2) / (2 * sqrt(s4)), 2)
            - pow(p1cm + p3cm, 2);

    if (t > tmin || t < tmax) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Kinematics not valid, kinematics: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
    }

    // 6. Boost to CMS

    Particle gamma_GP = gamma_TAR;
    Particle p_GP = p_TAR;

    //boost
    TVector3 boost_TAR_to_GP = (gamma_TAR.getFourMomentum()
            + p_TAR.getFourMomentum()).BoostVector();

    gamma_GP.boost(-boost_TAR_to_GP);
    p_GP.boost(-boost_TAR_to_GP);

    // 7. Scattered proton and exclusive particle
    //see: https://link.springer.com/content/pdf/bbm%3A978-3-319-60498-5%2F1.pdf

    //s in CMS

    double p_exclusive_GP = sqrt(
            (pow(s4 - Q2Prim - pow(Mp, 2), 2) - 4 * Q2Prim * pow(Mp, 2))
                    / (4 * s4));

    Particle exclusive_GP(ParticleType::PHOTON,
            TLorentzVector(p_exclusive_GP * (gamma_GP.getMomentum().Unit()),
                    sqrt(pow(p_exclusive_GP, 2) + Q2Prim)));
    Particle pS_GP(targetType, -exclusive_GP.getMomentum());

    //scattering angle
    double E_p_GP = p_GP.getEnergy();
    double p_p_GP = p_GP.getMomentum().Mag();

    double E_pS_GP = pS_GP.getEnergy();
    double p_pS_GP = pS_GP.getMomentum().Mag();

    double cosTheta_exclusive_GP = (t - pow(E_p_GP - E_pS_GP, 2)
            + pow(p_p_GP, 2) + pow(p_pS_GP, 2)) / (2 * p_p_GP * p_pS_GP);

    //rotate
    exclusive_GP.rotate(AxisType::Y, acos(cosTheta_exclusive_GP));
    pS_GP.rotate(AxisType::Y, acos(cosTheta_exclusive_GP));

    // 8. Back to target rest frame

    Particle exclusive_TAR = exclusive_GP;
    Particle pS_TAR = pS_GP;

    //boost
    exclusive_TAR.boost(boost_TAR_to_GP);
    pS_TAR.boost(boost_TAR_to_GP);

    // 9. Creation of lepton pair

    //boost to exclusive particle CMS
    TVector3 boost_TAR_to_EXC = exclusive_TAR.getFourMomentum().BoostVector();

    Particle p_EXC = p_TAR;
    Particle pS_EXC = pS_TAR;

    p_EXC.boost(-boost_TAR_to_EXC);
    pS_EXC.boost(-boost_TAR_to_EXC);

    //decay type
    if (m_decayType != ParticleType::ELECTRON
            && m_decayType != ParticleType::MUON_MINUS
            && m_decayType != ParticleType::TAU_MINUS) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "TCSParticles: " << __func__
                        << ": error: wrong decay type: "
                        << ParticleType(m_decayType).toString());
    }

    if (sqrt(Q2Prim) < 2 * ParticleType(m_decayType).getMass()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Kinematics not valid, kinematics: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
    }

    ParticleType::Type decayOpositeType;

    switch (m_decayType) {

    case ParticleType::ELECTRON: {
        decayOpositeType = ParticleType::POSITRON;
        break;
    }

    case ParticleType::MUON_MINUS: {
        decayOpositeType = ParticleType::MUON_PLUS;
        break;
    }

    case ParticleType::TAU_MINUS: {
        decayOpositeType = ParticleType::TAU_PLUS;
        break;
    }
    }

    //create pair (along direction of scattered target particle)
    Particle lepton1_EXC(m_decayType, pS_EXC.getMomentum().Unit(), sqrt(Q2Prim) / 2.);
    Particle lepton2_EXC(decayOpositeType, -pS_EXC.getMomentum().Unit(),
            sqrt(Q2Prim) / 2.);

    //rotate
    TVector3 rotate_theta =
            p_EXC.getMomentum().Cross(pS_EXC.getMomentum()).Unit();

    lepton1_EXC.rotate(rotate_theta, theta);
    lepton2_EXC.rotate(rotate_theta, theta);

    lepton1_EXC.rotate(pS_EXC.getMomentum().Unit(), phi);
    lepton2_EXC.rotate(pS_EXC.getMomentum().Unit(), phi);

    //back to target rest frame
    Particle lepton1_TAR = lepton1_EXC;
    Particle lepton2_TAR = lepton2_EXC;

    lepton1_TAR.boost(boost_TAR_to_EXC);
    lepton2_TAR.boost(boost_TAR_to_EXC);

    // 10. Back to LAB
    Particle gamma_LAB = gamma_TAR;
    Particle exclusive_LAB = exclusive_TAR;
    Particle eS_LAB = eS_TAR;
    Particle pS_LAB = pS_TAR;
    Particle lepton1_LAB = lepton1_TAR;
    Particle lepton2_LAB = lepton2_TAR;

    gamma_LAB.boost(boost_LAB_to_TAR);
    exclusive_LAB.boost(boost_LAB_to_TAR);
    eS_LAB.boost(boost_LAB_to_TAR);
    pS_LAB.boost(boost_LAB_to_TAR);
    lepton1_LAB.boost(boost_LAB_to_TAR);
    lepton2_LAB.boost(boost_LAB_to_TAR);

    // 11. Angle in LAB

    gamma_LAB.rotate(AxisType::Z, phiS);
    exclusive_LAB.rotate(AxisType::Z, phiS);
    eS_LAB.rotate(AxisType::Z, phiS);
    pS_LAB.rotate(AxisType::Z, phiS);
    lepton1_LAB.rotate(AxisType::Z, phiS);
    lepton2_LAB.rotate(AxisType::Z, phiS);

    // 12. Store

    Event event;

    std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > particles(
            8);

    particles.at(0) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared<Particle>(e_LAB));
    particles.at(1) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared<Particle>(p_LAB));
    particles.at(2) = std::make_pair(ParticleCodeType::SCATTERED,
            std::make_shared<Particle>(eS_LAB));
    particles.at(3) = std::make_pair(ParticleCodeType::VIRTUAL,
            std::make_shared<Particle>(gamma_LAB));
    particles.at(4) = std::make_pair(ParticleCodeType::SCATTERED,
            std::make_shared<Particle>(pS_LAB));
    particles.at(5) = std::make_pair(ParticleCodeType::DOCUMENTATION,
            std::make_shared<Particle>(exclusive_LAB));
    particles.at(6) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared<Particle>(lepton1_LAB));
    particles.at(7) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared<Particle>(lepton2_LAB));

    event.setParticles(particles);

    std::vector<std::shared_ptr<Vertex> > vertices(3);

    vertices.at(0) = std::make_shared<Vertex>();
    vertices.at(0)->addParticleIn(particles.at(0).second);
    vertices.at(0)->addParticleOut(particles.at(2).second);
    vertices.at(0)->addParticleOut(particles.at(3).second);

    vertices.at(1) = std::make_shared<Vertex>();
    vertices.at(1)->addParticleIn(particles.at(3).second);
    vertices.at(1)->addParticleIn(particles.at(1).second);
    vertices.at(1)->addParticleOut(particles.at(5).second);
    vertices.at(1)->addParticleOut(particles.at(4).second);

    vertices.at(2) = std::make_shared<Vertex>();
    vertices.at(2)->addParticleIn(particles.at(5).second);
    vertices.at(2)->addParticleOut(particles.at(6).second);
    vertices.at(2)->addParticleOut(particles.at(7).second);

    event.setVertices(vertices);

    // 13. Return
    return event;
}

} /* namespace EPIC */

/*
 * DVCSKinematicDefault.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/kinematic/DVCS/DVCSKinematicDefault.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "../../../../include/beans/containers/DVCSKinematic.h"
#include "../../../../include/beans/containers/ExperimentalConditions.h"
#include "../../../../include/beans/physics/Particle.h"
#include "../../../../include/beans/physics/Vertex.h"
#include "../../../../include/beans/physics/Event.h"
#include "../../../../include/beans/types/AxisType.h"
#include "../../../../include/beans/types/ParticleCodeType.h"
#include "../../../../include/beans/types/ParticleType.h"

namespace EPIC {

const unsigned int DVCSKinematicDefault::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSKinematicDefault("DVCSKinematicDefault"));

DVCSKinematicDefault::DVCSKinematicDefault(const std::string &className) :
        DVCSKinematicModule(className) {
}

DVCSKinematicDefault::DVCSKinematicDefault(const DVCSKinematicDefault &other) :
        DVCSKinematicModule(other) {
}

DVCSKinematicDefault::~DVCSKinematicDefault() {
}

DVCSKinematicDefault *DVCSKinematicDefault::clone() const {
    return new DVCSKinematicDefault(*this);
}

bool DVCSKinematicDefault::checkIfValid(
        const ExperimentalConditions &conditions, const DVCSKinematic &kin) {

    //run for mother
    if (!DVCSKinematicModule::checkIfValid(conditions, kin))
        return false;

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();
    ParticleType::Type beamType = conditions.getLeptonType();
    ParticleType::Type targetType = conditions.getHadronType();
    ParticleType::Type exclusiveType = ParticleType::PHOTON;

    double y = kin.getY();
    double Q2 = kin.getQ2();
    double t = kin.getT();

    double Mp = ParticleType(targetType).getMass();

    Particle e_LAB(beamType, TVector3(0., 0., -1.), Ee);
    Particle p_LAB(targetType, TVector3(0., 0., 1.), Ep);

    TVector3 boost_LAB_to_TAR = p_LAB.getFourMomentum().BoostVector();

    Particle e_TAR = e_LAB;
    Particle p_TAR = p_LAB;

    e_TAR.boost(-boost_LAB_to_TAR);
    p_TAR.boost(-boost_LAB_to_TAR);

    //y
    if (y < 0. || y > 1.) {
        return false;
    }

    //xB
    double xB = Q2 / (2 * y * Mp * e_TAR.getEnergy());

    if (xB
            < 2 * Q2 * e_TAR.getEnergy() / Mp
                    / (4 * pow(e_TAR.getEnergy(), 2) - Q2)) {
        return false;
    }

    double nu = Q2 / (2 * Mp * xB);

    double E_e_TAR = e_TAR.getEnergy();
    double E_eS_TAR = E_e_TAR - nu;

    double cosTheta_eS_TAR = 1. - Q2 / (2 * E_e_TAR * E_eS_TAR);

    Particle eS_TAR(beamType, e_TAR.getMomentum().Unit(), E_eS_TAR);
    eS_TAR.rotate(AxisType::Y, acos(cosTheta_eS_TAR));

    Particle gammaStar_TAR(ParticleType::PHOTON,
            e_TAR.getFourMomentum() - eS_TAR.getFourMomentum());

    //s
    double s4 =
            (gammaStar_TAR.getFourMomentum() + p_TAR.getFourMomentum()).Mag2();

    // tmin and tmax
    double m1_2 = -Q2;
    double m2_2 = pow(ParticleType(targetType).getMass(), 2);
    double m3_2 = pow(ParticleType(exclusiveType).getMass(), 2);
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

    return true;
}

Event DVCSKinematicDefault::evaluate(const ExperimentalConditions &conditions,
        const DVCSKinematic &kin) {

    // variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();
    ParticleType::Type beamType = conditions.getLeptonType();
    ParticleType::Type targetType = conditions.getHadronType();
    ParticleType::Type exclusiveType = ParticleType::PHOTON;

    double y = kin.getY();
    double Q2 = kin.getQ2();
    double t = kin.getT();
    double phi = kin.getPhi();
    double phiS = kin.getPhiS();

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

    if (y < 0. || y > 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Kinematics not valid, kinematics: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
    }

    double xB = Q2 / (2 * y * Mp * e_TAR.getEnergy());

    if (xB
            < 2 * Q2 * e_TAR.getEnergy() / Mp
                    / (4 * pow(e_TAR.getEnergy(), 2) - Q2)) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Kinematics not valid, kinematics: "
                        << kin.toString() << " experimental conditions"
                        << conditions.toString());
    }

    // 3. Scattered electron

    // energy of virtual photon
    double nu = Q2 / (2 * Mp * xB);

    // energy of scattered electron
    double E_e_TAR = e_TAR.getEnergy();
    double E_eS_TAR = E_e_TAR - nu;

    // scattering angle
    double cosTheta_eS_TAR = 1. - Q2 / (2 * E_e_TAR * E_eS_TAR);

    Particle eS_TAR(beamType, e_TAR.getMomentum().Unit(), E_eS_TAR);
    eS_TAR.rotate(AxisType::Y, acos(cosTheta_eS_TAR));

    // 4. Virtual photon

    Particle gammaStar_TAR(ParticleType::PHOTON,
            e_TAR.getFourMomentum() - eS_TAR.getFourMomentum());

    // 5. Evaluate tmin and tmax
    // see: http://pdg.lbl.gov/2018/reviews/rpp2018-rev-kinematics.pdf

    double s4 =
            (gammaStar_TAR.getFourMomentum() + p_TAR.getFourMomentum()).Mag2();

    double m1_2 = -Q2;
    double m2_2 = pow(ParticleType(targetType).getMass(), 2);
    double m3_2 = pow(ParticleType(exclusiveType).getMass(), 2);
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

    // 6. Rotate so that virtual photon goes along Z direction and boost to CMS

    Particle gammaStar_GP = gammaStar_TAR;
    Particle p_GP = p_TAR;

    // angle
    double angle_TAR_to_GP = gammaStar_TAR.getAngle(AxisType::Z);

    gammaStar_GP.rotate(AxisType::Y, -angle_TAR_to_GP);
    p_GP.rotate(AxisType::Y, -angle_TAR_to_GP);

    // boost
    TVector3 boost_TAR_to_GP = (gammaStar_GP.getFourMomentum()
            + p_GP.getFourMomentum()).BoostVector();

    gammaStar_GP.boost(-boost_TAR_to_GP);
    p_GP.boost(-boost_TAR_to_GP);

    // 7. Scattered proton and exclusive particle
    // see: https://link.springer.com/content/pdf/bbm%3A978-3-319-60498-5%2F1.pdf

    // s in CMS

    double p_exclusive_GP = sqrt((pow(s4 - pow(Mp, 2), 2)) / (4 * s4));

    Particle exclusive_GP(exclusiveType,
            p_exclusive_GP * (gammaStar_GP.getMomentum().Unit()));
    Particle pS_GP(targetType, -exclusive_GP.getMomentum());

    // scattering angle
    double E_p_GP = p_GP.getEnergy();
    double p_p_GP = p_GP.getMomentum().Mag();

    double E_pS_GP = pS_GP.getEnergy();
    double p_pS_GP = pS_GP.getMomentum().Mag();

    double cosTheta_exclusive_GP = (t - pow(E_p_GP - E_pS_GP, 2)
            + pow(p_p_GP, 2) + pow(p_pS_GP, 2)) / (2 * p_p_GP * p_pS_GP);

    // rotate
    exclusive_GP.rotate(AxisType::Y, acos(cosTheta_exclusive_GP));
    pS_GP.rotate(AxisType::Y, acos(cosTheta_exclusive_GP));

    // 8. Back to target rest frame

    Particle exclusive_TAR = exclusive_GP;
    Particle pS_TAR = pS_GP;

    // boost
    exclusive_TAR.boost(boost_TAR_to_GP);
    pS_TAR.boost(boost_TAR_to_GP);

    // rotate
    exclusive_TAR.rotate(AxisType::Y, angle_TAR_to_GP);
    pS_TAR.rotate(AxisType::Y, angle_TAR_to_GP);

    // 9. Introduce phi angle

    pS_TAR.rotate(gammaStar_TAR.getMomentum().Unit(), phi);
    exclusive_TAR.rotate(gammaStar_TAR.getMomentum().Unit(), phi);

    // 10. Back to LAB

    Particle eS_LAB = eS_TAR;
    Particle gammaStar_LAB = gammaStar_TAR;
    Particle exclusive_LAB = exclusive_TAR;
    Particle pS_LAB = pS_TAR;

    eS_LAB.boost(boost_LAB_to_TAR);
    gammaStar_LAB.boost(boost_LAB_to_TAR);
    exclusive_LAB.boost(boost_LAB_to_TAR);
    pS_LAB.boost(boost_LAB_to_TAR);

    // 11. Introduce phiS angle

    //rotation angle
    double psi;

    //target polarisation has transverse component
    if (conditions.getHadronPolarisation().getX() != 0.
            || conditions.getHadronPolarisation().getY() != 0.) {

        // Gamma factor
        double gamma = 2 * Mp * xB / sqrt(Q2);

        // Sine angle between the incoming lepton and the virtual photon
        double sinTheta = gamma
                * sqrt(
                        (1 - y - 1 / 4 * pow(y, 2) * pow(gamma, 2))
                                / (1 + pow(gamma, 2)));

        // Cosine angle between the incoming lepton and the virtual photon
        double cosTheta = sqrt(1. - pow(sinTheta, 2));

        // Longitudinal polarization
        double PL = conditions.getHadronPolarisation().getZ();

        // Transverse polarization, a positive definite number between 0 and 1
        double PT = sqrt(
                pow(conditions.getHadronPolarisation().getX(), 2)
                        + pow(conditions.getHadronPolarisation().getY(), 2));

        // Transverse polarization w.r.t the virtual photon's direction
        double ST =
                (-PL * cos(phiS) * sinTheta
                        + sqrt(
                                pow(cosTheta, 2)
                                        * (-pow(PL, 2) + pow(cosTheta, 2)
                                                + pow(cos(phiS), 2)
                                                        * pow(sinTheta, 2))))
                        / (pow(cosTheta, 2)
                                + pow(cos(phiS), 2) * pow(sinTheta, 2));

        // Sine angle value to be rotated in the lab frame
        double sinPsi = ST / PT * sin(phiS);

        // Cosine angle value to be rotated in the lab frame
        double cosPsi = (ST * cos(phiS) + sinTheta * PL) / (cosTheta * PT);

        // rotate angle
        psi = atan2(sinPsi, cosPsi);

    } else {
        psi = phiS;
    }

    eS_LAB.rotate(AxisType::Z, psi);
    gammaStar_LAB.rotate(AxisType::Z, psi);
    exclusive_LAB.rotate(AxisType::Z, psi);
    pS_LAB.rotate(AxisType::Z, psi);

    // 12. Store

    Event event;

    std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > particles(
            6);

    particles.at(0) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared<Particle>(e_LAB));
    particles.at(1) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared<Particle>(p_LAB));
    particles.at(2) = std::make_pair(ParticleCodeType::SCATTERED,
            std::make_shared<Particle>(eS_LAB));
    particles.at(3) = std::make_pair(ParticleCodeType::VIRTUAL,
            std::make_shared<Particle>(gammaStar_LAB));
    particles.at(4) = std::make_pair(ParticleCodeType::SCATTERED,
            std::make_shared<Particle>(pS_LAB));
    particles.at(5) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared<Particle>(exclusive_LAB));

    event.setParticles(particles);

    std::vector<std::shared_ptr<Vertex> > vertices(2);

    vertices.at(0) = std::make_shared<Vertex>();
    vertices.at(0)->addParticleIn(particles.at(0).second);
    vertices.at(0)->addParticleOut(particles.at(2).second);
    vertices.at(0)->addParticleOut(particles.at(3).second);

    vertices.at(1) = std::make_shared<Vertex>();
    vertices.at(1)->addParticleIn(particles.at(3).second);
    vertices.at(1)->addParticleIn(particles.at(1).second);
    vertices.at(1)->addParticleOut(particles.at(5).second);
    vertices.at(1)->addParticleOut(particles.at(4).second);

    event.setVertices(vertices);

    // 13. Return
    return event;
}

} /* namespace EPIC */

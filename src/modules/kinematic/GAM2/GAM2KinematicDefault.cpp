/*
 * GAM2KinematicDefault.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/kinematic/GAM2/GAM2KinematicDefault.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "../../../../include/beans/containers/GAM2Kinematic.h"
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

const unsigned int GAM2KinematicDefault::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2KinematicDefault("GAM2KinematicDefault"));

GAM2KinematicDefault::GAM2KinematicDefault(const std::string &className) :
        GAM2KinematicModule(className) {
    m_randomNumberModule = nullptr;
}

GAM2KinematicDefault::GAM2KinematicDefault(const GAM2KinematicDefault &other) :
        GAM2KinematicModule(other) {

    if (other.m_randomNumberModule == nullptr) {
        m_randomNumberModule = nullptr;
    } else {
        m_randomNumberModule = std::make_shared < RandomNumberGSL
                > (RandomNumberGSL(
                        *(std::static_pointer_cast < RandomNumberGSL
                                > (other.m_randomNumberModule))));
    }
}

GAM2KinematicDefault::~GAM2KinematicDefault() {
}

GAM2KinematicDefault *GAM2KinematicDefault::clone() const {
    return new GAM2KinematicDefault(*this);
}

bool GAM2KinematicDefault::checkIfValid(
        const ExperimentalConditions &conditions, const GAM2Kinematic &kin) {

    //run for mother
    if (!GAM2KinematicModule::checkIfValid(conditions, kin))
        return false;

    //variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();
    ParticleType::Type beamType = conditions.getLeptonType();
    ParticleType::Type targetType = conditions.getHadronType();

    double y = kin.getY();
    double Q2 = kin.getQ2();

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
        return false;
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

    // 4. Rotate such as gamma* goes along x direction
    double cosTheta_gamma_TAR = gamma_TAR.getMomentum().X()
            / gamma_TAR.getMomentum().Mag();

    Particle gamma_X = gamma_TAR;
    Particle p_X = p_TAR;

    gamma_X.rotate(AxisType::Y, -1 * acos(cosTheta_gamma_TAR));
    p_X.rotate(AxisType::Y, -1 * acos(cosTheta_gamma_TAR));

    // 5. Boost to gamma* - p CMS
    TVector3 boost_X_to_CMS =
            (gamma_X.getFourMomentum() + p_X.getFourMomentum()).BoostVector();

    Particle gamma_CMS = gamma_X;
    Particle p_CMS = p_X;

    gamma_CMS.boost(-boost_X_to_CMS);
    p_CMS.boost(-boost_X_to_CMS);

    //6. Four momenta of particles

    double s = (gamma_CMS.getFourMomentum() + p_CMS.getFourMomentum()).Mag2();
    double t = kin.getT();
    double uPrim = kin.getUPrim();
    double Mgg2 = kin.getMgg2();
    double q2 = gamma_CMS.getFourMomentum().Mag2();

    double E1 = ((q2 + s - pow(Mp, 2)) * pow(s, -0.5)) / 2.;
    double E2 = ((-q2 + s + pow(Mp, 2)) * pow(s, -0.5)) / 2.;
    double E3 = ((Mgg2 + s - pow(Mp, 2)) * pow(s, -0.5)) / 2.;
    double E4 = ((-Mgg2 + s + pow(Mp, 2)) * pow(s, -0.5)) / 2.;

    if (-2 * q2 * s - 2 * q2 * pow(Mp, 2) - 2 * s * pow(Mp, 2) + pow(Mp, 4)
            + pow(q2, 2) + pow(s, 2) < 0.)
        return false;
    if (-2 * Mgg2 * s + pow(Mgg2, 2) - 2 * Mgg2 * pow(Mp, 2)
            - 2 * s * pow(Mp, 2) + pow(Mp, 4) + pow(s, 2) < 0.)
        return false;

    if (pow(Mgg2, 2) - 2 * Mgg2 * (s + pow(Mp, 2)) + pow(-s + pow(Mp, 2), 2)
            < 0.)
        return false;
    if (-2 * (q2 + s) * pow(Mp, 2) + pow(Mp, 4) + pow(q2 - s, 2) < 0.)
        return false;

    double p = pow(
            pow(s, -1)
                    * (-2 * q2 * s - 2 * q2 * pow(Mp, 2) - 2 * s * pow(Mp, 2)
                            + pow(Mp, 4) + pow(q2, 2) + pow(s, 2)), 0.5) / 2.;
    double pPrim = pow(
            pow(s, -1)
                    * (-2 * Mgg2 * s + pow(Mgg2, 2) - 2 * Mgg2 * pow(Mp, 2)
                            - 2 * s * pow(Mp, 2) + pow(Mp, 4) + pow(s, 2)), 0.5)
            / 2.;
    double cosTheta = (s * (-q2 + s + 2 * t) - (q2 + 2 * s) * pow(Mp, 2)
            - Mgg2 * (-q2 + s + pow(Mp, 2)) + pow(Mp, 4))
            * pow(-2 * (q2 + s) * pow(Mp, 2) + pow(Mp, 4) + pow(q2 - s, 2),
                    -0.5)
            * pow(
                    pow(Mgg2, 2) - 2 * Mgg2 * (s + pow(Mp, 2))
                            + pow(-s + pow(Mp, 2), 2), -0.5);
    double sinTheta = TMath::Sqrt(1 - cosTheta * cosTheta);

    if (fabs(cosTheta) > 1.)
        return false;

    double lv3X = pPrim * cosTheta;
    double lv3Y = pPrim * sinTheta; //sign of sinTheta does not matter as below lv3Y always appear in even power
    double lv3E = E3;
    double lv1X = p;
    double lv1E = E1;
    double uP = uPrim;
    double angle = kin.getPhi();

    double sqrtFactor2 =
            pow(lv1X * lv3E - lv1E * lv3X, 2)
                    * (4 * lv1X * lv3X * uP * pow(lv3E, 2)
                            - 4 * uP * pow(lv1X, 2) * pow(lv3E, 2)
                            + 4 * lv3X * pow(lv1X, 3) * pow(lv3E, 2)
                            - 2 * pow(lv1X, 4) * pow(lv3E, 2)
                            + 2 * pow(lv1X, 2) * pow(lv3E, 4)
                            + 4 * uP * pow(lv1X, 2) * pow(lv3X, 2)
                            + 2 * pow(lv1X, 4) * pow(lv3X, 2)
                            - 4 * pow(lv1X, 2) * pow(lv3E, 2) * pow(lv3X, 2)
                            - 4 * lv1X * uP * pow(lv3X, 3)
                            - 4 * pow(lv1X, 3) * pow(lv3X, 3)
                            + 2 * pow(lv1X, 2) * pow(lv3X, 4)
                            + 4 * lv3E * pow(lv1E, 3)
                                    * (pow(lv3E, 2) - pow(lv3X, 2)
                                            - pow(lv3Y, 2))
                            - 4 * lv1E * lv3E * (uP + pow(lv1X, 2))
                                    * (pow(lv3E, 2) - pow(lv3X, 2)
                                            - pow(lv3Y, 2))
                            - 4 * lv1X * lv3X * uP * pow(lv3Y, 2)
                            + 2 * uP * pow(lv1X, 2) * pow(lv3Y, 2)
                            - 4 * lv3X * pow(lv1X, 3) * pow(lv3Y, 2)
                            + pow(lv1X, 4) * pow(lv3Y, 2)
                            - 4 * pow(lv1X, 2) * pow(lv3E, 2) * pow(lv3Y, 2)
                            + 4 * pow(lv1X, 2) * pow(lv3X, 2) * pow(lv3Y, 2)
                            + pow(lv1E, 4)
                                    * (-2 * pow(lv3E, 2) + 2 * pow(lv3X, 2)
                                            + pow(lv3Y, 2))
                            + pow(lv1E, 2)
                                    * (4 * lv1X * lv3X
                                            * (-pow(lv3E, 2) + pow(lv3X, 2)
                                                    + pow(lv3Y, 2))
                                            - 2
                                                    * (pow(lv3E, 4)
                                                            + pow(lv3X, 4)
                                                            + 2 * pow(lv3X, 2)
                                                                    * (uP
                                                                            + pow(
                                                                                    lv3Y,
                                                                                    2))
                                                            + pow(lv3Y, 2)
                                                                    * (uP
                                                                            + pow(
                                                                                    lv3Y,
                                                                                    2))
                                                            - 2 * pow(lv3E, 2)
                                                                    * (uP
                                                                            + pow(
                                                                                    lv3X,
                                                                                    2)
                                                                            + pow(
                                                                                    lv3Y,
                                                                                    2)))
                                            + pow(lv1X, 2)
                                                    * (4 * pow(lv3E, 2)
                                                            - 2
                                                                    * (2
                                                                            * pow(
                                                                                    lv3X,
                                                                                    2)
                                                                            + pow(
                                                                                    lv3Y,
                                                                                    2))))
                            + 2 * pow(lv1X, 2) * pow(lv3Y, 4)
                            - 2 * pow(lv3E, 2) * pow(uP, 2)
                            + 2 * pow(lv3X, 2) * pow(uP, 2)
                            + pow(lv3Y, 2) * pow(uP, 2)
                            + cos(2 * angle) * pow(lv3Y, 2)
                                    * pow(uP - pow(lv1E, 2) + pow(lv1X, 2), 2));

    if (sqrtFactor2 < 0.)
        return false;

    return true;
}

Event GAM2KinematicDefault::evaluate(const ExperimentalConditions &conditions,
        const GAM2Kinematic &kin) {

    //random
    if (m_randomNumberModule == nullptr) {
        m_randomNumberModule =
                Epic::getInstance()->getModuleObjectFactory()->newRandomNumberModule(
                        RandomNumberGSL::classId);
    }

    //variables
    double Ee = conditions.getLeptonEnergy();
    double Ep = conditions.getHadronEnergy();
    ParticleType::Type beamType = conditions.getLeptonType();
    ParticleType::Type targetType = conditions.getHadronType();

    double y = kin.getY();
    double Q2 = kin.getQ2();

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

    // 4. Rotate such as gamma* goes along x direction
    double cosTheta_gamma_TAR = gamma_TAR.getMomentum().X()
            / gamma_TAR.getMomentum().Mag();

    Particle gamma_X = gamma_TAR;
    Particle p_X = p_TAR;

    gamma_X.rotate(AxisType::Y, -1 * acos(cosTheta_gamma_TAR));
    p_X.rotate(AxisType::Y, -1 * acos(cosTheta_gamma_TAR));

    // 5. Boost to gamma* - p CMS
    TVector3 boost_X_to_CMS =
            (gamma_X.getFourMomentum() + p_X.getFourMomentum()).BoostVector();

    Particle gamma_CMS = gamma_X;
    Particle p_CMS = p_X;

    gamma_CMS.boost(-boost_X_to_CMS);
    p_CMS.boost(-boost_X_to_CMS);

    //6. Four momenta of particles

    double s = (gamma_CMS.getFourMomentum() + p_CMS.getFourMomentum()).Mag2();
    double t = kin.getT();
    double uPrim = kin.getUPrim();
    double Mgg2 = kin.getMgg2();
    double q2 = gamma_CMS.getFourMomentum().Mag2();

    double E1 = ((q2 + s - pow(Mp, 2)) * pow(s, -0.5)) / 2.;
    double E2 = ((-q2 + s + pow(Mp, 2)) * pow(s, -0.5)) / 2.;
    double E3 = ((Mgg2 + s - pow(Mp, 2)) * pow(s, -0.5)) / 2.;
    double E4 = ((-Mgg2 + s + pow(Mp, 2)) * pow(s, -0.5)) / 2.;

    double p = pow(
            pow(s, -1)
                    * (-2 * q2 * s - 2 * q2 * pow(Mp, 2) - 2 * s * pow(Mp, 2)
                            + pow(Mp, 4) + pow(q2, 2) + pow(s, 2)), 0.5) / 2.;
    double pPrim = pow(
            pow(s, -1)
                    * (-2 * Mgg2 * s + pow(Mgg2, 2) - 2 * Mgg2 * pow(Mp, 2)
                            - 2 * s * pow(Mp, 2) + pow(Mp, 4) + pow(s, 2)), 0.5)
            / 2.;
    double cosTheta = (s * (-q2 + s + 2 * t) - (q2 + 2 * s) * pow(Mp, 2)
            - Mgg2 * (-q2 + s + pow(Mp, 2)) + pow(Mp, 4))
            * pow(-2 * (q2 + s) * pow(Mp, 2) + pow(Mp, 4) + pow(q2 - s, 2),
                    -0.5)
            * pow(
                    pow(Mgg2, 2) - 2 * Mgg2 * (s + pow(Mp, 2))
                            + pow(-s + pow(Mp, 2), 2), -0.5);
    double sinTheta = ((m_randomNumberModule->diceFlat() < 0.5) ? (-1.) : (1.))
            * TMath::Sqrt(1 - cosTheta * cosTheta);

    TLorentzVector lvGamma(p, 0, 0, E1);
    TLorentzVector lvP(-p, 0, 0, E2);

    TLorentzVector lvGamma3(pPrim * cosTheta, pPrim * sinTheta, 0, E3);
    TLorentzVector lvPPrim(-pPrim * cosTheta, -pPrim * sinTheta, 0, E4);

    double lv3X = lvGamma3[0];
    double lv3Y = lvGamma3[1];
    double lv3E = lvGamma3[3];
    double lv1X = lvGamma[0];
    double lv1E = lvGamma[3];
    double uP = uPrim;
    double angle = kin.getPhi();

    double lv3AX, lv3AY, lv3BX, lv3BY;

    double solution = m_randomNumberModule->diceFlat();

    double sqrtFactor =
            pow(
                    pow(lv1X * lv3E - lv1E * lv3X, 2)
                            * (4 * lv1X * lv3X * uP * pow(lv3E, 2)
                                    - 4 * uP * pow(lv1X, 2) * pow(lv3E, 2)
                                    + 4 * lv3X * pow(lv1X, 3) * pow(lv3E, 2)
                                    - 2 * pow(lv1X, 4) * pow(lv3E, 2)
                                    + 2 * pow(lv1X, 2) * pow(lv3E, 4)
                                    + 4 * uP * pow(lv1X, 2) * pow(lv3X, 2)
                                    + 2 * pow(lv1X, 4) * pow(lv3X, 2)
                                    - 4 * pow(lv1X, 2) * pow(lv3E, 2)
                                            * pow(lv3X, 2)
                                    - 4 * lv1X * uP * pow(lv3X, 3)
                                    - 4 * pow(lv1X, 3) * pow(lv3X, 3)
                                    + 2 * pow(lv1X, 2) * pow(lv3X, 4)
                                    + 4 * lv3E * pow(lv1E, 3)
                                            * (pow(lv3E, 2) - pow(lv3X, 2)
                                                    - pow(lv3Y, 2))
                                    - 4 * lv1E * lv3E * (uP + pow(lv1X, 2))
                                            * (pow(lv3E, 2) - pow(lv3X, 2)
                                                    - pow(lv3Y, 2))
                                    - 4 * lv1X * lv3X * uP * pow(lv3Y, 2)
                                    + 2 * uP * pow(lv1X, 2) * pow(lv3Y, 2)
                                    - 4 * lv3X * pow(lv1X, 3) * pow(lv3Y, 2)
                                    + pow(lv1X, 4) * pow(lv3Y, 2)
                                    - 4 * pow(lv1X, 2) * pow(lv3E, 2)
                                            * pow(lv3Y, 2)
                                    + 4 * pow(lv1X, 2) * pow(lv3X, 2)
                                            * pow(lv3Y, 2)
                                    + pow(lv1E, 4)
                                            * (-2 * pow(lv3E, 2)
                                                    + 2 * pow(lv3X, 2)
                                                    + pow(lv3Y, 2))
                                    + pow(lv1E, 2)
                                            * (4 * lv1X * lv3X
                                                    * (-pow(lv3E, 2)
                                                            + pow(lv3X, 2)
                                                            + pow(lv3Y, 2))
                                                    - 2
                                                            * (pow(lv3E, 4)
                                                                    + pow(lv3X,
                                                                            4)
                                                                    + 2
                                                                            * pow(
                                                                                    lv3X,
                                                                                    2)
                                                                            * (uP
                                                                                    + pow(
                                                                                            lv3Y,
                                                                                            2))
                                                                    + pow(lv3Y,
                                                                            2)
                                                                            * (uP
                                                                                    + pow(
                                                                                            lv3Y,
                                                                                            2))
                                                                    - 2
                                                                            * pow(
                                                                                    lv3E,
                                                                                    2)
                                                                            * (uP
                                                                                    + pow(
                                                                                            lv3X,
                                                                                            2)
                                                                                    + pow(
                                                                                            lv3Y,
                                                                                            2)))
                                                    + pow(lv1X, 2)
                                                            * (4 * pow(lv3E, 2)
                                                                    - 2
                                                                            * (2
                                                                                    * pow(
                                                                                            lv3X,
                                                                                            2)
                                                                                    + pow(
                                                                                            lv3Y,
                                                                                            2))))
                                    + 2 * pow(lv1X, 2) * pow(lv3Y, 4)
                                    - 2 * pow(lv3E, 2) * pow(uP, 2)
                                    + 2 * pow(lv3X, 2) * pow(uP, 2)
                                    + pow(lv3Y, 2) * pow(uP, 2)
                                    + cos(2 * angle) * pow(lv3Y, 2)
                                            * pow(
                                                    uP - pow(lv1E, 2)
                                                            + pow(lv1X, 2), 2)),
                    0.5);

    //results 1
    if (solution < 0.5) {
        lv3AX =
                -0.5
                        * (pow(lv1X * lv3E - lv1E * lv3X, -1)
                                * pow(
                                        -4 * lv1E * lv1X * lv3E * lv3X
                                                + pow(lv1X, 2)
                                                        * (2 * pow(lv3E, 2)
                                                                - pow(lv3Y, 2))
                                                + cos(2 * angle)
                                                        * (pow(lv1E, 2)
                                                                - pow(lv1X, 2))
                                                        * pow(lv3Y, 2)
                                                + pow(lv1E, 2)
                                                        * (2 * pow(lv3X, 2)
                                                                + pow(lv3Y, 2)),
                                        -1)
                                * (lv1X * (lv1X * lv3E - lv1E * lv3X)
                                        * cos(2 * angle)
                                        * (uP - pow(lv1E, 2) + pow(lv1X, 2))
                                        * pow(lv3Y, 2)
                                        - (lv1X * lv3E - lv1E * lv3X)
                                                * (2 * lv3E * lv3X
                                                        * pow(lv1E, 3)
                                                        + lv1X
                                                                * (uP
                                                                        + pow(
                                                                                lv1X,
                                                                                2))
                                                                * (2
                                                                        * pow(
                                                                                lv3E,
                                                                                2)
                                                                        - pow(
                                                                                lv3Y,
                                                                                2))
                                                        - 2 * lv1E * lv3E
                                                                * (lv3X * uP
                                                                        + lv3X
                                                                                * pow(
                                                                                        lv1X,
                                                                                        2)
                                                                        + lv1X
                                                                                * (-pow(
                                                                                        lv3E,
                                                                                        2)
                                                                                        + pow(
                                                                                                lv3X,
                                                                                                2)
                                                                                        + pow(
                                                                                                lv3Y,
                                                                                                2)))
                                                        + pow(lv1E, 2)
                                                                * (lv1X
                                                                        * (-2
                                                                                * pow(
                                                                                        lv3E,
                                                                                        2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2))
                                                                        + 2
                                                                                * lv3X
                                                                                * (-pow(
                                                                                        lv3E,
                                                                                        2)
                                                                                        + pow(
                                                                                                lv3X,
                                                                                                2)
                                                                                        + pow(
                                                                                                lv3Y,
                                                                                                2))))
                                        + lv1E * lv3Y * cos(angle) * pow(2, 0.5)
                                                * sqrtFactor));

        lv3AY = pow(
                -4 * lv1E * lv1X * lv3E * lv3X
                        + pow(lv1X, 2) * (2 * pow(lv3E, 2) - pow(lv3Y, 2))
                        + cos(2 * angle) * (pow(lv1E, 2) - pow(lv1X, 2))
                                * pow(lv3Y, 2)
                        + pow(lv1E, 2) * (2 * pow(lv3X, 2) + pow(lv3Y, 2)), -1)
                * (-(lv1E * lv3E * lv3Y * uP * cos(angle))
                        + lv1X * lv3X * lv3Y * uP * cos(angle)
                        - lv1X * lv3X * lv3Y * cos(angle) * pow(lv1E, 2)
                        + lv3E * lv3Y * cos(angle) * pow(lv1E, 3)
                        - lv1E * lv3E * lv3Y * cos(angle) * pow(lv1X, 2)
                        + lv3X * lv3Y * cos(angle) * pow(lv1X, 3)
                        - lv3Y * cos(angle) * pow(lv1E, 2) * pow(lv3E, 2)
                        + lv3Y * cos(angle) * pow(lv1X, 2) * pow(lv3E, 2)
                        + lv3Y * cos(angle) * pow(lv1E, 2) * pow(lv3X, 2)
                        - lv3Y * cos(angle) * pow(lv1X, 2) * pow(lv3X, 2)
                        + cos(angle) * pow(lv1E, 2) * pow(lv3Y, 3)
                        - cos(angle) * pow(lv1X, 2) * pow(lv3Y, 3)
                        - pow(2, -0.5) * sqrtFactor);

        lv3BX =
                (pow(lv1X * lv3E - lv1E * lv3X, -1)
                        * pow(
                                -4 * lv1E * lv1X * lv3E * lv3X
                                        + pow(lv1X, 2)
                                                * (2 * pow(lv3E, 2)
                                                        - pow(lv3Y, 2))
                                        + cos(2 * angle)
                                                * (pow(lv1E, 2) - pow(lv1X, 2))
                                                * pow(lv3Y, 2)
                                        + pow(lv1E, 2)
                                                * (2 * pow(lv3X, 2)
                                                        + pow(lv3Y, 2)), -1)
                        * ((lv1X * lv3E - lv1E * lv3X) * cos(2 * angle)
                                * (-((lv1X - 2 * lv3X) * pow(lv1E, 2))
                                        + lv1X
                                                * (-2 * lv1X * lv3X + uP
                                                        + pow(lv1X, 2)))
                                * pow(lv3Y, 2)
                                + (-(lv1X * lv3E) + lv1E * lv3X)
                                        * (2 * lv3E * lv3X * pow(lv1E, 3)
                                                + lv1X
                                                        * (-2 * lv1X * lv3X + uP
                                                                + pow(lv1X, 2))
                                                        * (2 * pow(lv3E, 2)
                                                                - pow(lv3Y, 2))
                                                + pow(lv1E, 2)
                                                        * (-2 * lv3X
                                                                * (pow(lv3E, 2)
                                                                        + pow(
                                                                                lv3X,
                                                                                2))
                                                                + lv1X
                                                                        * (-2
                                                                                * pow(
                                                                                        lv3E,
                                                                                        2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2)))
                                                - 2 * lv1E * lv3E
                                                        * (lv3X * uP
                                                                + lv3X
                                                                        * pow(
                                                                                lv1X,
                                                                                2)
                                                                + lv1X
                                                                        * (-pow(
                                                                                lv3E,
                                                                                2)
                                                                                - 3
                                                                                        * pow(
                                                                                                lv3X,
                                                                                                2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2))))
                                + lv1E * lv3Y * cos(angle) * pow(2, 0.5)
                                        * sqrtFactor)) / 2.;

        lv3BY = cos(angle)
                * pow(
                        -4 * lv1E * lv1X * lv3E * lv3X
                                + pow(lv1X, 2)
                                        * (2 * pow(lv3E, 2) - pow(lv3Y, 2))
                                + cos(2 * angle) * (pow(lv1E, 2) - pow(lv1X, 2))
                                        * pow(lv3Y, 2)
                                + pow(lv1E, 2)
                                        * (2 * pow(lv3X, 2) + pow(lv3Y, 2)), -1)
                * (-4 * lv1E * lv1X * lv3E * lv3X * lv3Y
                        + lv1E * lv3E * lv3Y * uP - lv1X * lv3X * lv3Y * uP
                        + lv1X * lv3X * lv3Y * pow(lv1E, 2)
                        - lv3E * lv3Y * pow(lv1E, 3)
                        + lv1E * lv3E * lv3Y * pow(lv1X, 2)
                        - lv3X * lv3Y * pow(lv1X, 3)
                        + lv3Y * pow(lv1E, 2) * pow(lv3E, 2)
                        + lv3Y * pow(lv1X, 2) * pow(lv3E, 2)
                        + lv3Y * pow(lv1E, 2) * pow(lv3X, 2)
                        + lv3Y * pow(lv1X, 2) * pow(lv3X, 2)
                        + pow(lv1E, 2) * pow(lv3Y, 3)
                        - pow(lv1X, 2) * pow(lv3Y, 3)
                        - 4 * lv1E * lv1X * lv3E * lv3X * lv3Y
                                * pow(tan(angle), 2)
                        + 2 * lv3Y * pow(lv1X, 2) * pow(lv3E, 2)
                                * pow(tan(angle), 2)
                        + 2 * lv3Y * pow(lv1E, 2) * pow(lv3X, 2)
                                * pow(tan(angle), 2)
                        + pow(2, -0.5) * sqrtFactor * 1 / cos(angle));
    } else {

        lv3AX =
                (pow(lv1X * lv3E - lv1E * lv3X, -1)
                        * pow(
                                -4 * lv1E * lv1X * lv3E * lv3X
                                        + pow(lv1X, 2)
                                                * (2 * pow(lv3E, 2)
                                                        - pow(lv3Y, 2))
                                        + cos(2 * angle)
                                                * (pow(lv1E, 2) - pow(lv1X, 2))
                                                * pow(lv3Y, 2)
                                        + pow(lv1E, 2)
                                                * (2 * pow(lv3X, 2)
                                                        + pow(lv3Y, 2)), -1)
                        * (-(lv1X * (lv1X * lv3E - lv1E * lv3X) * cos(2 * angle)
                                * (uP - pow(lv1E, 2) + pow(lv1X, 2))
                                * pow(lv3Y, 2))
                                + (lv1X * lv3E - lv1E * lv3X)
                                        * (2 * lv3E * lv3X * pow(lv1E, 3)
                                                + lv1X * (uP + pow(lv1X, 2))
                                                        * (2 * pow(lv3E, 2)
                                                                - pow(lv3Y, 2))
                                                - 2 * lv1E * lv3E
                                                        * (lv3X * uP
                                                                + lv3X
                                                                        * pow(
                                                                                lv1X,
                                                                                2)
                                                                + lv1X
                                                                        * (-pow(
                                                                                lv3E,
                                                                                2)
                                                                                + pow(
                                                                                        lv3X,
                                                                                        2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2)))
                                                + pow(lv1E, 2)
                                                        * (lv1X
                                                                * (-2
                                                                        * pow(
                                                                                lv3E,
                                                                                2)
                                                                        + pow(
                                                                                lv3Y,
                                                                                2))
                                                                + 2 * lv3X
                                                                        * (-pow(
                                                                                lv3E,
                                                                                2)
                                                                                + pow(
                                                                                        lv3X,
                                                                                        2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2))))
                                + lv1E * lv3Y * cos(angle) * pow(2, 0.5)
                                        * sqrtFactor)) / 2.;

        lv3AY = pow(
                -4 * lv1E * lv1X * lv3E * lv3X
                        + pow(lv1X, 2) * (2 * pow(lv3E, 2) - pow(lv3Y, 2))
                        + cos(2 * angle) * (pow(lv1E, 2) - pow(lv1X, 2))
                                * pow(lv3Y, 2)
                        + pow(lv1E, 2) * (2 * pow(lv3X, 2) + pow(lv3Y, 2)), -1)
                * (-(lv1E * lv3E * lv3Y * uP * cos(angle))
                        + lv1X * lv3X * lv3Y * uP * cos(angle)
                        - lv1X * lv3X * lv3Y * cos(angle) * pow(lv1E, 2)
                        + lv3E * lv3Y * cos(angle) * pow(lv1E, 3)
                        - lv1E * lv3E * lv3Y * cos(angle) * pow(lv1X, 2)
                        + lv3X * lv3Y * cos(angle) * pow(lv1X, 3)
                        - lv3Y * cos(angle) * pow(lv1E, 2) * pow(lv3E, 2)
                        + lv3Y * cos(angle) * pow(lv1X, 2) * pow(lv3E, 2)
                        + lv3Y * cos(angle) * pow(lv1E, 2) * pow(lv3X, 2)
                        - lv3Y * cos(angle) * pow(lv1X, 2) * pow(lv3X, 2)
                        + cos(angle) * pow(lv1E, 2) * pow(lv3Y, 3)
                        - cos(angle) * pow(lv1X, 2) * pow(lv3Y, 3)
                        + pow(2, -0.5) * sqrtFactor);

        lv3BX =
                (pow(lv1X * lv3E - lv1E * lv3X, -1)
                        * pow(
                                -4 * lv1E * lv1X * lv3E * lv3X
                                        + pow(lv1X, 2)
                                                * (2 * pow(lv3E, 2)
                                                        - pow(lv3Y, 2))
                                        + cos(2 * angle)
                                                * (pow(lv1E, 2) - pow(lv1X, 2))
                                                * pow(lv3Y, 2)
                                        + pow(lv1E, 2)
                                                * (2 * pow(lv3X, 2)
                                                        + pow(lv3Y, 2)), -1)
                        * ((lv1X * lv3E - lv1E * lv3X) * cos(2 * angle)
                                * (-((lv1X - 2 * lv3X) * pow(lv1E, 2))
                                        + lv1X
                                                * (-2 * lv1X * lv3X + uP
                                                        + pow(lv1X, 2)))
                                * pow(lv3Y, 2)
                                + (-(lv1X * lv3E) + lv1E * lv3X)
                                        * (2 * lv3E * lv3X * pow(lv1E, 3)
                                                + lv1X
                                                        * (-2 * lv1X * lv3X + uP
                                                                + pow(lv1X, 2))
                                                        * (2 * pow(lv3E, 2)
                                                                - pow(lv3Y, 2))
                                                + pow(lv1E, 2)
                                                        * (-2 * lv3X
                                                                * (pow(lv3E, 2)
                                                                        + pow(
                                                                                lv3X,
                                                                                2))
                                                                + lv1X
                                                                        * (-2
                                                                                * pow(
                                                                                        lv3E,
                                                                                        2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2)))
                                                - 2 * lv1E * lv3E
                                                        * (lv3X * uP
                                                                + lv3X
                                                                        * pow(
                                                                                lv1X,
                                                                                2)
                                                                + lv1X
                                                                        * (-pow(
                                                                                lv3E,
                                                                                2)
                                                                                - 3
                                                                                        * pow(
                                                                                                lv3X,
                                                                                                2)
                                                                                + pow(
                                                                                        lv3Y,
                                                                                        2))))
                                - lv1E * lv3Y * cos(angle) * pow(2, 0.5)
                                        * sqrtFactor)) / 2.;

        lv3BY = -(cos(angle)
                * pow(
                        -4 * lv1E * lv1X * lv3E * lv3X
                                + pow(lv1X, 2)
                                        * (2 * pow(lv3E, 2) - pow(lv3Y, 2))
                                + cos(2 * angle) * (pow(lv1E, 2) - pow(lv1X, 2))
                                        * pow(lv3Y, 2)
                                + pow(lv1E, 2)
                                        * (2 * pow(lv3X, 2) + pow(lv3Y, 2)), -1)
                * (4 * lv1E * lv1X * lv3E * lv3X * lv3Y
                        - lv1E * lv3E * lv3Y * uP + lv1X * lv3X * lv3Y * uP
                        - lv1X * lv3X * lv3Y * pow(lv1E, 2)
                        + lv3E * lv3Y * pow(lv1E, 3)
                        - lv1E * lv3E * lv3Y * pow(lv1X, 2)
                        + lv3X * lv3Y * pow(lv1X, 3)
                        - lv3Y * pow(lv1E, 2) * pow(lv3E, 2)
                        - lv3Y * pow(lv1X, 2) * pow(lv3E, 2)
                        - lv3Y * pow(lv1E, 2) * pow(lv3X, 2)
                        - lv3Y * pow(lv1X, 2) * pow(lv3X, 2)
                        - pow(lv1E, 2) * pow(lv3Y, 3)
                        + pow(lv1X, 2) * pow(lv3Y, 3)
                        + 4 * lv1E * lv1X * lv3E * lv3X * lv3Y
                                * pow(tan(angle), 2)
                        - 2 * lv3Y * pow(lv1X, 2) * pow(lv3E, 2)
                                * pow(tan(angle), 2)
                        - 2 * lv3Y * pow(lv1E, 2) * pow(lv3X, 2)
                                * pow(tan(angle), 2)
                        + pow(2, -0.5) * sqrtFactor * 1 / cos(angle)));

    }

    double lv3AZ = lv3AY * sin(angle);

    lv3AZ *= ((m_randomNumberModule->diceFlat() < 0.5) ? (-1) : (1));

    double lv3BZ = -1 * lv3AZ;

    TLorentzVector lvGamma1(lv3AX, lv3AY * cos(angle), lv3AZ,
            TMath::Sqrt(
                    pow(lv3AX, 2) + pow(lv3AY * cos(angle), 2)
                            + pow(lv3AZ, 2)));
    TLorentzVector lvGamma2(lv3BX, lv3BY * cos(angle), lv3BZ,
            TMath::Sqrt(
                    pow(lv3BX, 2) + pow(lv3BY * cos(angle), 2)
                            + pow(lv3BZ, 2)));

    Particle pPrim_CMS(ParticleType::PROTON, lvPPrim);
    Particle gamma1_CMS(ParticleType::PHOTON, lvGamma1);
    Particle gamma2_CMS(ParticleType::PHOTON, lvGamma2);

    //boost to X and rotate back
    Particle pPrim_X = pPrim_CMS;
    Particle gamma1_X = gamma1_CMS;
    Particle gamma2_X = gamma2_CMS;

    pPrim_X.boost(boost_X_to_CMS);
    gamma1_X.boost(boost_X_to_CMS);
    gamma2_X.boost(boost_X_to_CMS);

    pPrim_X.rotate(AxisType::Y, acos(cosTheta_gamma_TAR));
    gamma1_X.rotate(AxisType::Y, acos(cosTheta_gamma_TAR));
    gamma2_X.rotate(AxisType::Y, acos(cosTheta_gamma_TAR));

    //return
    Particle eS_LAB = eS_TAR;
    Particle gamma_LAB = gamma_TAR;

    eS_LAB.boost(boost_LAB_to_TAR);
    gamma_LAB.boost(boost_LAB_to_TAR);

    Particle pPrim_LAB = pPrim_X;
    Particle gamma1_LAB = gamma1_X;
    Particle gamma2_LAB = gamma2_X;

    pPrim_LAB.boost(boost_LAB_to_TAR);
    gamma1_LAB.boost(boost_LAB_to_TAR);
    gamma2_LAB.boost(boost_LAB_to_TAR);

//    std::cout << solution << std::endl;
//    std::cout << "M: " << Mgg2 << ' '
//            << (gamma1_LAB.getFourMomentum() + gamma2_LAB.getFourMomentum()).Mag2()
//            << std::endl;
//    std::cout << "u': " << uP << ' '
//            << (gamma_LAB.getFourMomentum() - gamma1_LAB.getFourMomentum()).Mag2()
//            << std::endl;
//    std::cout << "t: " << t << ' '
//            << (pPrim_LAB.getFourMomentum() - p_LAB.getFourMomentum()).Mag2()
//            << std::endl;
//    std::cout << "Q2: " << Q2 << ' '
//            << -(e_LAB.getFourMomentum() - eS_LAB.getFourMomentum()).Mag2()
//            << std::endl;
//    std::cout << "y: " << y << ' '
//            << (e_LAB.getEnergy() - eS_LAB.getEnergy()) / e_LAB.getEnergy()
//            << std::endl;
//
//    std::cout << "balance: " << std::endl;
//    (e_LAB.getFourMomentum() + p_LAB.getFourMomentum()
//            - eS_LAB.getFourMomentum() - pPrim_LAB.getFourMomentum()
//            - gamma1_LAB.getFourMomentum() - gamma2_LAB.getFourMomentum()).Print();
//
//    std::cout << "PP" << gamma1_CMS.getEnergy() << " " << gamma2_CMS.getEnergy()
//            << std::endl;

    //tworzenie obiektow Particl, Vertex i kontenera Event
    Event event;

    std::vector < std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> >
            > particles(7);

    particles.at(0) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared < Particle > (e_LAB));
    particles.at(1) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared < Particle > (p_LAB));
    particles.at(2) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle > (eS_LAB));
    particles.at(3) = std::make_pair(ParticleCodeType::VIRTUAL,
            std::make_shared < Particle > (gamma_LAB));
    particles.at(4) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle > (pPrim_LAB));
    particles.at(5) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle > (gamma1_LAB));
    particles.at(6) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle > (gamma2_LAB));

    event.setParticles(particles);

    std::vector < std::shared_ptr<Vertex> > vertices(2);

    vertices.at(0) = std::make_shared<Vertex>();
    vertices.at(0)->addParticleIn(particles.at(0).second);
    vertices.at(0)->addParticleOut(particles.at(2).second);
    vertices.at(0)->addParticleOut(particles.at(3).second);

    vertices.at(1) = std::make_shared<Vertex>();
    vertices.at(1)->addParticleIn(particles.at(3).second);
    vertices.at(1)->addParticleIn(particles.at(1).second);
    vertices.at(1)->addParticleOut(particles.at(4).second);
    vertices.at(1)->addParticleOut(particles.at(5).second);
    vertices.at(1)->addParticleOut(particles.at(6).second);

    event.setVertices(vertices);

    return event;
}

} /* namespace EPIC */

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

namespace EPIC {

const unsigned int GAM2KinematicDefault::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2KinematicDefault("GAM2KinematicDefault"));

GAM2KinematicDefault::GAM2KinematicDefault(const std::string &className) :
        GAM2KinematicModule(className) {
}

GAM2KinematicDefault::GAM2KinematicDefault(const GAM2KinematicDefault &other) :
        GAM2KinematicModule(other) {
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

    double Egamma = conditions.getLeptonEnergy();
    //Mgg2 = Sqrt(Mgg^2)
    double Mgg2 =TMath::Sqrt(kin.getMgg2());
    double uP = kin.getUPrim();
    double Mp = 0.938272088;
    double angle = kin.getPhi();
    double s = 2*Egamma*Egamma +Mp*Mp + 2*Egamma*TMath::Sqrt(Egamma*Egamma+Mp*Mp);
    double t = kin.getT();
    double E2 = (s+Mp*Mp)/(2*TMath::Sqrt(s));
    double p = TMath::Sqrt((TMath::Power(Mp,4)-2*Mp*Mp*s+s*s)/(4*s));
    double pPrim = TMath::Sqrt((TMath::Power(Mgg2,4)+(Mp*Mp-s)*(Mp*Mp-s)-2*Mgg2*Mgg2*(Mp*Mp+s))/(4*s));
    double cosTheta = (TMath::Power(Mp,4)-2*Mp*Mp*s-Mgg2*Mgg2*(Mp*Mp+s)+s*(s+2*t))/TMath::Sqrt((Mp*Mp-s)*(Mp*Mp-s))/TMath::Sqrt(TMath::Power(Mgg2,4)+(Mp*Mp-s)*(Mp*Mp-s)-2*Mgg2*Mgg2*(Mp*Mp+s));
    double sinTheta = TMath::Sqrt(1-cosTheta*cosTheta);

    double p1CM = Egamma;
    double E3CM = (s+Mgg2*Mgg2-Mp*Mp)/(2*TMath::Sqrt(s));
    double p3CM = TMath::Sqrt(E3CM*E3CM - Mgg2*Mgg2);

    double t0 = pow((Mgg2*Mgg2)/(2*TMath::Sqrt(s)),2) - pow(p1CM-p3CM,2);
    double t1 = pow((Mgg2*Mgg2)/(2*TMath::Sqrt(s)),2) - pow(p1CM+p3CM,2);

    TLorentzVector lvGamma(p,0,0,Egamma);
    TLorentzVector lvP(-p,0,0,E2);
    TLorentzVector lvGamma3(pPrim*cosTheta,pPrim*sinTheta,0,E3CM);
    TVector3 boost_lvP = lvP.BoostVector();
    lvGamma.Boost(-boost_lvP);
    lvGamma3.Boost(-boost_lvP);
    double lv3X = lvGamma3[0];
    double lv3Y = lvGamma3[1];
    double lv3E = lvGamma3[3];
    double lv1X = lvGamma[0];
    double lv1E = lvGamma[3];



    if (t < t1 || t > t0)
        return false;


    if (Mgg2*Mgg2 > s + Mp*Mp)
        return false;

    if (s > pow((Mgg2-Mp),2) && s < pow((Mgg2+Mp),2))
        return false;

    if(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
            (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) - 4*pow(lv1X,3)*pow(lv3X,3) +
              2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) - 4*lv3X*pow(lv1X,3)*pow(lv3Y,2) + 2*pow(lv1X,4)*pow(lv3Y,2) -
              pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) - 4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) +
              2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) + 4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) +
              cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
              2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) + pow(lv1X,2)*pow(lv3Y,4) +
              2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2)< 0)
        return false;

    /*\
     *
     *
     * Tutaj musimy zaimplementowac kod sluzacy sprawdzeniu czy dane warunki wejsciowe (kinematyka) ma sens fizyczny.
     * Mozemy to zrobic albo w trakcie pisania funkcji evaluate() albo pozniej
     */

    return true;
}

Event GAM2KinematicDefault::evaluate(const ExperimentalConditions &conditions,
        const GAM2Kinematic &kin) {

    std::cout << "dane wejsciowe:" << std::endl;

    std::cout << conditions.toString() << std::endl;
    std::cout << kin.toString() << std::endl;

    //z obiektu ExperimentalConditions bedziemy na razie potrzebowali tylko energie wiazki (prosze zignoro-wac "lepton" w nazwie funkcji)
    double Egamma = conditions.getLeptonEnergy();

    //proton stoi w miejscu, czyli jego energia to masa spoczynkowa (pracujemy w GeV-ach) *obecnie proton porusza sie wzdluz osi x*
    double Mp = 0.938272088;


    double s = 2*Egamma*Egamma +Mp*Mp + 2*Egamma*TMath::Sqrt(Egamma*Egamma+Mp*Mp);
    double t = kin.getT();
    double uPrim = kin.getUPrim();
    //Mgg2 = Sqrt(Mgg^2)
    double Mgg2 = TMath::Sqrt(kin.getMgg2());
    double E1 = Egamma;
    double E2 = (s+Mp*Mp)/(2*TMath::Sqrt(s));
    double E3 = (s+Mgg2*Mgg2-Mp*Mp)/(2*TMath::Sqrt(s));
    double E4 = (s-Mgg2*Mgg2+Mp*Mp)/(2*TMath::Sqrt(s));
    double p = TMath::Sqrt((TMath::Power(Mp,4)-2*Mp*Mp*s+s*s)/(4*s));
    double pPrim = TMath::Sqrt((TMath::Power(Mgg2,4)+(Mp*Mp-s)*(Mp*Mp-s)-2*Mgg2*Mgg2*(Mp*Mp+s))/(4*s));
    double cosTheta = (TMath::Power(Mp,4)-2*Mp*Mp*s-Mgg2*Mgg2*(Mp*Mp+s)+s*(s+2*t))/TMath::Sqrt((Mp*Mp-s)*(Mp*Mp-s))/TMath::Sqrt(TMath::Power(Mgg2,4)+(Mp*Mp-s)*(Mp*Mp-s)-2*Mgg2*Mgg2*(Mp*Mp+s));
    double sinTheta = TMath::Sqrt(1-cosTheta*cosTheta);
    //naszym celem jest zapelnienie tych 4-wektorow
    TLorentzVector lvGamma(p,0,0,E1);
    TLorentzVector lvP(-p,0,0,E2);

    TVector3 boost_lvP = lvP.BoostVector();


    lvP.Boost(-boost_lvP);
    lvGamma.Boost(-boost_lvP);

    TLorentzVector lvGamma3(pPrim*cosTheta,pPrim*sinTheta,0,E3);
    TLorentzVector lvPPrim(-pPrim*cosTheta,-pPrim*sinTheta,0,E4);
    lvGamma3.Boost(-boost_lvP);
    lvPPrim.Boost(-boost_lvP);

    double lv3X = lvGamma3[0];
    double lv3Y = lvGamma3[1];
    double lv3E = lvGamma3[3];
    double lv1X = lvGamma[0];
    double lv1E = lvGamma[3];
    double uP = uPrim;
    double angle = kin.getPhi();

    //results 1
    double lv3AX = pow(cos(angle),2)*pow(-4*lv1E*lv1X*lv3E*lv3X*(pow(lv3X,2) + pow(lv3Y,2)) -
            cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
            pow(lv1X,2)*(-(pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))) + pow(lv3E,2)*(2*pow(lv3X,2) + pow(lv3Y,2))) + 2*pow(lv1E,2)*pow(pow(lv3X,2) + pow(lv3Y,2),2),-1)*
          (lv1X*uP*pow(lv3E,2)*pow(lv3X,2) + pow(lv1X,3)*pow(lv3E,2)*pow(lv3X,2) + lv1X*uP*pow(lv3E,2)*pow(lv3Y,2) + pow(lv1X,3)*pow(lv3E,2)*pow(lv3Y,2) -
            lv1X*uP*pow(lv3X,2)*pow(lv3Y,2) - pow(lv1X,3)*pow(lv3X,2)*pow(lv3Y,2) + lv3E*lv3X*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) -
            lv1E*lv3E*(pow(lv3X,2) + pow(lv3Y,2))*(lv3X*uP + lv3X*pow(lv1X,2) + lv1X*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))) +
            pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(lv1X*(-pow(lv3E,2) + pow(lv3Y,2)) + lv3X*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))) - lv1X*uP*pow(lv3Y,4) -
            pow(lv1X,3)*pow(lv3Y,4) - pow(2,-0.5)*pow(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
               (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) - 4*pow(lv1X,3)*pow(lv3X,3) +
                 2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) - 4*lv3X*pow(lv1X,3)*pow(lv3Y,2) + 2*pow(lv1X,4)*pow(lv3Y,2) -
                 pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) - 4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) +
                 2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) + 4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) +
                 cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
                 2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) + pow(lv1X,2)*pow(lv3Y,4) +
                 2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2),0.5) +
            lv3X*(-(lv1X*lv3E*lv3X) + lv1E*(pow(lv3X,2) + pow(lv3Y,2)))*(lv3E*pow(lv1E,2) - lv3E*(uP + pow(lv1X,2)) + lv1E*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)))*
             pow(tan(angle),2));

    double lv3AY = -(pow(lv1E,-1)*pow(lv3Y,-1)*pow(cos(angle),2)*pow(4*lv1E*lv1X*lv3E*lv3X*(pow(lv3X,2) + pow(lv3Y,2)) +
            cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
            pow(lv1X,2)*(pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) - pow(lv3E,2)*(2*pow(lv3X,2) + pow(lv3Y,2))) - 2*pow(lv1E,2)*pow(pow(lv3X,2) + pow(lv3Y,2),2),-1)*
          (lv1E*lv1X*lv3X*uP*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) + lv3E*pow(lv1E,4)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) -
            lv3E*pow(lv1E,2)*(uP + pow(lv1X,2))*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) + lv1E*lv3X*pow(lv1X,3)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) -
            lv1E*pow(lv1X,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
            pow(lv1E,3)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-(lv1X*lv3X) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
            lv1X*lv3E*pow(2,-0.5)*pow(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
               (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) -
                 4*pow(lv1X,3)*pow(lv3X,3) + 2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) - 4*lv3X*pow(lv1X,3)*pow(lv3Y,2) +
                 2*pow(lv1X,4)*pow(lv3Y,2) - pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) -
                 4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) + 2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) +
                 4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) + cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
                 2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
                 pow(lv1X,2)*pow(lv3Y,4) + 2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2),0.5) +
            lv1E*lv3X*pow(2,-0.5)*pow(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
               (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) -
                 4*pow(lv1X,3)*pow(lv3X,3) + 2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) - 4*lv3X*pow(lv1X,3)*pow(lv3Y,2) +
                 2*pow(lv1X,4)*pow(lv3Y,2) - pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) -
                 4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) + 2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) +
                 4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) + cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
                 2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
                 pow(lv1X,2)*pow(lv3Y,4) + 2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2),0.5) +
            lv1E*pow(lv3Y,2)*(-(lv1X*lv3E*lv3X) + lv1E*(pow(lv3X,2) + pow(lv3Y,2)))*
             (lv3E*pow(lv1E,2) - lv3E*(uP + pow(lv1X,2)) + lv1E*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)))*pow(tan(angle),2)));

    double lv3BX = pow(cos(angle),2)*pow(-4*lv1E*lv1X*lv3E*lv3X*(pow(lv3X,2) + pow(lv3Y,2)) -
            cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
            pow(lv1X,2)*(-(pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))) + pow(lv3E,2)*(2*pow(lv3X,2) + pow(lv3Y,2))) + 2*pow(lv1E,2)*pow(pow(lv3X,2) + pow(lv3Y,2),2),-1)*
          (-(lv1X*uP*pow(lv3E,2)*pow(lv3X,2)) - pow(lv1X,3)*pow(lv3E,2)*pow(lv3X,2) + 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,3) - lv1X*uP*pow(lv3E,2)*pow(lv3Y,2) +
            2*lv3X*pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) - pow(lv1X,3)*pow(lv3E,2)*pow(lv3Y,2) + lv1X*uP*pow(lv3X,2)*pow(lv3Y,2) + pow(lv1X,3)*pow(lv3X,2)*pow(lv3Y,2) -
            2*pow(lv1X,2)*pow(lv3X,3)*pow(lv3Y,2) - lv3E*lv3X*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) +
            lv1E*lv3E*(pow(lv3X,2) + pow(lv3Y,2))*(lv3X*uP + lv3X*pow(lv1X,2) + lv1X*(-pow(lv3E,2) - 3*pow(lv3X,2) + pow(lv3Y,2))) +
            pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(lv1X*(pow(lv3E,2) - pow(lv3Y,2)) + lv3X*(pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))) + lv1X*uP*pow(lv3Y,4) -
            2*lv3X*pow(lv1X,2)*pow(lv3Y,4) + pow(lv1X,3)*pow(lv3Y,4) +
            pow(2,-0.5)*pow(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
               (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) - 4*pow(lv1X,3)*pow(lv3X,3) +
                 2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) - 4*lv3X*pow(lv1X,3)*pow(lv3Y,2) + 2*pow(lv1X,4)*pow(lv3Y,2) -
                 pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) - 4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) +
                 2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) + 4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) +
                 cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
                 2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) + pow(lv1X,2)*pow(lv3Y,4) +
                 2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2),0.5) +
            lv3X*(-(lv1X*lv3E*lv3X) + lv1E*(pow(lv3X,2) + pow(lv3Y,2)))*
             (-(lv3E*pow(lv1E,2)) + lv3E*(-2*lv1X*lv3X + uP + pow(lv1X,2)) + lv1E*(pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)))*pow(tan(angle),2));

    double lv3BY = -0.5*(pow(lv1E,-1)*pow(lv3Y,-1)*pow(cos(angle),2)*pow(-4*lv1E*lv1X*lv3E*lv3X*(pow(lv3X,2) + pow(lv3Y,2)) -
            cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
            pow(lv1X,2)*(-(pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))) + pow(lv3E,2)*(2*pow(lv3X,2) + pow(lv3Y,2))) + 2*pow(lv1E,2)*pow(pow(lv3X,2) + pow(lv3Y,2),2),-1)*
          (2*lv3E*pow(lv1E,4)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) -
            2*lv3E*pow(lv1E,2)*(-4*lv1X*lv3X + uP + pow(lv1X,2))*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) -
            2*pow(lv1E,3)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(lv1X*lv3X + pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
            lv1X*lv3E*pow(2,0.5)*pow(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
               (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) -
                 4*pow(lv1X,3)*pow(lv3X,3) + 2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) - 4*lv3X*pow(lv1X,3)*pow(lv3Y,2) +
                 2*pow(lv1X,4)*pow(lv3Y,2) - pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) -
                 4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) + 2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) +
                 4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) + cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
                 2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
                 pow(lv1X,2)*pow(lv3Y,4) + 2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2),0.5) +
            lv1E*(2*lv1X*lv3X*uP*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) + 2*lv3X*pow(lv1X,3)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) -
               2*pow(lv1X,2)*(pow(lv3E,2) + pow(lv3X,2) - pow(lv3Y,2))*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2)) +
               lv3X*pow(2,0.5)*pow(pow(lv1E,2)*pow(lv3Y,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2))*
                  (4*uP*pow(lv1X,2)*pow(lv3X,2) + 2*pow(lv1X,4)*pow(lv3X,2) - 2*pow(lv1X,2)*pow(lv3E,2)*pow(lv3X,2) - 4*lv1X*uP*pow(lv3X,3) -
                    4*pow(lv1X,3)*pow(lv3X,3) + 2*pow(lv1X,2)*pow(lv3X,4) - 4*lv1X*lv3X*uP*pow(lv3Y,2) + 4*uP*pow(lv1X,2)*pow(lv3Y,2) -
                    4*lv3X*pow(lv1X,3)*pow(lv3Y,2) + 2*pow(lv1X,4)*pow(lv3Y,2) - pow(lv1X,2)*pow(lv3E,2)*pow(lv3Y,2) + 3*pow(lv1X,2)*pow(lv3X,2)*pow(lv3Y,2) -
                    4*lv3E*pow(lv1E,3)*(pow(lv3X,2) + pow(lv3Y,2)) + 2*pow(lv1E,4)*(pow(lv3X,2) + pow(lv3Y,2)) +
                    4*lv1E*lv3E*(uP + pow(lv1X,2))*(pow(lv3X,2) + pow(lv3Y,2)) + cos(2*angle)*pow(lv1X,2)*pow(lv3Y,2)*(-pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) -
                    2*pow(lv1E,2)*(pow(lv3X,2) + pow(lv3Y,2))*(-2*lv1X*lv3X + 2*uP + 2*pow(lv1X,2) - pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)) +
                    pow(lv1X,2)*pow(lv3Y,4) + 2*pow(lv3X,2)*pow(uP,2) + 2*pow(lv3Y,2)*pow(uP,2))*pow(1/cos(angle),2),0.5)) +
            2*lv1E*pow(lv3Y,2)*(-(lv1X*lv3E*lv3X) + lv1E*(pow(lv3X,2) + pow(lv3Y,2)))*
             (lv3E*pow(lv1E,2) - lv3E*(-2*lv1X*lv3X + uP + pow(lv1X,2)) - lv1E*(pow(lv3E,2) + pow(lv3X,2) + pow(lv3Y,2)))*pow(tan(angle),2)));

    double lv3AZ = (-(lv3AY*lv3X) + lv3AX*lv3Y)*pow(pow(lv3X,2) + pow(lv3Y,2),-0.5)*tan(angle);

    double lv3BZ = -1 * lv3AZ;

    TLorentzVector lvGamma1(lv3AX,lv3AY,lv3AZ,TMath::Sqrt(lv3AX*lv3AX+lv3AY*lv3AY));
    TLorentzVector lvGamma2(lv3BX,lv3BY,lv3BZ,TMath::Sqrt(lv3BX*lv3BX+lv3BY*lv3BY));


    //return

    //tworzenie obiektow Particl, Vertex i kontenera Event
    Event event;

    std::vector < std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> >
            > particles(5);

    particles.at(0) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared < Particle
                    > (Particle(ParticleType::PHOTON, lvGamma)));
    particles.at(1) = std::make_pair(ParticleCodeType::BEAM,
            std::make_shared < Particle
                    > (Particle(ParticleType::PROTON, lvP)));
    particles.at(2) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle
                    > (Particle(ParticleType::PHOTON, lvGamma1)));
    particles.at(3) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle
                    > (Particle(ParticleType::PHOTON, lvGamma2)));
    particles.at(4) = std::make_pair(ParticleCodeType::UNDECAYED,
            std::make_shared < Particle
                    > (Particle(ParticleType::PROTON, lvPPrim)));

    event.setParticles(particles);

    std::vector < std::shared_ptr<Vertex> > vertices(1);

    vertices.at(0) = std::make_shared<Vertex>();
    vertices.at(0)->addParticleIn(particles.at(0).second);
    vertices.at(0)->addParticleIn(particles.at(1).second);
    vertices.at(0)->addParticleOut(particles.at(2).second);
    vertices.at(0)->addParticleOut(particles.at(3).second);
    vertices.at(0)->addParticleOut(particles.at(4).second);

    event.setVertices(vertices);

    return event;
}

} /* namespace EPIC */

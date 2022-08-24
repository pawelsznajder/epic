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

    /*
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

    //z obiektu ExperimentalConditions bedziemy na razie potrzebowali tylko energie wiazki (prosze zignorowac "lepton" w nazwie funkcji)
    double Egamma = conditions.getLeptonEnergy();

    //proton stoi w miejscu, czyli jego energia to masa spoczynkowa (pracujemy w GeV-ach)
    double Ep = 0.938272088;

    //naszym celem jest zapelnienie tych 4-wektorow
    TLorentzVector lvGamma;
    TLorentzVector lvP;
    TLorentzVector lvGamma1;
    TLorentzVector lvGamma2;
    TLorentzVector lvPPrim;

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

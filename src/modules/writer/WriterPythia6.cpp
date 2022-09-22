/*
 * WriterPythia6.cpp
 *
 *  Created on: Feb 11, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../include/modules/writer/WriterPythia6.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <stddef.h>
#include <TLorentzVector.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <utility>

#include "../../../include/beans/containers/GenerationInformation.h"
#include "../../../include/beans/physics/Particle.h"
#include "../../../include/beans/physics/Vertex.h"
#include "../../../include/beans/types/EventAttributeType.h"
#include "../../../include/beans/types/ParticleCodeType.h"
#include "../../../include/beans/types/ParticleType.h"

namespace EPIC {

const unsigned int WriterPythia6::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new WriterPythia6("WriterPythia6"));

WriterPythia6::WriterPythia6(const std::string &className) :
        WriterModule(className) {

    m_trailSign << std::scientific;

    m_writerPythia6NoSeparator = new WriterPythia6NoSeparator();
}

WriterPythia6::WriterPythia6(const WriterPythia6 &other) :
        WriterModule(other) {

    m_trailSign << std::scientific;

    m_writerPythia6NoSeparator = new WriterPythia6NoSeparator();
}

WriterPythia6::~WriterPythia6() {

    if (m_writerPythia6NoSeparator) {

        delete m_writerPythia6NoSeparator;
        m_writerPythia6NoSeparator = nullptr;
    }
}

WriterPythia6 *WriterPythia6::clone() const {
    return new WriterPythia6(*this);
}

void WriterPythia6::configure(const ElemUtils::Parameters &parameters) {

    // run for mother
    WriterModule::configure(parameters);
}

void WriterPythia6::open() {

    // check path
    if (m_path.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Path not set");
    }

    // close if was open
    if (m_ofstream.is_open()) {
        close();
    }

    //open
    m_ofstream.open(m_path);

    m_ofstream.imbue(std::locale(std::locale(), m_writerPythia6NoSeparator));

    //headers
    m_ofstream << "PYTHIA EVENT FILE" << std::endl;
    m_ofstream << "============================================" << std::endl;
    m_ofstream
            << "I, ievent, genevent, subprocess, nucleon,                 targetparton, xtargparton, beamparton, xbeamparton,               thetabeamprtn, truey, trueQ2, truex, trueW2, trueNu, leptonphi,   s_hat, t_hat, u_hat, pt2_hat, Q2_hat, F2, F1, R, sigma_rad,       SigRadCor, EBrems, photonflux, t-diff, nrTracks"
            << std::endl;
    m_ofstream << "============================================" << std::endl;
    m_ofstream
            << "I  K(I,1)  K(I,2)  K(I,3)  K(I,4)  K(I,5)  P(I,1)  P(I,2)  P(I,3)  P(I,4)  P(I,5)  V(I,1)  V(I,2)  V(I,3)"
            << std::endl;
    m_ofstream << "============================================" << std::endl;
}

void WriterPythia6::saveGenerationInformation(
        const GenerationInformation& generationInformation) {

    //open file
    std::ofstream ofstreamGeneralInfo;

    //open
    ofstreamGeneralInfo.open(m_path + ".log");

    ofstreamGeneralInfo.imbue(
            std::locale(std::locale(), m_writerPythia6NoSeparator));

    if (!ofstreamGeneralInfo.is_open()) {

        warn(__func__, "Pythia6 writer for general information is not open");
        return;
    }

    //save info
    ofstreamGeneralInfo << "generator_name: "
            << generationInformation.getGeneratorName() << std::endl;
    ofstreamGeneralInfo << "generator_version: "
            << generationInformation.getGeneratorVersion() << std::endl;
    ofstreamGeneralInfo << "scenario_description: "
            << generationInformation.getDescription() << std::endl;

    ofstreamGeneralInfo << "service_name: "
            << generationInformation.getServiceName() << std::endl;
    ofstreamGeneralInfo << "generated_events_number: "
            << generationInformation.getNEvents() << std::endl;
    ofstreamGeneralInfo << "integrated_cross_section_value: "
            << generationInformation.getIntegratedCrossSection().first
            << std::endl;
    ofstreamGeneralInfo << "integrated_cross_section_uncertainty: "
            << generationInformation.getIntegratedCrossSection().second
            << std::endl;
    ofstreamGeneralInfo << "generation_date: "
            << generationInformation.getGenerationDate() << std::endl;

    for (std::vector<std::pair<std::string, std::string> >::const_iterator it =
            generationInformation.getAdditionalInfo().begin();
            it != generationInformation.getAdditionalInfo().end(); it++) {
        ofstreamGeneralInfo << it->first << ": " << it->second << '\n';
    }

    //close
    ofstreamGeneralInfo.close();
}

void WriterPythia6::close() {

    if (!m_ofstream.is_open()) {

        warn(__func__, "Pythia6 writer is not open");
        return;
    }

    m_ofstream.close();
}

void WriterPythia6::write(const Event &event) {

    // check if open
    if (!m_ofstream.is_open()) {

        warn(__func__, "Pythia6 writer is not open");
        return;
    }

    // references
    const std::vector<
            std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >& eventParticles =
            event.getParticles();
    const std::vector<std::shared_ptr<Vertex> >& eventVertices =
            event.getVertices();
    const std::map<EventAttributeType::Type, double>& eventAttributes =
            event.getAttributes();

    // sort particles such as on top we have beam, scattered lepton and virtual photon
    std::vector < std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> >
            > eventParticlesSorted = eventParticles;

    for (std::vector<
            std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >::iterator itP =
            eventParticlesSorted.begin(); itP != eventParticlesSorted.end();
            itP++) {

        //beam lepton goes first
        if (itP->first == ParticleCodeType::BEAM
                && (itP->second->getType() == ParticleType::ELECTRON
                        || itP->second->getType() == ParticleType::POSITRON
                        || itP->second->getType() == ParticleType::MUON_MINUS
                        || itP->second->getType() == ParticleType::MUON_PLUS
                        || itP->second->getType() == ParticleType::TAU_MINUS
                        || itP->second->getType() == ParticleType::TAU_PLUS)) {
            if (itP != eventParticlesSorted.begin() + 0) {
                std::iter_swap(itP, eventParticlesSorted.begin() + 0);
            }
        }

        //then, hadron beam
        if (itP->first == ParticleCodeType::BEAM
                && !(itP->second->getType() == ParticleType::ELECTRON
                        || itP->second->getType() == ParticleType::POSITRON
                        || itP->second->getType() == ParticleType::MUON_MINUS
                        || itP->second->getType() == ParticleType::MUON_PLUS
                        || itP->second->getType() == ParticleType::TAU_MINUS
                        || itP->second->getType() == ParticleType::TAU_PLUS)) {
            if (itP != eventParticlesSorted.begin() + 1) {
                std::iter_swap(itP, eventParticlesSorted.begin() + 1);
            }
        }

        //scattered electron
        if (itP->first == ParticleCodeType::SCATTERED
                && (itP->second->getType() == ParticleType::ELECTRON
                        || itP->second->getType() == ParticleType::POSITRON
                        || itP->second->getType() == ParticleType::MUON_MINUS
                        || itP->second->getType() == ParticleType::MUON_PLUS
                        || itP->second->getType() == ParticleType::TAU_MINUS
                        || itP->second->getType() == ParticleType::TAU_PLUS)) {
            if (itP != eventParticlesSorted.begin() + 2) {
                std::iter_swap(itP, eventParticlesSorted.begin() + 2);
            }
        }

        //finally, virtual photon
        if (itP->first == ParticleCodeType::VIRTUAL) {
            if (itP != eventParticlesSorted.begin() + 3) {
                std::iter_swap(itP, eventParticlesSorted.begin() + 3);
            }
        }

    }

    //first lines
    m_ofstream << "0         " << std::fixed
            << size_t(eventAttributes.find(EventAttributeType::ID)->second)
            << "         0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0"
            << std::endl;
    m_ofstream << "============================================" << std::endl;

    //string stream to double info about scattered lepton
    std::stringstream ss;

    // particles
    for (std::vector<
            std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >::const_iterator itP =
            eventParticlesSorted.begin(); itP != eventParticlesSorted.end();
            itP++) {

        size_t position = size_t(itP - eventParticlesSorted.begin()) + 1;
        size_t code = getParticleCode(itP->first);
        int pid = int(itP->second->getType());

        //get pointers to parents and daughters and check if scattered electron
        std::vector < std::shared_ptr<Particle> > parents;
        std::vector < std::shared_ptr<Particle> > daughters;

        for (std::vector<std::shared_ptr<Vertex> >::const_iterator itV =
                eventVertices.begin(); itV != eventVertices.end(); itV++) {

            for (std::vector<std::shared_ptr<Particle> >::const_iterator itPP =
                    (*itV)->getParticlesIn().begin();
                    itPP != (*itV)->getParticlesIn().end(); itPP++) {
                if ((*itPP) == (itP->second)) {
                    for (std::vector<std::shared_ptr<Particle> >::const_iterator itPPP =
                            (*itV)->getParticlesOut().begin();
                            itPPP != (*itV)->getParticlesOut().end(); itPPP++) {
                        daughters.push_back(*itPPP);
                    }
                }
            }

            for (std::vector<std::shared_ptr<Particle> >::const_iterator itPP =
                    (*itV)->getParticlesOut().begin();
                    itPP != (*itV)->getParticlesOut().end(); itPP++) {
                if ((*itPP) == (itP->second)) {
                    for (std::vector<std::shared_ptr<Particle> >::const_iterator itPPP =
                            (*itV)->getParticlesIn().begin();
                            itPPP != (*itV)->getParticlesIn().end(); itPPP++) {

                        parents.push_back(*itPPP);
                    }
                }
            }
        }

        //translate into positions
        std::vector < size_t > parentsInt(parents.size(), -1);
        std::vector < size_t > daughtersInt(daughters.size(), -1);

        for (std::vector<std::shared_ptr<Particle> >::const_iterator itPP =
                parents.begin(); itPP != parents.end(); itPP++) {

            int i = -1;

            for (std::vector<
                    std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >::const_iterator itPPP =
                    eventParticlesSorted.begin();
                    itPPP != eventParticlesSorted.end(); itPPP++) {
                if ((*itPP) == (itPPP->second)) {

                    i = int(itPPP - eventParticlesSorted.begin());
                    break;
                }
            }

            parentsInt.at(size_t(itPP - parents.begin())) = i;
        }

        for (std::vector<std::shared_ptr<Particle> >::const_iterator itPP =
                daughters.begin(); itPP != daughters.end(); itPP++) {

            int i = -1;

            for (std::vector<
                    std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >::const_iterator itPPP =
                    eventParticlesSorted.begin();
                    itPPP != eventParticlesSorted.end(); itPPP++) {
                if ((*itPP) == (itPPP->second)) {

                    i = int(itPPP - eventParticlesSorted.begin());
                    break;
                }
            }

            daughtersInt.at(size_t(itPP - daughters.begin())) = i;
        }

        //sort
        std::sort(parentsInt.begin(), parentsInt.end());
        std::sort(daughtersInt.begin(), daughtersInt.end());

        //data
        size_t positionFirstParent = 0;

        if (parentsInt.size() != 0)
            positionFirstParent = parentsInt.at(0) + 1;

        size_t positionFirstDaughter = 0;

        if (daughtersInt.size() != 0)
            positionFirstDaughter = daughtersInt.at(0) + 1;

        size_t positionLastDaughter = 0;

        if (daughtersInt.size() > 1)
            positionLastDaughter = daughtersInt.at(daughtersInt.size() - 1) + 1;

        double pX = itP->second->getFourMomentum().Px();
        double pY = itP->second->getFourMomentum().Py();
        double pZ = itP->second->getFourMomentum().Pz();
        double E = itP->second->getFourMomentum().E();
        double M = itP->second->getMass();

        //check if scattered lepton
        bool isScatteredLepton = false;

        if (itP->first == ParticleCodeType::SCATTERED
                && (itP->second->getType() == ParticleType::ELECTRON
                        || itP->second->getType() == ParticleType::POSITRON
                        || itP->second->getType() == ParticleType::MUON_MINUS
                        || itP->second->getType() == ParticleType::MUON_PLUS
                        || itP->second->getType() == ParticleType::TAU_MINUS
                        || itP->second->getType() == ParticleType::TAU_PLUS)) {
            isScatteredLepton = true;
        }

        //print scattered lepton twice
        if (isScatteredLepton) {

            ss << std::fixed << eventParticlesSorted.size() + 1 << "\t" << code
                    << "\t" << pid << "\t" << position << "\t"
                    << positionFirstDaughter << "\t" << positionLastDaughter
                    << "\t" << trailSign(pX) << "\t" << trailSign(pY) << "\t"
                    << trailSign(pZ) << "\t" << trailSign(E) << "\t"
                    << trailSign(M) << "\t" << "0. 0. 0." << std::endl;

            code = 21;
        }

        //print
        m_ofstream << std::fixed << position << "\t" << code << "\t" << pid
                << "\t" << positionFirstParent << "\t" << positionFirstDaughter
                << "\t" << positionLastDaughter << "\t" << trailSign(pX) << "\t"
                << trailSign(pY) << "\t" << trailSign(pZ) << "\t"
                << trailSign(E) << "\t" << trailSign(M) << "\t" << "0. 0. 0."
                << std::endl;
    }

    //scattered lepton
    m_ofstream << ss.str() << std::endl;

    //print
    m_ofstream << "=============== Event finished ===============" << std::endl;
}

void WriterPythia6::write(const std::vector<Event> &events) {

    for (std::vector<Event>::const_iterator it = events.begin();
            it != events.end(); it++) {
        write(*it);
    }
}

int WriterPythia6::getParticleCode(ParticleCodeType::Type type) const {

    switch (type) {

    case ParticleCodeType::UNDECAYED:
        return 1;
        break;
    case ParticleCodeType::DECAYED:
        return 21;
        break;
    case ParticleCodeType::DOCUMENTATION:
        return 21;
        break;
    case ParticleCodeType::BEAM:
        return 21;
        break;
    case ParticleCodeType::SCATTERED:
        return 1;
        break;
    case ParticleCodeType::VIRTUAL:
        return 21;
        break;

    default:
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Conversion undefined for type: "
                        << ParticleCodeType(type).toString());
    }
}

std::string WriterPythia6::trailSign(double v) {

    //clear
    m_trailSign.str("");
    m_trailSign.clear();

    //check
    if (v < 0.) {
        m_trailSign << v;
    } else {
        m_trailSign << ' ' << v;
    }

    //return
    return m_trailSign.str();
}

} /* namespace EPIC */

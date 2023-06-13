/*
 * ExperimentalConditions.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../include/beans/containers/ExperimentalConditions.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <cmath>
#include <vector>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"

namespace EPIC {

const std::string ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_ENERGY =
        "lepton_energy";
const std::string ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_PARTICLE =
        "lepton_type";
const std::string ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_HELICITY =
        "lepton_helicity";
const std::string ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_ENERGY =
        "hadron_energy";
const std::string ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_PARTICLE =
        "hadron_type";
const std::string ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_POLARISATION =
        "hadron_polarisation";

ExperimentalConditions::ExperimentalConditions() :
        PARTONS::BaseObject("ExperimentalConditions"), m_leptonEnergy(0.), m_leptonType(
                ParticleType::UNDEFINED), m_leptonHelicity(0), m_hadronEnergy(
                0.), m_hadronType(ParticleType::UNDEFINED), m_hadronPolarisation(
                NumA::Vector3D(0., 0., 0.)) {
}

ExperimentalConditions::ExperimentalConditions(double leptonEnergy,
        ParticleType::Type leptonType, int leptonHelicity, double hadronEnergy,
        ParticleType::Type hadronType, const NumA::Vector3D &hadronPolarisation) :
        PARTONS::BaseObject("ExperimentalConditions"), m_leptonEnergy(
                leptonEnergy), m_leptonType(leptonType), m_leptonHelicity(
                leptonHelicity), m_hadronEnergy(hadronEnergy), m_hadronType(
                hadronType), m_hadronPolarisation(hadronPolarisation) {
}

ExperimentalConditions::ExperimentalConditions(
        const ExperimentalConditions &other) :
        PARTONS::BaseObject(other), m_leptonEnergy(other.m_leptonEnergy), m_leptonType(
                other.m_leptonType), m_leptonHelicity(other.m_leptonHelicity), m_hadronEnergy(
                other.m_hadronEnergy), m_hadronType(other.m_hadronType), m_hadronPolarisation(
                other.m_hadronPolarisation) {
}

ExperimentalConditions::~ExperimentalConditions() {
}

std::string ExperimentalConditions::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "Experimental condition lepton energy: " << m_leptonEnergy
            << " [GeV]\n";
    formatter << "Experimental condition lepton type: "
            << ParticleType(m_leptonType).toString() << '\n';
    formatter << "Experimental condition lepton helicity: " << m_leptonHelicity
            << '\n';

    formatter << "Experimental condition hadron energy: " << m_hadronEnergy
            << " [GeV]\n";
    formatter << "Experimental condition hadron type: "
            << ParticleType(m_hadronType).toString() << '\n';
    formatter << "Experimental condition hadron polarisation: "
            << m_hadronPolarisation.toString();

    formatter << "Experimental condition fixed target lepton energy equivalent: "
            << getLeptonEnergyFixedTargetEquivalent() << " [GeV]\n";

    return formatter.str();
}

ExperimentalConditions ExperimentalConditions::fromTask(
        const MonteCarloTask &task) {

    // result
    ExperimentalConditions result;

    // data
    const ElemUtils::Parameters &data =
            task.getExperimentalConditions().getParameters();

    // lepton energy
    result.setLeptonEnergy(
            ContainerUtils::findAndParseDouble("ExperimentalConditions", data,
                    ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_ENERGY));

    // lepton type
    if (data.isAvailable(
            ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_PARTICLE)) {

        std::string valueString = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(valueString);

        result.setLeptonType(ParticleType::fromString(valueString));
    } else {
        throw ElemUtils::CustomException("ExperimentalConditions", __func__,
                ElemUtils::Formatter()
                        << "Experimental conditions missing for key "
                        << ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_PARTICLE);
    }

    // lepton helicity
    result.setLeptonHelicity(
            ContainerUtils::findAndParseInt("ExperimentalConditions", data,
                    ExperimentalConditions::EXPERIMENTAL_CONDITION_LEPTON_HELICITY));

    // hadron energy
    result.setHadronEnergy(
            ContainerUtils::findAndParseDouble("ExperimentalConditions", data,
                    ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_ENERGY, true));

    // hadron type
    if (data.isAvailable(
            ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_PARTICLE)) {

        std::string valueString = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(valueString);

        result.setHadronType(ParticleType::fromString(valueString));
    } else {
        throw ElemUtils::CustomException("ExperimentalConditions", __func__,
                ElemUtils::Formatter()
                        << "Experimental conditions missing for key "
                        << ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_PARTICLE);
    }

    // hadron polarisation
    std::vector<double> hadronPolarisation =
            ContainerUtils::findAndParseVectorDouble("ExperimentalConditions",
                    data,
                    ExperimentalConditions::EXPERIMENTAL_CONDITION_HADRON_POLARISATION,
                    3);

    result.setHadronPolarisation(
            NumA::Vector3D(hadronPolarisation.at(0), hadronPolarisation.at(1),
                    hadronPolarisation.at(2)));

    return result;
}

double ExperimentalConditions::getLeptonEnergyFixedTargetEquivalent() const {

    static bool suppressWarning[2] = {false, false};  

    double E1 = m_leptonEnergy;
    double E2 = m_hadronEnergy;

    double m1 = ParticleType(m_leptonType).getMass();
    double m2 = ParticleType(m_hadronType).getMass();

    double p1, p2;
   
    if(fabs(1. - m1/E1) < 1.E-6){

        if(!suppressWarning[0]){

            warn(__func__, "Lepton beam energy close to its mass (within 1.E-6 tollerance), setting lepton beam momentum to zero");
            suppressWarning[0] = true;
        }

        p1 = 0.;
    }else{
        p1 = sqrt(pow(E1, 2) - pow(m1, 2));
    }
 
    if(fabs(1. - m2/E2) < 1.E-6){

        if(!suppressWarning[1]){

            warn(__func__, "Hadron beam energy close to its mass (within 1.E-6 tollerance), setting hadron beam momentum to zero");
            suppressWarning[1] = true;
        }

        p2 = 0.;
    }else{
        p2 = sqrt(pow(E2, 2) - pow(m2, 2));
    }

    return (E1 * E2 + p1 * p2) / m2;
}

double ExperimentalConditions::getLeptonEnergy() const {
    return m_leptonEnergy;
}

void ExperimentalConditions::setLeptonEnergy(double leptonEnergy) {
    m_leptonEnergy = leptonEnergy;
}

ParticleType::Type ExperimentalConditions::getLeptonType() const {
    return m_leptonType;
}

void ExperimentalConditions::setLeptonType(ParticleType::Type leptonType) {
    m_leptonType = leptonType;
}

int ExperimentalConditions::getLeptonHelicity() const {
    return m_leptonHelicity;
}

void ExperimentalConditions::setLeptonHelicity(int leptonHelicity) {
    m_leptonHelicity = leptonHelicity;
}

double ExperimentalConditions::getHadronEnergy() const {
    return m_hadronEnergy;
}

void ExperimentalConditions::setHadronEnergy(double hadronEnergy) {
    m_hadronEnergy = hadronEnergy;
}

ParticleType::Type ExperimentalConditions::getHadronType() const {
    return m_hadronType;
}

void ExperimentalConditions::setHadronType(ParticleType::Type hadronType) {
    m_hadronType = hadronType;
}

const NumA::Vector3D &ExperimentalConditions::getHadronPolarisation() const {
    return m_hadronPolarisation;
}

void ExperimentalConditions::setHadronPolarisation(
        const NumA::Vector3D &hadronPolarisation) {
    m_hadronPolarisation = hadronPolarisation;
}

} /* namespace EPIC */

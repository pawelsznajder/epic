#include "../../../include/beans/containers/DVMPKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/DVMP/DVMPObservableKinematic.h>

namespace EPIC {

DVMPKinematic::DVMPKinematic() :
        BaseObject("DVMPKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_E(0.), m_phi(
                0.), m_phiS(0.), m_mesonType(ParticleType::UNDEFINED), m_mesonPolarization(
                PARTONS::MesonPolarization::UNDEFINED) {
}

DVMPKinematic::DVMPKinematic(double xB, double t, double Q2, double E,
        double phi, double phiS, ParticleType::Type mesonType,
        PARTONS::MesonPolarization::Type mesonPolarization) :
        BaseObject("DVMPKinematic"), m_xB(xB), m_t(t), m_Q2(Q2), m_E(E), m_phi(
                phi), m_phiS(phiS), m_mesonType(mesonType), m_mesonPolarization(
                mesonPolarization) {
}

DVMPKinematic::DVMPKinematic(const DVMPKinematic &other) :
        BaseObject(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(other.m_Q2), m_E(
                other.m_E), m_phi(other.m_phi), m_phiS(other.m_phiS), m_mesonType(
                other.m_mesonType), m_mesonPolarization(
                other.m_mesonPolarization) {
}

DVMPKinematic::~DVMPKinematic() {
}

std::string DVMPKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "xB: " << m_xB << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';
    formatter << "phiS: " << m_phiS << ' ';
    formatter << "meson type: " << ParticleType(m_mesonType).toString() << ' ';
    formatter << "meson polarization: "
            << PARTONS::MesonPolarization(m_mesonPolarization).toString();

    return formatter.str();
}

PARTONS::DVMPObservableKinematic DVMPKinematic::toPARTONSDVMPObservableKinematic() const {

    if (m_mesonType != ParticleType::PI0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Unknown conversion");
    }

    return PARTONS::DVMPObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi,
            PARTONS::MesonType::PI0);
}

double DVMPKinematic::getXB() const {
    return m_xB;
}

double DVMPKinematic::getT() const {
    return m_t;
}

double DVMPKinematic::getQ2() const {
    return m_Q2;
}

double DVMPKinematic::getE() const {
    return m_E;
}

double DVMPKinematic::getPhi() const {
    return m_phi;
}

double DVMPKinematic::getPhiS() const {
    return m_phiS;
}

ParticleType::Type DVMPKinematic::getMesonType() const {
    return m_mesonType;
}

PARTONS::MesonPolarization::Type DVMPKinematic::getMesonPolarization() const {
    return m_mesonPolarization;
}

void DVMPKinematic::setXB(double xB) {
    m_xB = xB;
}

void DVMPKinematic::setT(double t) {
    m_t = t;
}

void DVMPKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

void DVMPKinematic::setE(double E) {
    m_E = E;
}

void DVMPKinematic::setPhi(double phi) {
    m_phi = phi;
}

void DVMPKinematic::setPhiS(double phiS) {
    m_phiS = phiS;
}

void DVMPKinematic::setMesonType(ParticleType::Type mesonType) {
    m_mesonType = mesonType;
}

void DVMPKinematic::setMesonPolarization(
        PARTONS::MesonPolarization::Type mesonPolarization) {
    m_mesonPolarization = mesonPolarization;
}

}

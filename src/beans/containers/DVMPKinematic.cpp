#include "../../../include/beans/containers/DVMPKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/DVMP/DVMPObservableKinematic.h>
#include <partons/FundamentalPhysicalConstants.h>

namespace EPIC {

DVMPKinematic::DVMPKinematic() :
        BaseObject("DVMPKinematic"), m_y(0.), m_Q2(0.), m_t(0.), m_E(0.), m_phi(
                0.), m_phiS(0.), m_mesonType(ParticleType::UNDEFINED), m_mesonPolarization(
                PARTONS::PolarizationType::UNDEFINED) {
}

DVMPKinematic::DVMPKinematic(double y, double Q2, double t, double E,
        double phi, double phiS, ParticleType::Type mesonType,
        PARTONS::PolarizationType::Type mesonPolarization) :
        BaseObject("DVMPKinematic"), m_y(y), m_Q2(Q2), m_t(t), m_E(E), m_phi(
                phi), m_phiS(phiS), m_mesonType(mesonType), m_mesonPolarization(
                mesonPolarization) {
}

DVMPKinematic::DVMPKinematic(const DVMPKinematic &other) :
        BaseObject(other), m_y(other.m_y), m_Q2(other.m_Q2), m_t(other.m_t), m_E(
                other.m_E), m_phi(other.m_phi), m_phiS(other.m_phiS), m_mesonType(
                other.m_mesonType), m_mesonPolarization(
                other.m_mesonPolarization) {
}

DVMPKinematic::~DVMPKinematic() {
}

std::string DVMPKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "y: " << m_y << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';
    formatter << "phiS: " << m_phiS << ' ';
    formatter << "meson type: " << ParticleType(m_mesonType).toString() << ' ';
    formatter << "meson polarization: "
            << PARTONS::PolarizationType(m_mesonPolarization).toString();

    return formatter.str();
}

PARTONS::DVMPObservableKinematic DVMPKinematic::toPARTONSDVMPObservableKinematic() const {

    if (m_mesonType != ParticleType::PI0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Unknown conversion");
    }

    //TODO mass of proton is explicitly set here. Instead, the function should get ExperimentalConditions with true target mass.
    double xB = m_Q2 / (2 * PARTONS::Constant::PROTON_MASS * m_E * m_y);

    return PARTONS::DVMPObservableKinematic(xB, m_t, m_Q2, m_E, m_phi,
            PARTONS::MesonType::PI0);
}

double DVMPKinematic::getY() const {
    return m_y;
}

double DVMPKinematic::getQ2() const {
    return m_Q2;
}

double DVMPKinematic::getT() const {
    return m_t;
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

PARTONS::PolarizationType::Type DVMPKinematic::getPolarizationType() const {
    return m_mesonPolarization;
}

void DVMPKinematic::setY(double y) {
    m_y = y;
}

void DVMPKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

void DVMPKinematic::setT(double t) {
    m_t = t;
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

void DVMPKinematic::setPolarizationType(
        PARTONS::PolarizationType::Type mesonPolarization) {
    m_mesonPolarization = mesonPolarization;
}

}

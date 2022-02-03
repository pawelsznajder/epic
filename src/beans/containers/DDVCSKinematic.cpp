#include "../../../include/beans/containers/DDVCSKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/TCS/TCSObservableKinematic.h>

namespace EPIC {

DDVCSKinematic::DDVCSKinematic() :
        BaseObject("DDVCSKinematic"), m_y(0.), m_Q2(0.), m_t(0.), m_Q2Prim(0.), m_E(
                0.), m_phi(0.), m_phiS(0.), m_theta(0.) {
}

DDVCSKinematic::DDVCSKinematic(double y, double Q2, double t, double Q2Prim,
        double E, double phi, double phiS, double theta) :
        BaseObject("DDVCSKinematic"), m_y(y), m_Q2(Q2), m_t(t), m_Q2Prim(
                Q2Prim), m_E(E), m_phi(phi), m_phiS(phiS), m_theta(theta) {
}

DDVCSKinematic::DDVCSKinematic(const DDVCSKinematic &other) :
        BaseObject(other), m_y(other.m_y), m_Q2(other.m_Q2), m_t(other.m_t), m_Q2Prim(
                other.m_Q2Prim), m_E(other.m_E), m_phi(other.m_phi), m_phiS(
                other.m_phiS), m_theta(other.m_theta) {
}

DDVCSKinematic::~DDVCSKinematic() {
}

std::string DDVCSKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "y: " << m_y << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "Q'2: " << m_Q2Prim << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';
    formatter << "phiS: " << m_phiS << ' ';
    formatter << "theta: " << m_theta;

    return formatter.str();
}

PARTONS::TCSObservableKinematic DDVCSKinematic::toPARTONSTCSObservableKinematic() const {
    return PARTONS::TCSObservableKinematic(m_t, m_Q2Prim, m_y * m_E, m_phi,
            m_theta);
}

double DDVCSKinematic::getY() const {
    return m_y;
}

double DDVCSKinematic::getQ2() const {
    return m_Q2;
}

double DDVCSKinematic::getT() const {
    return m_t;
}

double DDVCSKinematic::getQ2Prim() const {
    return m_Q2Prim;
}

double DDVCSKinematic::getE() const {
    return m_E;
}

double DDVCSKinematic::getPhi() const {
    return m_phi;
}

double DDVCSKinematic::getPhiS() const {
    return m_phiS;
}

double DDVCSKinematic::getTheta() const {
    return m_theta;
}

void DDVCSKinematic::setY(double y) {
    m_y = y;
}

void DDVCSKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

void DDVCSKinematic::setT(double t) {
    m_t = t;
}

void DDVCSKinematic::setQ2Prim(double Q2Prim) {
    m_Q2Prim = Q2Prim;
}

void DDVCSKinematic::setE(double E) {
    m_E = E;
}

void DDVCSKinematic::setPhi(double phi) {
    m_phi = phi;
}

void DDVCSKinematic::setPhiS(double phiS) {
    m_phiS = phiS;
}

void DDVCSKinematic::setTheta(double theta) {
    m_theta = theta;
}

}

#include "../../../include/beans/containers/DVCSKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/DVCS/DVCSObservableKinematic.h>

namespace EPIC {

DVCSKinematic::DVCSKinematic() :
        BaseObject("DVCSKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_E(0.), m_phi(
                0.), m_phiS(0.) {
}

DVCSKinematic::DVCSKinematic(double xB, double t, double Q2, double E,
        double phi, double phiS) :
        BaseObject("DVCSKinematic"), m_xB(xB), m_t(t), m_Q2(Q2), m_E(E), m_phi(
                phi), m_phiS(phiS) {
}

DVCSKinematic::DVCSKinematic(const DVCSKinematic &other) :
        BaseObject(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(other.m_Q2), m_E(
                other.m_E), m_phi(other.m_phi), m_phiS(other.m_phiS) {
}

DVCSKinematic::~DVCSKinematic() {
}

std::string DVCSKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "xB: " << m_xB << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';
    formatter << "phiS: " << m_phiS;

    return formatter.str();
}

PARTONS::DVCSObservableKinematic DVCSKinematic::toPARTONSDVCSObservableKinematic() const {
    return PARTONS::DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi);
}

double DVCSKinematic::getXB() const {
    return m_xB;
}

double DVCSKinematic::getT() const {
    return m_t;
}

double DVCSKinematic::getQ2() const {
    return m_Q2;
}

double DVCSKinematic::getE() const {
    return m_E;
}

double DVCSKinematic::getPhi() const {
    return m_phi;
}

double DVCSKinematic::getPhiS() const {
    return m_phiS;
}

void DVCSKinematic::setXB(double xB) {
    m_xB = xB;
}

void DVCSKinematic::setT(double t) {
    m_t = t;
}

void DVCSKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

void DVCSKinematic::setE(double E) {
    m_E = E;
}

void DVCSKinematic::setPhi(double phi) {
    m_phi = phi;
}

void DVCSKinematic::setPhiS(double phiS) {
    m_phiS = phiS;
}

}

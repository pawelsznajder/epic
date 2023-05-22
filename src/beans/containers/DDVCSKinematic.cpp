#include "../../../include/beans/containers/DDVCSKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/DDVCS/DDVCSObservableKinematic.h>
#include <partons/FundamentalPhysicalConstants.h>

namespace EPIC {

DDVCSKinematic::DDVCSKinematic() :
        BaseObject("DDVCSKinematic"), m_y(0.), m_Q2(0.), m_t(0.), m_Q2Prim(0.), m_E(
                0.), m_phi(0.), m_phiS(0.), m_phiL(0.), m_thetaL(0.) {
}

DDVCSKinematic::DDVCSKinematic(double y, double Q2, double t, double Q2Prim,
        double E, double phi, double phiS, double phiL, double thetaL) :
        BaseObject("DDVCSKinematic"), m_y(y), m_Q2(Q2), m_t(t), m_Q2Prim(
                Q2Prim), m_E(E), m_phi(phi), m_phiS(phiS), m_phiL(phiL), m_thetaL(thetaL) {
}

DDVCSKinematic::DDVCSKinematic(const DDVCSKinematic &other) :
        BaseObject(other), m_y(other.m_y), m_Q2(other.m_Q2), m_t(other.m_t), m_Q2Prim(
                other.m_Q2Prim), m_E(other.m_E), m_phi(other.m_phi), m_phiS(
                other.m_phiS), m_phiL(other.m_phiL), m_thetaL(other.m_thetaL) {
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
    formatter << "phiL: " << m_phiL << ' ';
    formatter << "thetaL: " << m_thetaL;

    return formatter.str();
}

PARTONS::DDVCSObservableKinematic DDVCSKinematic::toPARTONSDDVCSObservableKinematic() const {

    //TODO mass of proton is explicitly set here. Instead, the function should get ExperimentalConditions with true target mass.
    double xB = m_Q2 / (2 * PARTONS::Constant::PROTON_MASS * m_E * m_y);

    return PARTONS::DDVCSObservableKinematic(xB, m_t, m_Q2, m_Q2Prim, m_E, m_phi, m_phiL,
            m_thetaL);
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

double DDVCSKinematic::getPhiL() const {
    return m_phiL;
}

double DDVCSKinematic::getThetaL() const {
    return m_thetaL;
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

void DDVCSKinematic::setPhiL(double phiL) {
    m_phiL = phiL;
}

void DDVCSKinematic::setThetaL(double thetaL) {
    m_thetaL = thetaL;
}

}

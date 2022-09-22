#include "../../../include/beans/containers/GAM2Kinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/GAM2/GAM2ObservableKinematic.h>

namespace EPIC {

GAM2Kinematic::GAM2Kinematic() :
        BaseObject("GAM2Kinematic"), m_y(0.), m_Q2(0.), m_t(0.), m_uPrim(0.), m_Mgg2(
                0.), m_E(0.), m_phi(0.) {
}

GAM2Kinematic::GAM2Kinematic(double y, double Q2, double t, double uPrim,
        double Mgg2, double E, double phi) :
        BaseObject("GAM2Kinematic"), m_y(y), m_Q2(Q2), m_t(t), m_uPrim(uPrim), m_Mgg2(
                Mgg2), m_E(E), m_phi(phi) {
}

GAM2Kinematic::GAM2Kinematic(const GAM2Kinematic &other) :
        BaseObject(other), m_y(other.m_y), m_Q2(other.m_Q2), m_t(other.m_t), m_uPrim(
                other.m_uPrim), m_Mgg2(other.m_Mgg2), m_E(other.m_E), m_phi(
                other.m_phi) {
}

GAM2Kinematic::~GAM2Kinematic() {
}

std::string GAM2Kinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "y: " << m_y << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "u': " << m_uPrim << ' ';
    formatter << "Mgg2: " << m_Mgg2 << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';

    return formatter.str();
}

PARTONS::GAM2ObservableKinematic GAM2Kinematic::toPARTONSGAM2ObservableKinematic() const {
    return PARTONS::GAM2ObservableKinematic(m_t, m_uPrim, m_Mgg2, m_y * m_E, m_phi);
}

double GAM2Kinematic::getY() const {
    return m_y;
}

void GAM2Kinematic::setY(double y) {
    m_y = y;
}

double GAM2Kinematic::getQ2() const {
    return m_Q2;
}

void GAM2Kinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

double GAM2Kinematic::getT() const {
    return m_t;
}

void GAM2Kinematic::setT(double t) {
    m_t = t;
}

double GAM2Kinematic::getUPrim() const {
    return m_uPrim;
}

void GAM2Kinematic::setUPrim(double uPrim) {
    m_uPrim = uPrim;
}

double GAM2Kinematic::getMgg2() const {
    return m_Mgg2;
}

void GAM2Kinematic::setMgg2(double mgg2) {
    m_Mgg2 = mgg2;
}

double GAM2Kinematic::getE() const {
    return m_E;
}

void GAM2Kinematic::setE(double e) {
    m_E = e;
}

double GAM2Kinematic::getPhi() const {
    return m_phi;
}

void GAM2Kinematic::setPhi(double phi) {
    m_phi = phi;
}

}

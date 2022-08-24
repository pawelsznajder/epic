#include "../../../include/beans/containers/GAM2Kinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

//#include <partons/beans/observable/GAM2/GAM2ObservableKinematic.h>

namespace EPIC {

GAM2Kinematic::GAM2Kinematic() :
        BaseObject("GAM2Kinematic"), m_t(0.), m_uPrim(0.), m_Mgg2(0.), m_E(0.), m_phi(
                0.) {
}

GAM2Kinematic::GAM2Kinematic(double t, double uPrim, double Mgg2, double E,
        double phi) :
        BaseObject("GAM2Kinematic"), m_t(t), m_uPrim(uPrim), m_Mgg2(Mgg2), m_E(
                E), m_phi(phi) {
}

GAM2Kinematic::GAM2Kinematic(const GAM2Kinematic &other) :
        BaseObject(other), m_t(other.m_t), m_uPrim(other.m_uPrim), m_Mgg2(
                other.m_Mgg2), m_E(other.m_E), m_phi(other.m_phi) {
}

GAM2Kinematic::~GAM2Kinematic() {
}

std::string GAM2Kinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "t: " << m_t << ' ';
    formatter << "u': " << m_uPrim << ' ';
    formatter << "Mgg2: " << m_Mgg2 << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';

    return formatter.str();
}

//PARTONS::GAM2ObservableKinematic GAM2Kinematic::toPARTONSGAM2ObservableKinematic() const {
//    return PARTONS::GAM2ObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi);
//}

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

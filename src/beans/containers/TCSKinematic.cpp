#include "../../../include/beans/containers/TCSKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/TCS/TCSObservableKinematic.h>

namespace EPIC {

TCSKinematic::TCSKinematic() :
        DDVCSKinematic() {
}

TCSKinematic::TCSKinematic(double y, double Q2, double t, double Q2Prim,
        double E, double phiS, double phiL, double thetaL) :
        DDVCSKinematic(y, Q2, t, Q2Prim, E, 0., phiS, phiL, thetaL) {
}

TCSKinematic::TCSKinematic(const TCSKinematic &other) :
        DDVCSKinematic(other) {
}

TCSKinematic::~TCSKinematic() {
}

std::string TCSKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "y: " << m_y << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "Q'2: " << m_Q2Prim << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phiS: " << m_phiS << ' ';
    formatter << "phiL: " << m_phiL << ' ';
    formatter << "thetaL: " << m_thetaL;

    return formatter.str();
}

PARTONS::TCSObservableKinematic TCSKinematic::toPARTONSTCSObservableKinematic() const {
    return PARTONS::TCSObservableKinematic(m_t, m_Q2Prim, m_y * m_E, m_phiL,
            m_thetaL);
}

}

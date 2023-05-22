#include "../../../include/beans/containers/DVCSKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include <partons/beans/observable/DVCS/DVCSObservableKinematic.h>
#include <partons/FundamentalPhysicalConstants.h>

namespace EPIC {

DVCSKinematic::DVCSKinematic() :
        DDVCSKinematic() {
}

DVCSKinematic::DVCSKinematic(double y, double Q2, double t, double E,
        double phi, double phiS) :
        DDVCSKinematic(y, Q2, t, 0., E, phi, phiS, 0., 0.) {
}

DVCSKinematic::DVCSKinematic(const DVCSKinematic &other) :
        DDVCSKinematic(other) {
}

DVCSKinematic::~DVCSKinematic() {
}

std::string DVCSKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "y: " << m_y << ' ';
    formatter << "Q2: " << m_Q2 << ' ';
    formatter << "t: " << m_t << ' ';
    formatter << "E: " << m_E << ' ';
    formatter << "phi: " << m_phi << ' ';
    formatter << "phiS: " << m_phiS;

    return formatter.str();
}

PARTONS::DVCSObservableKinematic DVCSKinematic::toPARTONSDVCSObservableKinematic() const {

    //TODO mass of proton is explicitly set here. Instead, the function should get ExperimentalConditions with true target mass.
    double xB = m_Q2 / (2 * PARTONS::Constant::PROTON_MASS * m_E * m_y);

    return PARTONS::DVCSObservableKinematic(xB, m_t, m_Q2, m_E, m_phi);
}

}

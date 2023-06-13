/*
 * DVCSKinematicRanges.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/DVCSKinematicRanges.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <limits>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"
#include "../../../include/beans/containers/DVCSKinematic.h"
#include "../../../include/beans/containers/ExperimentalConditions.h"

namespace EPIC {

DVCSKinematicRanges::DVCSKinematicRanges() :
        DDVCSKinematicRanges() {
}

DVCSKinematicRanges::DVCSKinematicRanges(const KinematicRange &rangeY,
        const KinematicRange &rangeQ2, const KinematicRange &rangeT,
        const KinematicRange &rangePhi, const KinematicRange &rangePhiS) :
        DDVCSKinematicRanges(rangeY, rangeQ2, rangeT,
                KinematicRange(std::numeric_limits<double>::lowest(),
                        std::numeric_limits<double>::max()), rangePhi,
                rangePhiS,
                KinematicRange(std::numeric_limits<double>::lowest(),
                        std::numeric_limits<double>::max()),
                KinematicRange(std::numeric_limits<double>::lowest(),
                        std::numeric_limits<double>::max())) {
}

DVCSKinematicRanges::DVCSKinematicRanges(const DVCSKinematicRanges &other) :
        DDVCSKinematicRanges(other) {
}

DVCSKinematicRanges::~DVCSKinematicRanges() {
}

std::string DVCSKinematicRanges::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "DVCS kinematic range y: " << m_rangeY.toString() << '\n';
    formatter << "DVCS kinematic range Q2: " << m_rangeQ2.toString() << '\n';
    formatter << "DVCS kinematic range t: " << m_rangeT.toString() << '\n';
    formatter << "DVCS kinematic range phi: " << m_rangePhi.toString() << '\n';
    formatter << "DVCS kinematic range phiS: " << m_rangePhiS.toString() << '\n'
            << '\n';

    formatter << "DVCS kinematic range xB: " << m_rangeXB.toString();

    return formatter.str();
}

bool DVCSKinematicRanges::inRange(
        const ExperimentalConditions& experimentalConditions,
        const DVCSKinematic& obsKin) const {

    if (!m_rangeY.inRange(obsKin.getY()))
        return false;
    if (!m_rangeQ2.inRange(obsKin.getQ2()))
        return false;
    if (!m_rangeT.inRange(obsKin.getT()))
        return false;
    if (!m_rangePhi.inRange(obsKin.getPhi()))
        return false;
    if (!m_rangePhiS.inRange(obsKin.getPhiS()))
        return false;

    double xB =
            obsKin.getQ2()
                    / (2
                            * ParticleType(
                                    experimentalConditions.getHadronType()).getMass()
                            * obsKin.getE() * obsKin.getY());

    if (!m_rangeXB.inRange(xB))
        return false;

    return true;
}

DVCSKinematicRanges DVCSKinematicRanges::getDVCSKinematicRangesfromTask(
        const MonteCarloTask &task) {

    DVCSKinematicRanges result;

    const ElemUtils::Parameters &data =
            task.getKinematicRange().getParameters();

    result.setRangeY(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Y, 2)));

    result.setRangeQ2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2,
                            2)));

    result.setRangeT(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_T, 2)));

    result.setRangePhi(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHI, 2,
                            true)));

    result.setRangePhiS(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIS, 2,
                            true)));

    if (data.isAvailable(DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_XB)) {

        result.setRangeXB(
                KinematicRange(
                        ContainerUtils::findAndParseVectorDouble(
                                "DVCSKinematicRanges", data,
                                DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_XB,
                                2)));
    } else {
        result.setRangeXB(KinematicRange(0., 1.));
    }

    return result;
}

} /* namespace EPIC */

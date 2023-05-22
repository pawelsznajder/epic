/*
 * TCSKinematicRanges.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/TCSKinematicRanges.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <limits>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"
#include "../../../include/beans/containers/TCSKinematic.h"

namespace EPIC {

TCSKinematicRanges::TCSKinematicRanges() :
        DDVCSKinematicRanges() {
}

TCSKinematicRanges::TCSKinematicRanges(const KinematicRange &rangeY,
        const KinematicRange &rangeQ2, const KinematicRange &rangeT,
        const KinematicRange &rangeQ2Prim, const KinematicRange &rangePhiS,
        const KinematicRange &rangePhiL, const KinematicRange &rangeThetaL) :
        DDVCSKinematicRanges(rangeY, rangeQ2, rangeT, rangeQ2Prim,
                KinematicRange(std::numeric_limits<double>::lowest(),
                        std::numeric_limits<double>::max()), rangePhiS,
                rangePhiL, rangeThetaL) {
}

TCSKinematicRanges::TCSKinematicRanges(const TCSKinematicRanges &other) :
        DDVCSKinematicRanges(other) {
}

TCSKinematicRanges::~TCSKinematicRanges() {
}

std::string TCSKinematicRanges::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "TCS kinematic range y: " << m_rangeY.toString() << '\n';
    formatter << "TCS kinematic range Q2: " << m_rangeQ2.toString() << '\n';
    formatter << "TCS kinematic range t: " << m_rangeT.toString() << '\n';
    formatter << "TCS kinematic range Q'2: " << m_rangeQ2Prim.toString()
            << '\n';
    formatter << "TCS kinematic range phiS: " << m_rangePhiS.toString() << '\n';
    formatter << "TCS kinematic range phiL: " << m_rangePhiL.toString() << '\n';
    formatter << "TCS kinematic range thetaL: " << m_rangeThetaL.toString();

    return formatter.str();
}

bool TCSKinematicRanges::inRange(
        const ExperimentalConditions& experimentalConditions,
        const TCSKinematic& obsKin) const {

    if (!m_rangeY.inRange(obsKin.getY()))
        return false;
    if (!m_rangeQ2.inRange(obsKin.getQ2()))
        return false;
    if (!m_rangeT.inRange(obsKin.getT()))
        return false;
    if (!m_rangeQ2Prim.inRange(obsKin.getQ2Prim()))
        return false;
    if (!m_rangePhiS.inRange(obsKin.getPhiS()))
        return false;
    if (!m_rangePhiL.inRange(obsKin.getPhiL()))
        return false;
    if (!m_rangeThetaL.inRange(obsKin.getThetaL()))
        return false;

    return true;
}

TCSKinematicRanges TCSKinematicRanges::getTCSKinematicRangesfromTask(const MonteCarloTask &task) {

    TCSKinematicRanges result;

    const ElemUtils::Parameters &data =
            task.getKinematicRange().getParameters();

    result.setRangeY(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Y, 2)));

    result.setRangeQ2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2,
                            2)));

    result.setRangeT(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_T, 2)));

    result.setRangeQ2Prim(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2Prim,
                            2)));

    result.setRangePhiS(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIS, 2,
                            true)));

    result.setRangePhiL(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIL, 2,
                            true)));

    result.setRangeThetaL(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "TCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_THETAL,
                            2, true)));

    return result;
}

} /* namespace EPIC */

/*
 * GAM2KinematicRanges.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/GAM2KinematicRanges.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"
#include "../../../include/beans/containers/GAM2Kinematic.h"
#include "../../../include/beans/containers/ExperimentalConditions.h"
#include "../../../include/beans/types/ParticleType.h"

namespace EPIC {

const std::string GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_Y = "range_y";
const std::string GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_Q2 = "range_Q2";
const std::string GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_T = "range_t";
const std::string GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_UPRIM =
        "range_uPrim";
const std::string GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_MGG2 = "range_Mgg2";
const std::string GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_PHI = "range_phi";

GAM2KinematicRanges::GAM2KinematicRanges() :
        PARTONS::BaseObject("GAM2KinematicRanges") {
}

GAM2KinematicRanges::GAM2KinematicRanges(const KinematicRange &rangeY,
        const KinematicRange &rangeQ2, const KinematicRange &rangeT,
        const KinematicRange &rangeUPrim, const KinematicRange &rangeMgg2,
        const KinematicRange &rangePhi) :
        PARTONS::BaseObject("GAM2KinematicRanges") {

    m_rangeY = rangeY;
    m_rangeQ2 = rangeQ2;
    m_rangeT = rangeT;
    m_rangeUPrim = rangeUPrim;
    m_rangeMgg2 = rangeMgg2;
    m_rangePhi = rangePhi;
}

GAM2KinematicRanges::GAM2KinematicRanges(const GAM2KinematicRanges &other) :
        PARTONS::BaseObject(other) {

    m_rangeY = other.m_rangeY;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangeT = other.m_rangeT;
    m_rangeUPrim = other.m_rangeUPrim;
    m_rangeMgg2 = other.m_rangeMgg2;
    m_rangePhi = other.m_rangePhi;
}

GAM2KinematicRanges::~GAM2KinematicRanges() {
}

std::string GAM2KinematicRanges::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "GAM2 kinematic range y: " << m_rangeY.toString() << '\n';
    formatter << "GAM2 kinematic range Q2: " << m_rangeQ2.toString() << '\n';
    formatter << "GAM2 kinematic range t: " << m_rangeT.toString() << '\n';
    formatter << "GAM2 kinematic range u': " << m_rangeUPrim.toString() << '\n';
    formatter << "GAM2 kinematic range Mgg2: " << m_rangeMgg2.toString()
            << '\n';
    formatter << "GAM2 kinematic range phi: " << m_rangePhi.toString();

    return formatter.str();
}

bool GAM2KinematicRanges::inRange(
        const ExperimentalConditions& experimentalConditions,
        const GAM2Kinematic& obsKin) const {

    if (!m_rangeY.inRange(obsKin.getY()))
        return false;
    if (!m_rangeQ2.inRange(obsKin.getQ2()))
        return false;
    if (!m_rangeT.inRange(obsKin.getT()))
        return false;
    if (!m_rangeUPrim.inRange(obsKin.getUPrim()))
        return false;
    if (!m_rangeMgg2.inRange(obsKin.getMgg2()))
        return false;
    if (!m_rangePhi.inRange(obsKin.getPhi()))
        return false;

    return true;
}

const KinematicRange& GAM2KinematicRanges::getRangeY() const {
    return m_rangeY;
}

void GAM2KinematicRanges::setRangeY(const KinematicRange& rangeY) {
    m_rangeY = rangeY;
}

const KinematicRange& GAM2KinematicRanges::getRangeQ2() const {
    return m_rangeQ2;
}

void GAM2KinematicRanges::setRangeQ2(const KinematicRange& rangeQ2) {
    m_rangeQ2 = rangeQ2;
}

const KinematicRange& GAM2KinematicRanges::getRangeT() const {
    return m_rangeT;
}

void GAM2KinematicRanges::setRangeT(const KinematicRange& rangeT) {
    m_rangeT = rangeT;
}

const KinematicRange& GAM2KinematicRanges::getRangeUPrim() const {
    return m_rangeUPrim;
}

void GAM2KinematicRanges::setRangeUPrim(const KinematicRange& rangeUPrim) {
    m_rangeUPrim = rangeUPrim;
}

const KinematicRange& GAM2KinematicRanges::getRangeMgg2() const {
    return m_rangeMgg2;
}

void GAM2KinematicRanges::setRangeMgg2(const KinematicRange& rangeMgg2) {
    m_rangeMgg2 = rangeMgg2;
}

const KinematicRange& GAM2KinematicRanges::getRangePhi() const {
    return m_rangePhi;
}

void GAM2KinematicRanges::setRangePhi(const KinematicRange& rangePhi) {
    m_rangePhi = rangePhi;
}

GAM2KinematicRanges GAM2KinematicRanges::fromTask(const MonteCarloTask &task) {

    GAM2KinematicRanges result;

    const ElemUtils::Parameters &data =
            task.getKinematicRange().getParameters();

    result.setRangeY(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "GAM2KinematicRanges", data,
                            GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_Y, 2)));

    result.setRangeQ2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "GAM2KinematicRanges", data,
                            GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_Q2, 2)));

    result.setRangeT(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "GAM2KinematicRanges", data,
                            GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_T, 2)));

    result.setRangeUPrim(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "GAM2KinematicRanges", data,
                            GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_UPRIM,
                            2)));

    result.setRangeMgg2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "GAM2KinematicRanges", data,
                            GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_MGG2,
                            2)));

    result.setRangePhi(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "GAM2KinematicRanges", data,
                            GAM2KinematicRanges::GAM2_KINEMATIC_RANGE_PHI, 2,
                            true)));

    return result;
}

} /* namespace EPIC */

/*
 * DVCSKinematicRanges.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/DVCSKinematicRanges.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"
#include "../../../include/beans/containers/DVCSKinematic.h"
#include "../../../include/beans/containers/ExperimentalConditions.h"
#include "../../../include/beans/types/ParticleType.h"

namespace EPIC {

const std::string DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_XB = "range_xB";
const std::string DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_T = "range_t";
const std::string DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_Q2 = "range_Q2";
const std::string DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_PHI = "range_phi";
const std::string DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_PHIS = "range_phiS";
const std::string DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_Y = "range_y";

DVCSKinematicRanges::DVCSKinematicRanges() :
        PARTONS::BaseObject("DVCSKinematicRanges") {
}

DVCSKinematicRanges::DVCSKinematicRanges(const KinematicRange &rangeXb,
        const KinematicRange &rangeT, const KinematicRange &rangeQ2,
        const KinematicRange &rangePhi, const KinematicRange &rangePhiS) :
        PARTONS::BaseObject("DVCSKinematicRanges") {

    m_rangeXb = rangeXb;
    m_rangeT = rangeT;
    m_rangeQ2 = rangeQ2;
    m_rangePhi = rangePhi;
    m_rangePhiS = rangePhiS;

    m_rangeY = KinematicRange(0., 1.);
}

DVCSKinematicRanges::DVCSKinematicRanges(const KinematicRange &rangeXb,
        const KinematicRange &rangeT, const KinematicRange &rangeQ2,
        const KinematicRange &rangePhi, const KinematicRange &rangePhiS,
        const KinematicRange &rangeY) :
        PARTONS::BaseObject("DVCSKinematicRanges") {

    m_rangeXb = rangeXb;
    m_rangeT = rangeT;
    m_rangeQ2 = rangeQ2;
    m_rangePhi = rangePhi;
    m_rangePhiS = rangePhiS;

    m_rangeY = rangeY;
}

DVCSKinematicRanges::DVCSKinematicRanges(const DVCSKinematicRanges &other) :
        PARTONS::BaseObject(other) {

    m_rangeXb = other.m_rangeXb;
    m_rangeT = other.m_rangeT;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangePhi = other.m_rangePhi;
    m_rangePhiS = other.m_rangePhiS;

    m_rangeY = other.m_rangeY;
}

DVCSKinematicRanges::~DVCSKinematicRanges() {
}

std::string DVCSKinematicRanges::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "DVCS kinematic range xB: " << m_rangeXb.toString() << '\n';
    formatter << "DVCS kinematic range t: " << m_rangeT.toString() << '\n';
    formatter << "DVCS kinematic range Q2: " << m_rangeQ2.toString() << '\n';
    formatter << "DVCS kinematic range phi: " << m_rangePhi.toString() << '\n';
    formatter << "DVCS kinematic range phiS: " << m_rangePhiS.toString()
            << '\n';
    formatter << "DVCS kinematic range y: " << m_rangeY.toString();

    return formatter.str();
}

bool DVCSKinematicRanges::inRange(
        const ExperimentalConditions& experimentalConditions,
        const DVCSKinematic& obsKin) const {

    if (!m_rangeXb.inRange(obsKin.getXB()))
        return false;
    if (!m_rangeT.inRange(obsKin.getT()))
        return false;
    if (!m_rangeQ2.inRange(obsKin.getQ2()))
        return false;
    if (!m_rangePhi.inRange(obsKin.getPhi()))
        return false;
    if (!m_rangePhiS.inRange(obsKin.getPhiS()))
        return false;

    if (!m_rangeY.inRange(
            obsKin.getQ2()
                    / (2
                            * ParticleType(
                                    experimentalConditions.getHadronType()).getMass()
                            * experimentalConditions.getLeptonEnergyFixedTargetEquivalent()
                            * obsKin.getXB())))
        return false;

    return true;
}

DVCSKinematicRanges DVCSKinematicRanges::fromTask(const MonteCarloTask &task) {

    DVCSKinematicRanges result;

    const ElemUtils::Parameters &data =
            task.getKinematicRange().getParameters();

    result.setRangeXb(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_XB, 2)));

    result.setRangeT(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_T, 2)));

    result.setRangeQ2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_Q2, 2)));

    result.setRangePhi(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_PHI, 2,
                            true)));

    result.setRangePhiS(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVCSKinematicRanges", data,
                            DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_PHIS, 2,
                            true)));

    if (data.isAvailable(DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_Y)) {

        result.setRangeY(
                KinematicRange(
                        ContainerUtils::findAndParseVectorDouble(
                                "DVCSKinematicRanges", data,
                                DVCSKinematicRanges::DVCS_KINEMATIC_RANGE_Y,
                                2)));
    } else {
        result.setRangeY(KinematicRange(0., 1.));
    }

    return result;
}

const KinematicRange &DVCSKinematicRanges::getRangeXb() const {
    return m_rangeXb;
}

void DVCSKinematicRanges::setRangeXb(const KinematicRange &rangeXb) {
    m_rangeXb = rangeXb;
}

const KinematicRange &DVCSKinematicRanges::getRangeT() const {
    return m_rangeT;
}

void DVCSKinematicRanges::setRangeT(const KinematicRange &rangeT) {
    m_rangeT = rangeT;
}

const KinematicRange &DVCSKinematicRanges::getRangeQ2() const {
    return m_rangeQ2;
}

void DVCSKinematicRanges::setRangeQ2(const KinematicRange &rangeQ2) {
    m_rangeQ2 = rangeQ2;
}

const KinematicRange &DVCSKinematicRanges::getRangePhi() const {
    return m_rangePhi;
}

void DVCSKinematicRanges::setRangePhi(const KinematicRange &rangePhi) {
    m_rangePhi = rangePhi;
}

const KinematicRange &DVCSKinematicRanges::getRangePhiS() const {
    return m_rangePhiS;
}

void DVCSKinematicRanges::setRangePhiS(const KinematicRange &rangePhiS) {
    m_rangePhiS = rangePhiS;
}

const KinematicRange &DVCSKinematicRanges::getRangeY() const {
    return m_rangeY;
}

void DVCSKinematicRanges::setRangeY(const KinematicRange &rangeY) {
    m_rangeY = rangeY;
}

} /* namespace EPIC */

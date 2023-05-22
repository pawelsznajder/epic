/*
 * DVMPKinematicRanges.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/DVMPKinematicRanges.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"
#include "../../../include/beans/containers/DVMPKinematic.h"
#include "../../../include/beans/containers/ExperimentalConditions.h"
#include "../../../include/beans/types/ParticleType.h"

namespace EPIC {

const std::string DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_Y = "range_y";
const std::string DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_Q2 = "range_Q2";
const std::string DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_T = "range_t";
const std::string DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_PHI = "range_phi";
const std::string DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_PHIS = "range_phiS";

const std::string DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_XB = "range_xB";

DVMPKinematicRanges::DVMPKinematicRanges() :
        PARTONS::BaseObject("DVMPKinematicRanges") {
}

DVMPKinematicRanges::DVMPKinematicRanges(const KinematicRange &rangeY,
        const KinematicRange &rangeQ2, const KinematicRange &rangeT,
        const KinematicRange &rangePhi, const KinematicRange &rangePhiS) :
        PARTONS::BaseObject("DVMPKinematicRanges") {

    m_rangeY = rangeY;
    m_rangeQ2 = rangeQ2;
    m_rangeT = rangeT;
    m_rangePhi = rangePhi;
    m_rangePhiS = rangePhiS;

    m_rangeXB = KinematicRange(0., 1.);
}

DVMPKinematicRanges::DVMPKinematicRanges(const DVMPKinematicRanges &other) :
        PARTONS::BaseObject(other) {

    m_rangeY = other.m_rangeY;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangeT = other.m_rangeT;
    m_rangePhi = other.m_rangePhi;
    m_rangePhiS = other.m_rangePhiS;

    m_rangeXB = other.m_rangeXB;
}

DVMPKinematicRanges::~DVMPKinematicRanges() {
}

std::string DVMPKinematicRanges::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "DVMP kinematic range y: " << m_rangeY.toString() << '\n';
    formatter << "DVMP kinematic range Q2: " << m_rangeQ2.toString() << '\n';
    formatter << "DVMP kinematic range t: " << m_rangeT.toString() << '\n';
    formatter << "DVMP kinematic range phi: " << m_rangePhi.toString() << '\n';
    formatter << "DVMP kinematic range phiS: " << m_rangePhiS.toString() << '\n'
            << '\n';
    formatter << "DVMP kinematic range xB: " << m_rangeXB.toString();

    return formatter.str();
}

bool DVMPKinematicRanges::inRange(
        const ExperimentalConditions& experimentalConditions,
        const DVMPKinematic& obsKin) const {

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

DVMPKinematicRanges DVMPKinematicRanges::getDVMPKinematicRangesfromTask(
        const MonteCarloTask &task) {

    DVMPKinematicRanges result;

    const ElemUtils::Parameters &data =
            task.getKinematicRange().getParameters();

    result.setRangeY(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVMPKinematicRanges", data,
                            DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_Y, 2)));

    result.setRangeQ2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVMPKinematicRanges", data,
                            DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_Q2, 2)));

    result.setRangeT(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVMPKinematicRanges", data,
                            DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_T, 2)));

    result.setRangePhi(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVMPKinematicRanges", data,
                            DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_PHI, 2,
                            true)));

    result.setRangePhiS(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DVMPKinematicRanges", data,
                            DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_PHIS, 2,
                            true)));

    if (data.isAvailable(DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_XB)) {

        result.setRangeXB(
                KinematicRange(
                        ContainerUtils::findAndParseVectorDouble(
                                "DVMPKinematicRanges", data,
                                DVMPKinematicRanges::DVMP_KINEMATIC_RANGE_XB,
                                2)));
    } else {
        result.setRangeY(KinematicRange(0., 1.));
    }

    return result;
}

const KinematicRange &DVMPKinematicRanges::getRangeY() const {
    return m_rangeY;
}

void DVMPKinematicRanges::setRangeY(const KinematicRange &rangeY) {
    m_rangeY = rangeY;
}

const KinematicRange &DVMPKinematicRanges::getRangeQ2() const {
    return m_rangeQ2;
}

void DVMPKinematicRanges::setRangeQ2(const KinematicRange &rangeQ2) {
    m_rangeQ2 = rangeQ2;
}

const KinematicRange &DVMPKinematicRanges::getRangeT() const {
    return m_rangeT;
}

void DVMPKinematicRanges::setRangeT(const KinematicRange &rangeT) {
    m_rangeT = rangeT;
}

const KinematicRange &DVMPKinematicRanges::getRangePhi() const {
    return m_rangePhi;
}

void DVMPKinematicRanges::setRangePhi(const KinematicRange &rangePhi) {
    m_rangePhi = rangePhi;
}

const KinematicRange &DVMPKinematicRanges::getRangePhiS() const {
    return m_rangePhiS;
}

void DVMPKinematicRanges::setRangePhiS(const KinematicRange &rangePhiS) {
    m_rangePhiS = rangePhiS;
}

const KinematicRange &DVMPKinematicRanges::getRangeXB() const {
    return m_rangeXB;
}

void DVMPKinematicRanges::setRangeXB(const KinematicRange &rangeXB) {
    m_rangeXB = rangeXB;
}

} /* namespace EPIC */

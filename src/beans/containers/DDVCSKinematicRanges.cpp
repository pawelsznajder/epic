/*
 * DDVCSKinematicRanges.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/DDVCSKinematicRanges.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/automation/BaseObjectData.h>

#include "../../../include/automation/MonteCarloTask.h"
#include "../../../include/beans/containers/ContainerUtils.h"
#include "../../../include/beans/containers/DDVCSKinematic.h"
#include "../../../include/beans/containers/ExperimentalConditions.h"

namespace EPIC {

const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Y = "range_y";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2 = "range_Q2";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_T = "range_t";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2Prim =
        "range_Q2Prim";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHI = "range_phi";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIS =
        "range_phiS";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIL =
        "range_phiL";
const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_THETAL =
        "range_thetaL";

const std::string DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_XB = "range_xB";

DDVCSKinematicRanges::DDVCSKinematicRanges() :
        PARTONS::BaseObject("DDVCSKinematicRanges") {
}

DDVCSKinematicRanges::DDVCSKinematicRanges(const KinematicRange &rangeY,
        const KinematicRange &rangeQ2, const KinematicRange &rangeT,
        const KinematicRange &rangeQ2Prim, const KinematicRange &rangePhi,
        const KinematicRange &rangePhiS, const KinematicRange &rangePhiL,
        const KinematicRange &rangeThetaL) :
        PARTONS::BaseObject("DDVCSKinematicRanges") {

    m_rangeY = rangeY;
    m_rangeQ2 = rangeQ2;
    m_rangeT = rangeT;
    m_rangeQ2Prim = rangeQ2Prim;
    m_rangePhi = rangePhi;
    m_rangePhiS = rangePhiS;
    m_rangePhiL = rangePhiL;
    m_rangeThetaL = rangeThetaL;

    m_rangeXB = KinematicRange(0., 1.);
}

DDVCSKinematicRanges::DDVCSKinematicRanges(const DDVCSKinematicRanges &other) :
        PARTONS::BaseObject(other) {

    m_rangeY = other.m_rangeY;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangeT = other.m_rangeT;
    m_rangeQ2Prim = other.m_rangeQ2Prim;
    m_rangePhi = other.m_rangePhi;
    m_rangePhiS = other.m_rangePhiS;
    m_rangePhiL = other.m_rangePhiL;
    m_rangeThetaL = other.m_rangeThetaL;

    m_rangeXB = other.m_rangeXB;
}

DDVCSKinematicRanges::~DDVCSKinematicRanges() {
}

std::string DDVCSKinematicRanges::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << "DDVCS kinematic range y: " << m_rangeY.toString() << '\n';
    formatter << "DDVCS kinematic range Q2: " << m_rangeQ2.toString() << '\n';
    formatter << "DDVCS kinematic range t: " << m_rangeT.toString() << '\n';
    formatter << "DDVCS kinematic range Q'2: " << m_rangeQ2Prim.toString()
            << '\n';
    formatter << "DDVCS kinematic range phi: " << m_rangePhi.toString() << '\n';
    formatter << "DDVCS kinematic range phiS: " << m_rangePhiS.toString()
            << '\n';
    formatter << "DDVCS kinematic range phiL: " << m_rangePhiL.toString()
            << '\n';
    formatter << "DDVCS kinematic range thetaL: " << m_rangeThetaL.toString()
            << '\n' << '\n';

    formatter << "DDVCS kinematic range xB: " << m_rangeXB.toString();

    return formatter.str();
}

bool DDVCSKinematicRanges::inRange(
        const ExperimentalConditions& experimentalConditions,
        const DDVCSKinematic& obsKin) const {

    if (!m_rangeY.inRange(obsKin.getY()))
        return false;
    if (!m_rangeQ2.inRange(obsKin.getQ2()))
        return false;
    if (!m_rangeT.inRange(obsKin.getT()))
        return false;
    if (!m_rangeQ2Prim.inRange(obsKin.getQ2Prim()))
        return false;
    if (!m_rangePhi.inRange(obsKin.getPhi()))
        return false;
    if (!m_rangePhiS.inRange(obsKin.getPhiS()))
        return false;
    if (!m_rangePhiL.inRange(obsKin.getPhiL()))
        return false;
    if (!m_rangeThetaL.inRange(obsKin.getThetaL()))
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

DDVCSKinematicRanges DDVCSKinematicRanges::getDDVCSKinematicRangesfromTask(
        const MonteCarloTask &task) {

    DDVCSKinematicRanges result;

    const ElemUtils::Parameters &data =
            task.getKinematicRange().getParameters();

    result.setRangeY(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Y, 2)));

    result.setRangeQ2(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2,
                            2)));

    result.setRangeT(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_T, 2)));

    result.setRangeQ2Prim(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_Q2Prim,
                            2)));

    result.setRangePhi(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHI, 2,
                            true)));

    result.setRangePhiS(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIS, 2,
                            true)));

    result.setRangePhiL(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_PHIL, 2,
                            true)));

    result.setRangeThetaL(
            KinematicRange(
                    ContainerUtils::findAndParseVectorDouble(
                            "DDVCSKinematicRanges", data,
                            DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_THETAL,
                            2, true)));

    if (data.isAvailable(DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_XB)) {

        result.setRangeXB(
                KinematicRange(
                        ContainerUtils::findAndParseVectorDouble(
                                "DDVCSKinematicRanges", data,
                                DDVCSKinematicRanges::DDVCS_KINEMATIC_RANGE_XB,
                                2)));
    } else {
        result.setRangeXB(KinematicRange(0., 1.));
    }

    return result;
}

const KinematicRange &DDVCSKinematicRanges::getRangeY() const {
    return m_rangeY;
}

void DDVCSKinematicRanges::setRangeY(const KinematicRange &rangeY) {
    m_rangeY = rangeY;
}

const KinematicRange &DDVCSKinematicRanges::getRangeQ2() const {
    return m_rangeQ2;
}

void DDVCSKinematicRanges::setRangeQ2(const KinematicRange &rangeQ2) {
    m_rangeQ2 = rangeQ2;
}

const KinematicRange &DDVCSKinematicRanges::getRangeT() const {
    return m_rangeT;
}

void DDVCSKinematicRanges::setRangeT(const KinematicRange &rangeT) {
    m_rangeT = rangeT;
}

const KinematicRange &DDVCSKinematicRanges::getRangeQ2Prim() const {
    return m_rangeQ2Prim;
}

void DDVCSKinematicRanges::setRangeQ2Prim(const KinematicRange &rangeQ2Prim) {
    m_rangeQ2Prim = rangeQ2Prim;
}

const KinematicRange &DDVCSKinematicRanges::getRangePhi() const {
    return m_rangePhi;
}

void DDVCSKinematicRanges::setRangePhi(const KinematicRange &rangePhi) {
    m_rangePhi = rangePhi;
}

const KinematicRange &DDVCSKinematicRanges::getRangePhiS() const {
    return m_rangePhiS;
}

void DDVCSKinematicRanges::setRangePhiS(const KinematicRange &rangePhiS) {
    m_rangePhiS = rangePhiS;
}

const KinematicRange &DDVCSKinematicRanges::getRangePhiL() const {
    return m_rangePhiL;
}

void DDVCSKinematicRanges::setRangePhiL(const KinematicRange &rangePhiL) {
    m_rangePhiL = rangePhiL;
}

const KinematicRange &DDVCSKinematicRanges::getRangeThetaL() const {
    return m_rangeThetaL;
}

void DDVCSKinematicRanges::setRangeThetaL(const KinematicRange &rangeThetaL) {
    m_rangeThetaL = rangeThetaL;
}

const KinematicRange &DDVCSKinematicRanges::getRangeXB() const {
    return m_rangeXB;
}

void DDVCSKinematicRanges::setRangeXB(const KinematicRange &rangeXB) {
    m_rangeXB = rangeXB;
}

} /* namespace EPIC */

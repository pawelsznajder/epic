/*
 * DVMPKinematicRanges.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_DVMPKINEMATICRANGES_H_
#define BEANS_DVMPKINEMATICRANGES_H_

#include <string>

#include "KinematicRange.h"

namespace EPIC {
class DVMPKinematic;
class ExperimentalConditions;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class DVMPKinematicRanges
 *
 * @brief Container to store single kinematic ranges for DVMP.
 *
 * This class acts as a container to store kinematic range for DVMP.
 */
class DVMPKinematicRanges: public PARTONS::BaseObject {

public:

    static const std::string DVMP_KINEMATIC_RANGE_Y; ///< Key to set y kinematic range.
    static const std::string DVMP_KINEMATIC_RANGE_Q2; ///< Key to set Q2 kinematic range.
    static const std::string DVMP_KINEMATIC_RANGE_T; ///< Key to set t kinematic range.
    static const std::string DVMP_KINEMATIC_RANGE_PHI; ///< Key to set phi kinematic range.
    static const std::string DVMP_KINEMATIC_RANGE_PHIS; ///< Key to set phiS kinematic range.

    static const std::string DVMP_KINEMATIC_RANGE_XB; ///< Key to set xB kinematic range.

    /**
     * Default constructor.
     */
    DVMPKinematicRanges();

    /**
     * Assignment constructor.
     */
    DVMPKinematicRanges(const KinematicRange &rangeY, const KinematicRange &rangeQ2,
            const KinematicRange &rangeT,
            const KinematicRange &rangePhi, const KinematicRange &rangePhiS);

    /**
     * Copy constructor.
     */
    DVMPKinematicRanges(const DVMPKinematicRanges &other);

    /**
     * Destructor.
     */
    virtual ~DVMPKinematicRanges();

    virtual std::string toString() const;

    bool inRange(const ExperimentalConditions& experimentalConditions,
            const DVMPKinematic& obsKin) const;

    /**
     * Build from task.
     */
    static DVMPKinematicRanges getDVMPKinematicRangesfromTask(
            const MonteCarloTask &task);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get kinematic range y.
     */
    const KinematicRange &getRangeY() const;

    /**
     * Set kinematic range y.
     */
    void setRangeY(const KinematicRange &rangeY);

    /**
     * Get kinematic range Q2.
     */
    const KinematicRange &getRangeQ2() const;

    /**
     * Set kinematic range Q2.
     */
    void setRangeQ2(const KinematicRange &rangeQ2);

    /**
     * Get kinematic range t.
     */
    const KinematicRange &getRangeT() const;

    /**
     * Set kinematic range t.
     */
    void setRangeT(const KinematicRange &rangeT);

    /**
     * Get kinematic range phi.
     */
    const KinematicRange &getRangePhi() const;

    /**
     * Set kinematic range phi.
     */
    void setRangePhi(const KinematicRange &rangePhi);

    /**
     * Get kinematic range phiS.
     */
    const KinematicRange &getRangePhiS() const;

    /**
     * Set kinematic range phiS.
     */
    void setRangePhiS(const KinematicRange &rangePhiS);

    /**
     * Get kinematic range xB.
     */
    const KinematicRange &getRangeXB() const;

    /**
     * Set kinematic range xB.
     */
    void setRangeXB(const KinematicRange &rangeXB);

private:

    KinematicRange m_rangeY;   ///< Kinematic range y.
    KinematicRange m_rangeQ2;  ///< Kinematic range Q2.
    KinematicRange m_rangeT;   ///< Kinematic range t.
    KinematicRange m_rangePhi; ///< Kinematic range phi.
    KinematicRange m_rangePhiS; ///< Kinematic range phiS.

    KinematicRange m_rangeXB;  ///< Kinematic range xB.
};

} /* namespace EPIC */

#endif /* BEANS_DVMPKINEMATICRANGES_H_ */

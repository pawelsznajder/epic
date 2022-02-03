/*
 * TCSKinematicRanges.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_DDVCSKINEMATICRANGES_H_
#define BEANS_DDVCSKINEMATICRANGES_H_

#include <string>

#include "KinematicRange.h"

namespace EPIC {
class DDVCSKinematic;
class ExperimentalConditions;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class TCSKinematicRanges
 *
 * @brief Container to store single kinematic ranges for TCS.
 *
 * This class acts as a container to store kinematic range for TCS.
 */
class DDVCSKinematicRanges: public PARTONS::BaseObject {

public:

    static const std::string DDVCS_KINEMATIC_RANGE_Y; ///< Key to set y kinematic range.
    static const std::string DDVCS_KINEMATIC_RANGE_Q2; ///< Key to set Q2 kinematic range.
    static const std::string DDVCS_KINEMATIC_RANGE_T; ///< Key to set t kinematic range.
    static const std::string DDVCS_KINEMATIC_RANGE_Q2Prim; ///< Key to set Q'2 kinematic range.
    static const std::string DDVCS_KINEMATIC_RANGE_PHI; ///< Key to set phi kinematic range.
    static const std::string DDVCS_KINEMATIC_RANGE_PHIS; ///< Key to set phiS kinematic range.
    static const std::string DDVCS_KINEMATIC_RANGE_THETA; ///< Key to set theta kinematic range.

    /**
     * Default constructor.
     */
    DDVCSKinematicRanges();

    /**
     * Assignment constructor.
     */
    DDVCSKinematicRanges(const KinematicRange &rangeY,
            const KinematicRange &rangeQ2, const KinematicRange &rangeT,
            const KinematicRange &rangeQ2Prim, const KinematicRange &rangePhi,
            const KinematicRange &rangePhiS, const KinematicRange &rangeTheta);

    /**
     * Copy constructor.
     */
    DDVCSKinematicRanges(const DDVCSKinematicRanges &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSKinematicRanges();

    virtual std::string toString() const;

    bool inRange(const ExperimentalConditions& experimentalConditions,
            const DDVCSKinematic& obsKin) const;

    /**
     * Build from task.
     */
    static DDVCSKinematicRanges fromTask(const MonteCarloTask &task);

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
     * Get kinematic range Q'2.
     */
    const KinematicRange &getRangeQ2Prim() const;

    /**
     * Set kinematic range Q'2.
     */
    void setRangeQ2Prim(const KinematicRange &rangeQ2Prim);

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
     * Get kinematic range theta.
     */
    const KinematicRange &getRangeTheta() const;

    /**
     * Set kinematic range theta.
     */
    void setRangeTheta(const KinematicRange &rangeTheta);

private:

    KinematicRange m_rangeY;   ///< Kinematic range y.
    KinematicRange m_rangeQ2;  ///< Kinematic range Q2.
    KinematicRange m_rangeT;   ///< Kinematic range t.
    KinematicRange m_rangeQ2Prim;  ///< Kinematic range Q'2.
    KinematicRange m_rangePhi; ///< Kinematic range phi.
    KinematicRange m_rangePhiS; ///< Kinematic range phiS.
    KinematicRange m_rangeTheta; ///< Kinematic range theta.
};

} /* namespace EPIC */

#endif /* BEANS_DDVCSKINEMATICRANGES_H_ */

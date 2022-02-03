/*
 * DVCSKinematicRanges.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_DVCSKINEMATICRANGES_H_
#define BEANS_DVCSKINEMATICRANGES_H_

#include <string>

#include "KinematicRange.h"

namespace EPIC {
class DVCSKinematic;
class ExperimentalConditions;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class DVCSKinematicRanges
 *
 * @brief Container to store single kinematic ranges for DVCS.
 *
 * This class acts as a container to store kinematic range for DVCS.
 */
class DVCSKinematicRanges: public PARTONS::BaseObject {

public:

    static const std::string DVCS_KINEMATIC_RANGE_XB; ///< Key to set xB kinematic range.
    static const std::string DVCS_KINEMATIC_RANGE_T; ///< Key to set t kinematic range.
    static const std::string DVCS_KINEMATIC_RANGE_Q2; ///< Key to set Q2 kinematic range.
    static const std::string DVCS_KINEMATIC_RANGE_PHI; ///< Key to set phi kinematic range.
    static const std::string DVCS_KINEMATIC_RANGE_PHIS; ///< Key to set phi kinematic range.
    static const std::string DVCS_KINEMATIC_RANGE_Y; ///< Key to set y kinematic range.

    /**
     * Default constructor.
     */
    DVCSKinematicRanges();

    /**
     * Assignment constructor.
     */
    DVCSKinematicRanges(const KinematicRange &rangeXb,
            const KinematicRange &rangeT, const KinematicRange &rangeQ2,
            const KinematicRange &rangePhi, const KinematicRange &rangePhiS);

    /**
     * Assignment constructor.
     */
    DVCSKinematicRanges(const KinematicRange &rangeXb,
            const KinematicRange &rangeT, const KinematicRange &rangeQ2,
            const KinematicRange &rangePhi, const KinematicRange &rangePhiS,
            const KinematicRange &rangeY);

    /**
     * Copy constructor.
     */
    DVCSKinematicRanges(const DVCSKinematicRanges &other);

    /**
     * Destructor.
     */
    virtual ~DVCSKinematicRanges();

    virtual std::string toString() const;

    bool inRange(const ExperimentalConditions& experimentalConditions,
            const DVCSKinematic& obsKin) const;

    /**
     * Build from task.
     */
    static DVCSKinematicRanges fromTask(const MonteCarloTask &task);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get kinematic range xB.
     */
    const KinematicRange &getRangeXb() const;

    /**
     * Set kinematic range xB.
     */
    void setRangeXb(const KinematicRange &rangeXb);

    /**
     * Get kinematic range t.
     */
    const KinematicRange &getRangeT() const;

    /**
     * Set kinematic range t.
     */
    void setRangeT(const KinematicRange &rangeT);

    /**
     * Get kinematic range Q2.
     */
    const KinematicRange &getRangeQ2() const;

    /**
     * Set kinematic range Q2.
     */
    void setRangeQ2(const KinematicRange &rangeQ2);

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
     * Get kinematic range y.
     */
    const KinematicRange &getRangeY() const;

    /**
     * Set kinematic range y.
     */
    void setRangeY(const KinematicRange &rangeY);

private:

    KinematicRange m_rangeXb;  ///< Kinematic range xB.
    KinematicRange m_rangeT;   ///< Kinematic range t.
    KinematicRange m_rangeQ2;  ///< Kinematic range Q2.
    KinematicRange m_rangePhi; ///< Kinematic range phi.
    KinematicRange m_rangePhiS; ///< Kinematic range phiS.

    KinematicRange m_rangeY;   ///< Kinematic range y.
};

} /* namespace EPIC */

#endif /* BEANS_DVCSKINEMATICRANGES_H_ */

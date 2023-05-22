/*
 * TCSKinematicRanges.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_TCSKINEMATICRANGES_H_
#define BEANS_TCSKINEMATICRANGES_H_

#include "DDVCSKinematicRanges.h"

namespace EPIC {
class TCSKinematic;
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
class TCSKinematicRanges: public DDVCSKinematicRanges {

public:

    /**
     * Default constructor.
     */
    TCSKinematicRanges();

    /**
     * Assignment constructor.
     */
    TCSKinematicRanges(const KinematicRange &rangeY,
            const KinematicRange &rangeQ2, const KinematicRange &rangeT,
            const KinematicRange &rangeQ2Prim, const KinematicRange &rangePhiS,
            const KinematicRange &rangePhiL, const KinematicRange &rangeThetaL);

    /**
     * Copy constructor.
     */
    TCSKinematicRanges(const TCSKinematicRanges &other);

    /**
     * Destructor.
     */
    virtual ~TCSKinematicRanges();

    virtual std::string toString() const;

    bool inRange(const ExperimentalConditions& experimentalConditions,
            const TCSKinematic& obsKin) const;

    /**
     * Build from task.
     */
    static TCSKinematicRanges getTCSKinematicRangesfromTask(const MonteCarloTask &task);

    bool inRange(const ExperimentalConditions& experimentalConditions,
            const DDVCSKinematic& obsKin) const = delete;
    static DDVCSKinematicRanges getDDVCSKinematicRangesfromTask(const MonteCarloTask &task) = delete;
    const KinematicRange &getRangePhi() const = delete;
    void setRangePhi(const KinematicRange &rangePhi) = delete;

};

} /* namespace EPIC */

#endif /* BEANS_TCSKINEMATICRANGES_H_ */

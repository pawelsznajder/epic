/*
 * DVCSKinematicRanges.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_DVCSKINEMATICRANGES_H_
#define BEANS_DVCSKINEMATICRANGES_H_

#include "DDVCSKinematicRanges.h"

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
class DVCSKinematicRanges: public DDVCSKinematicRanges {

public:

    /**
     * Default constructor.
     */
    DVCSKinematicRanges();

    /**
     * Assignment constructor.
     */
    DVCSKinematicRanges(const KinematicRange &rangeY,
            const KinematicRange &rangeQ2, const KinematicRange &rangeT,
            const KinematicRange &rangePhi, const KinematicRange &rangePhiS);

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
    static DVCSKinematicRanges getDVCSKinematicRangesfromTask(
            const MonteCarloTask &task);

    bool inRange(const ExperimentalConditions& experimentalConditions,
            const DDVCSKinematic& obsKin) const = delete;
    static DDVCSKinematicRanges getDDVCSKinematicRangesfromTask(
            const MonteCarloTask &task) = delete;
    const KinematicRange &getRangeQ2Prim() const = delete;
    void setRangeQ2Prim(const KinematicRange &rangeQ2Prim) = delete;
    const KinematicRange &getRangePhiL() const = delete;
    void setRangePhiL(const KinematicRange &rangePhiL) = delete;
    const KinematicRange &getRangeThetaL() const = delete;
    void setRangeThetaL(const KinematicRange &rangeThetaL) = delete;

};

} /* namespace EPIC */

#endif /* BEANS_DVCSKINEMATICRANGES_H_ */

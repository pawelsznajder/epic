/*
 * GAM2KinematicModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_GAM2_GAM2KINEMATICMODULE_H_
#define MODULES_KINEMATIC_GAM2_GAM2KINEMATICMODULE_H_

#include <string>

#include "../../../beans/containers/GAM2KinematicRanges.h"
#include "../../../beans/containers/GAM2Kinematic.h"
#include "../KinematicModule.h"

namespace EPIC {

/**
 * @class GAM2KinematicModule
 *
 * @brief Kinematic module for GAM2.
 *
 * This class defines kinematic module for GAM2 process.
 */
class GAM2KinematicModule: public KinematicModule<GAM2KinematicRanges, GAM2Kinematic> {

public:
    static const std::string GAM2_KINEMATIC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    GAM2KinematicModule(const std::string &className);

    /**
     * Copy constructor.
     */
    GAM2KinematicModule(const GAM2KinematicModule &other);

    /**
     * Destructor.
     */
    virtual ~GAM2KinematicModule();

    virtual bool runTest() const;

    virtual std::vector<KinematicRange> getKinematicRanges(const ExperimentalConditions &conditions, const GAM2KinematicRanges& ranges);

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const GAM2Kinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const GAM2Kinematic &kin) = 0;

protected:
    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_GAM2_GAM2KINEMATICMODULE_H_ */

/*
 * DVMPKinematicModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_DVMP_DVMPKINEMATICMODULE_H_
#define MODULES_KINEMATIC_DVMP_DVMPKINEMATICMODULE_H_

#include <string>

#include "../../../beans/containers/DVMPKinematicRanges.h"
#include "../../../beans/containers/DVMPKinematic.h"
#include "../KinematicModule.h"

namespace EPIC {

/**
 * @class DVMPKinematicModule
 *
 * @brief Kinematic module for DVMP.
 *
 * This class defines kinematic module for DVMP process.
 */
class DVMPKinematicModule: public KinematicModule<DVMPKinematicRanges, DVMPKinematic> {

public:
    static const std::string DVMP_KINEMATIC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    DVMPKinematicModule(const std::string &className);

    /**
     * Copy constructor.
     */
    DVMPKinematicModule(const DVMPKinematicModule &other);

    /**
     * Destructor.
     */
    virtual ~DVMPKinematicModule();

    virtual bool runTest() const;

    virtual std::vector<KinematicRange> getKinematicRanges(const ExperimentalConditions &conditions, const DVMPKinematicRanges& ranges);

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DVMPKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DVMPKinematic &kin) = 0;

protected:
    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_DVMP_DVMPKINEMATICMODULE_H_ */

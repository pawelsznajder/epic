/*
 * DVCSKinematicModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_DVCS_DVCSKINEMATICMODULE_H_
#define MODULES_KINEMATIC_DVCS_DVCSKINEMATICMODULE_H_

#include <string>

#include "../../../beans/containers/DVCSKinematicRanges.h"
#include "../../../beans/containers/DVCSKinematic.h"
#include "../KinematicModule.h"

namespace EPIC {

/**
 * @class DVCSKinematicModule
 *
 * @brief Kinematic module for DVCS.
 *
 * This class defines kinematic module for DVCS process.
 */
class DVCSKinematicModule: public KinematicModule<DVCSKinematicRanges, DVCSKinematic> {

public:
    static const std::string DVCS_KINEMATIC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    DVCSKinematicModule(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSKinematicModule(const DVCSKinematicModule &other);

    /**
     * Destructor.
     */
    virtual ~DVCSKinematicModule();

    virtual bool runTest() const;

    virtual std::vector<KinematicRange> getKinematicRanges(const ExperimentalConditions &conditions, const DVCSKinematicRanges& ranges);

    virtual bool checkIfValid(const ExperimentalConditions &conditions,
            const DVCSKinematic &kin);

    virtual Event evaluate(const ExperimentalConditions &conditions,
            const DVCSKinematic &kin) = 0;

protected:
    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_KINEMATIC_DVCS_DVCSKINEMATICMODULE_H_ */

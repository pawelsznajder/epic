/*
 * DVMPRCModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DVMP_DVMPRCMODULE_H_
#define MODULES_RADIATIVECORRECTIONS_DVMP_DVMPRCMODULE_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DVMPKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "../RCModule.h"

namespace EPIC {

/**
 * @class DVMPRCModule
 *
 * @brief Radiative corrections module for DVMP.
 *
 * This class defines radiative corrections module for DVMP process.
 */
class DVMPRCModule: public RCModule<DVMPKinematic> {

public:

    static const std::string DVMP_RC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    DVMPRCModule(const std::string &className);

    /**
     * Copy constructor.
     */
    DVMPRCModule(const DVMPRCModule &other);

    /**
     * Destructor.
     */
    virtual ~DVMPRCModule();

    virtual bool runTest() const;

    virtual std::tuple<double, ExperimentalConditions, DVMPKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DVMPKinematic& obsKin,
            const std::vector<double>& par) const = 0;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DVMP_DVMPRCMODULE_H_ */

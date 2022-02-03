/*
 * DVCSRCModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCMODULE_H_
#define MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCMODULE_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DVCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "../RCModule.h"

namespace EPIC {

/**
 * @class DVCSRCModule
 *
 * @brief Radiative corrections module for DVCS.
 *
 * This class defines radiative corrections module for DVCS process.
 */
class DVCSRCModule: public RCModule<DVCSKinematic> {

public:

    static const std::string DVCS_RC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    DVCSRCModule(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSRCModule(const DVCSRCModule &other);

    /**
     * Destructor.
     */
    virtual ~DVCSRCModule();

    virtual bool runTest() const;

    virtual std::tuple<double, ExperimentalConditions, DVCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DVCSKinematic& obsKin,
            const std::vector<double>& par) const = 0;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCMODULE_H_ */

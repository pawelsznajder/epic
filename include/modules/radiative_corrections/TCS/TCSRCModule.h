/*
 * TCSRCModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RADIATIVECORRECTIONS_TCS_TCSRCMODULE_H_
#define MODULES_RADIATIVECORRECTIONS_TCS_TCSRCMODULE_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/TCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "../RCModule.h"

namespace EPIC {

/**
 * @class TCSRCModule
 *
 * @brief Radiative corrections module for TCS.
 *
 * This class defines radiative corrections module for TCS process.
 */
class TCSRCModule: public RCModule<TCSKinematic> {

public:

    static const std::string TCS_RC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    TCSRCModule(const std::string &className);

    /**
     * Copy constructor.
     */
    TCSRCModule(const TCSRCModule &other);

    /**
     * Destructor.
     */
    virtual ~TCSRCModule();

    virtual bool runTest() const;

    virtual std::tuple<double, ExperimentalConditions, TCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const TCSKinematic& obsKin,
            const std::vector<double>& par) const = 0;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_TCS_TCSRCMODULE_H_ */

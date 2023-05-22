/*
 * DDVCSRCModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DDVCS_DDVCSRCMODULE_H_
#define MODULES_RADIATIVECORRECTIONS_DDVCS_DDVCSRCMODULE_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DDVCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "../RCModule.h"

namespace EPIC {

/**
 * @class DDVCSRCModule
 *
 * @brief Radiative corrections module for DDVCS.
 *
 * This class defines radiative corrections module for DDVCS process.
 */
class DDVCSRCModule: public RCModule<DDVCSKinematic> {

public:

    static const std::string DDVCS_RC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    DDVCSRCModule(const std::string &className);

    /**
     * Copy constructor.
     */
    DDVCSRCModule(const DDVCSRCModule &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSRCModule();

    virtual bool runTest() const;

    virtual std::tuple<double, ExperimentalConditions, DDVCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DDVCSKinematic& obsKin,
            const std::vector<double>& par) const = 0;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DDVCS_DDVCSRCMODULE_H_ */

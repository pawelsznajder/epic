/*
 * GAM2RCModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RADIATIVECORRECTIONS_GAM2_GAM2RCMODULE_H_
#define MODULES_RADIATIVECORRECTIONS_GAM2_GAM2RCMODULE_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/GAM2Kinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "../RCModule.h"

namespace EPIC {

/**
 * @class GAM2RCModule
 *
 * @brief Radiative corrections module for GAM2.
 *
 * This class defines radiative corrections module for GAM2 process.
 */
class GAM2RCModule: public RCModule<GAM2Kinematic> {

public:

    static const std::string GAM2_RC_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

    /**
     * Constructor.
     */
    GAM2RCModule(const std::string &className);

    /**
     * Copy constructor.
     */
    GAM2RCModule(const GAM2RCModule &other);

    /**
     * Destructor.
     */
    virtual ~GAM2RCModule();

    virtual bool runTest() const;

    virtual std::tuple<double, ExperimentalConditions, GAM2Kinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const GAM2Kinematic& obsKin,
            const std::vector<double>& par) const = 0;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_GAM2_GAM2RCMODULE_H_ */

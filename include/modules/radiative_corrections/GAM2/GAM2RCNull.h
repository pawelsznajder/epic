/*
 * GAM2RCNull.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_GAM2_GAM2RCNULL_H_
#define MODULES_RADIATIVECORRECTIONS_GAM2_GAM2RCNULL_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/GAM2Kinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "GAM2RCModule.h"

namespace EPIC {

/**
 * @class GAM2RCNull
 *
 * @brief Radiative correction module for GAM2 process giving no corrections.
 *
 * This class defines radiative correction module for GAM2 process. The module gives no corrections.
 */
class GAM2RCNull: public GAM2RCModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    GAM2RCNull(const std::string &className);

    /**
     * Copy constructor.
     */
    GAM2RCNull(const GAM2RCNull &other);

    /**
     * Destructor.
     */
    virtual ~GAM2RCNull();

    virtual GAM2RCNull *clone() const;

    virtual std::tuple<double, ExperimentalConditions, GAM2Kinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const GAM2Kinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_GAM2_GAM2RCNULL_H_ */

/*
 * DVMPRCNull.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DVMP_DVMPRCNULL_H_
#define MODULES_RADIATIVECORRECTIONS_DVMP_DVMPRCNULL_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DVMPKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "DVMPRCModule.h"

namespace EPIC {

/**
 * @class DVMPRCNull
 *
 * @brief Radiative correction module for DVMP process giving no corrections.
 *
 * This class defines radiative correction module for DVMP process. The module gives no corrections.
 */
class DVMPRCNull: public DVMPRCModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DVMPRCNull(const std::string &className);

    /**
     * Copy constructor.
     */
    DVMPRCNull(const DVMPRCNull &other);

    /**
     * Destructor.
     */
    virtual ~DVMPRCNull();

    virtual DVMPRCNull *clone() const;

    virtual std::tuple<double, ExperimentalConditions, DVMPKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DVMPKinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DVMP_DVMPRCNULL_H_ */

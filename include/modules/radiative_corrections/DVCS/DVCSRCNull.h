/*
 * DVCSRCNull.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCNULL_H_
#define MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCNULL_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DVCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "DVCSRCModule.h"

namespace EPIC {

/**
 * @class DVCSRCNull
 *
 * @brief Radiative correction module for DVCS process giving no corrections.
 *
 * This class defines radiative correction module for DVCS process. The module gives no corrections.
 */
class DVCSRCNull: public DVCSRCModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DVCSRCNull(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSRCNull(const DVCSRCNull &other);

    /**
     * Destructor.
     */
    virtual ~DVCSRCNull();

    virtual DVCSRCNull *clone() const;

    virtual std::tuple<double, ExperimentalConditions, DVCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DVCSKinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCNULL_H_ */

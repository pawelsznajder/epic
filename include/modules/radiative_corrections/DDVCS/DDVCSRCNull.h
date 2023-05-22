/*
 * DDVCSRCNull.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DDVCS_DDVCSRCNULL_H_
#define MODULES_RADIATIVECORRECTIONS_DDVCS_DDVCSRCNULL_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DDVCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "DDVCSRCModule.h"

namespace EPIC {

/**
 * @class DDVCSRCNull
 *
 * @brief Radiative correction module for DDVCS process giving no corrections.
 *
 * This class defines radiative correction module for DDVCS process. The module gives no corrections.
 */
class DDVCSRCNull: public DDVCSRCModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DDVCSRCNull(const std::string &className);

    /**
     * Copy constructor.
     */
    DDVCSRCNull(const DDVCSRCNull &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSRCNull();

    virtual DDVCSRCNull *clone() const;

    virtual std::tuple<double, ExperimentalConditions, DDVCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DDVCSKinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DDVCS_DDVCSRCNULL_H_ */

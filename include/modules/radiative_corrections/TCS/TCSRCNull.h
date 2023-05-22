/*
 * TCSRCNull.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_TCS_TCSRCNULL_H_
#define MODULES_RADIATIVECORRECTIONS_TCS_TCSRCNULL_H_

#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/TCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "TCSRCModule.h"

namespace EPIC {

/**
 * @class TCSRCNull
 *
 * @brief Radiative correction module for TCS process giving no corrections.
 *
 * This class defines radiative correction module for TCS process. The module gives no corrections.
 */
class TCSRCNull: public TCSRCModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    TCSRCNull(const std::string &className);

    /**
     * Copy constructor.
     */
    TCSRCNull(const TCSRCNull &other);

    /**
     * Destructor.
     */
    virtual ~TCSRCNull();

    virtual TCSRCNull *clone() const;

    virtual std::tuple<double, ExperimentalConditions, TCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const TCSKinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_TCS_TCSRCNULL_H_ */

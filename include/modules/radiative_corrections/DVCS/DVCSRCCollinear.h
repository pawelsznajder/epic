/*
 * DVCSRCCollinear.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCCOLLINEAR_H_
#define MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCCOLLINEAR_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <tuple>
#include <vector>

#include "../../../beans/containers/DVCSKinematic.h"
#include "../../../beans/containers/ExperimentalConditions.h"
#include "DVCSRCModule.h"

namespace EPIC {

/**
 * @class DVCSRCCollinear
 *
 * @brief Radiative correction module for DVCS process using collinear approximation.
 *
 * This class defines radiative correction module for DVCS process. The corrections
 * are evaluated using the collinear approximation.
 */
class DVCSRCCollinear: public DVCSRCModule {

public:

    static const std::string PARAMETER_NAME_EPSILON; ///< Key to set DVCSRCCollinear::m_epsilon.

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DVCSRCCollinear(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSRCCollinear(const DVCSRCCollinear &other);

    /**
     * Destructor.
     */
    virtual ~DVCSRCCollinear();

    virtual DVCSRCCollinear *clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::tuple<double, ExperimentalConditions, DVCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DVCSKinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;

private:

    double m_epsilon;	///< Epsilon near z = 1 point.
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCCOLLINEAR_H_ */

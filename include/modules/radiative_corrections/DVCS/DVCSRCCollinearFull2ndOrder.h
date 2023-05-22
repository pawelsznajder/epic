/*
 * DVCSRCCollinearFull2ndOrder.h
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCCOLLINEARFULL2NDORDER_H_
#define MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCCOLLINEARFULL2NDORDER_H_

#include "DVCSRCCollinear.h"

namespace EPIC {

/**
 * @class DVCSRCCollinearFull2ndOrder
 *
 * @brief Radiative correction module for DVCS process using collinear approximation.
 *
 * This class defines radiative correction module for DVCS process. The corrections
 * are evaluated using the collinear approximation.
 *
 * Emission of single ISR photon or single FSR photon (1st order) or two ISR photons or two FSR photons or ISR+FSR pair (2nd order) are implemented.
 * In the case of two ISR/FSR photons both of them are combined in the output into a single photon record.
 */
class DVCSRCCollinearFull2ndOrder: public DVCSRCCollinear {

public:

    static const unsigned int classId; ///< Unique ID to automatically register
                                       /// the class in the registry.

    /**
     * Constructor.
     */
    DVCSRCCollinearFull2ndOrder(const std::string &className);

    /**
     * Copy constructor.
     */
    DVCSRCCollinearFull2ndOrder(const DVCSRCCollinearFull2ndOrder &other);

    /**
     * Destructor.
     */
    virtual ~DVCSRCCollinearFull2ndOrder();

    virtual DVCSRCCollinearFull2ndOrder* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::tuple<double, ExperimentalConditions, DVCSKinematic> evaluate(
            const ExperimentalConditions& experimentalConditions,
            const DVCSKinematic& obsKin, const std::vector<double>& par) const;

    virtual void updateEvent(Event& event,
            const std::vector<double>& par) const;
};

} /* namespace EPIC */

#endif /* MODULES_RADIATIVECORRECTIONS_DVCS_DVCSRCCOLLINEARFULL2NDORDER_H_ */

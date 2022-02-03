/*
 * DVCSRCNull.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/radiative_corrections/DVCS/DVCSRCNull.h"

#include <partons/BaseObjectRegistry.h>

#include "../../../../include/modules/radiative_corrections/RCModule.h"

namespace EPIC {

const unsigned int DVCSRCNull::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSRCNull("DVCSRCNull"));

DVCSRCNull::DVCSRCNull(const std::string &className) :
        DVCSRCModule(className) {
}

DVCSRCNull::DVCSRCNull(const DVCSRCNull &other) :
        DVCSRCModule(other) {
}

DVCSRCNull::~DVCSRCNull() {
}

DVCSRCNull *DVCSRCNull::clone() const {
    return new DVCSRCNull(*this);
}

std::tuple<double, ExperimentalConditions, DVCSKinematic> DVCSRCNull::evaluate(
        const ExperimentalConditions& experimentalConditions,
        const DVCSKinematic& obsKin, const std::vector<double>& par) const {

    checkParameters(par);

    return std::make_tuple(1., experimentalConditions, obsKin);
}

void DVCSRCNull::updateEvent(Event& event,
        const std::vector<double>& par) const {
    checkParameters(par);
}

} /* namespace EPIC */

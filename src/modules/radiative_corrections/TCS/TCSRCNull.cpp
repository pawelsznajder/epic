/*
 * TCSRCNull.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/radiative_corrections/TCS/TCSRCNull.h"

#include <partons/BaseObjectRegistry.h>

#include "../../../../include/modules/radiative_corrections/RCModule.h"

namespace EPIC {

const unsigned int TCSRCNull::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSRCNull("TCSRCNull"));

TCSRCNull::TCSRCNull(const std::string &className) :
        TCSRCModule(className) {
}

TCSRCNull::TCSRCNull(const TCSRCNull &other) :
        TCSRCModule(other) {
}

TCSRCNull::~TCSRCNull() {
}

TCSRCNull *TCSRCNull::clone() const {
    return new TCSRCNull(*this);
}

std::tuple<double, ExperimentalConditions, TCSKinematic> TCSRCNull::evaluate(
        const ExperimentalConditions& experimentalConditions,
        const TCSKinematic& obsKin, const std::vector<double>& par) const {

    checkParameters(par);

    return std::make_tuple(1., experimentalConditions, obsKin);
}

void TCSRCNull::updateEvent(Event& event,
        const std::vector<double>& par) const {
    checkParameters(par);
}

} /* namespace EPIC */

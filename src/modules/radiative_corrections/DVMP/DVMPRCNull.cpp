/*
 * DVMPRCNull.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/radiative_corrections/DVMP/DVMPRCNull.h"

#include <partons/BaseObjectRegistry.h>

#include "../../../../include/modules/radiative_corrections/RCModule.h"

namespace EPIC {

const unsigned int DVMPRCNull::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPRCNull("DVMPRCNull"));

DVMPRCNull::DVMPRCNull(const std::string &className) :
        DVMPRCModule(className) {
}

DVMPRCNull::DVMPRCNull(const DVMPRCNull &other) :
        DVMPRCModule(other) {
}

DVMPRCNull::~DVMPRCNull() {
}

DVMPRCNull *DVMPRCNull::clone() const {
    return new DVMPRCNull(*this);
}

std::tuple<double, ExperimentalConditions, DVMPKinematic> DVMPRCNull::evaluate(
        const ExperimentalConditions& experimentalConditions,
        const DVMPKinematic& obsKin, const std::vector<double>& par) const {

    checkParameters(par);

    return std::make_tuple(1., experimentalConditions, obsKin);
}

void DVMPRCNull::updateEvent(Event& event,
        const std::vector<double>& par) const {
    checkParameters(par);
}

} /* namespace EPIC */

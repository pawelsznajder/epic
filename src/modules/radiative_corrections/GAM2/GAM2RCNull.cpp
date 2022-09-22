/*
 * GAM2RCNull.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/radiative_corrections/GAM2/GAM2RCNull.h"

#include <partons/BaseObjectRegistry.h>

#include "../../../../include/modules/radiative_corrections/RCModule.h"

namespace EPIC {

const unsigned int GAM2RCNull::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2RCNull("GAM2RCNull"));

GAM2RCNull::GAM2RCNull(const std::string &className) :
        GAM2RCModule(className) {
}

GAM2RCNull::GAM2RCNull(const GAM2RCNull &other) :
        GAM2RCModule(other) {
}

GAM2RCNull::~GAM2RCNull() {
}

GAM2RCNull *GAM2RCNull::clone() const {
    return new GAM2RCNull(*this);
}

std::tuple<double, ExperimentalConditions, GAM2Kinematic> GAM2RCNull::evaluate(
        const ExperimentalConditions& experimentalConditions,
        const GAM2Kinematic& obsKin, const std::vector<double>& par) const {

    checkParameters(par);

    return std::make_tuple(1., experimentalConditions, obsKin);
}

void GAM2RCNull::updateEvent(Event& event,
        const std::vector<double>& par) const {
    checkParameters(par);
}

} /* namespace EPIC */

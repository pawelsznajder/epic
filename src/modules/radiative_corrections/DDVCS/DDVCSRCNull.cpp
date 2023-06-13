/*
 * DDVCSRCNull.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../../include/modules/radiative_corrections/DDVCS/DDVCSRCNull.h"

#include <partons/BaseObjectRegistry.h>

#include "../../../../include/modules/radiative_corrections/RCModule.h"

namespace EPIC {

const unsigned int DDVCSRCNull::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSRCNull("DDVCSRCNull"));

DDVCSRCNull::DDVCSRCNull(const std::string &className) :
        DDVCSRCModule(className) {
}

DDVCSRCNull::DDVCSRCNull(const DDVCSRCNull &other) :
        DDVCSRCModule(other) {
}

DDVCSRCNull::~DDVCSRCNull() {
}

DDVCSRCNull *DDVCSRCNull::clone() const {
    return new DDVCSRCNull(*this);
}

std::tuple<double, ExperimentalConditions, DDVCSKinematic> DDVCSRCNull::evaluate(
        const ExperimentalConditions& experimentalConditions,
        const DDVCSKinematic& obsKin, const std::vector<double>& par) const {

    checkParameters(par);

    return std::make_tuple(1., experimentalConditions, obsKin);
}

void DDVCSRCNull::updateEvent(Event& event,
        const std::vector<double>& par) const {
    checkParameters(par);
}

} /* namespace EPIC */

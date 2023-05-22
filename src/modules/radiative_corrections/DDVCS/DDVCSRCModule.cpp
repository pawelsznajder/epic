/*
 * DDVCSRCModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/radiative_corrections/DDVCS/DDVCSRCModule.h"

#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string DDVCSRCModule::DDVCS_RC_MODULE_CLASS_NAME = "DDVCSRCModule";

DDVCSRCModule::DDVCSRCModule(const std::string &className) :
        RCModule<DDVCSKinematic>(className, PARTONS::ChannelType::DDVCS) {
}

DDVCSRCModule::DDVCSRCModule(const DDVCSRCModule &other) :
        RCModule<DDVCSKinematic>(other) {
}

DDVCSRCModule::~DDVCSRCModule() {
}

bool DDVCSRCModule::runTest() const {

    // TODO
    return true;
}

void DDVCSRCModule::initModule() {
}

void DDVCSRCModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

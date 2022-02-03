/*
 * TCSRCModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/radiative_corrections/TCS/TCSRCModule.h"

#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string TCSRCModule::TCS_RC_MODULE_CLASS_NAME = "TCSRCModule";

TCSRCModule::TCSRCModule(const std::string &className) :
        RCModule<DDVCSKinematic>(className, PARTONS::ChannelType::TCS) {
}

TCSRCModule::TCSRCModule(const TCSRCModule &other) :
        RCModule<DDVCSKinematic>(other) {
}

TCSRCModule::~TCSRCModule() {
}

bool TCSRCModule::runTest() const {

    // TODO
    return true;
}

void TCSRCModule::initModule() {
}

void TCSRCModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

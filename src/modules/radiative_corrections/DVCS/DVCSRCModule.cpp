/*
 * DVCSRCModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/radiative_corrections/DVCS/DVCSRCModule.h"

#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string DVCSRCModule::DVCS_RC_MODULE_CLASS_NAME = "DVCSRCModule";

DVCSRCModule::DVCSRCModule(const std::string &className) :
        RCModule<DVCSKinematic>(className, PARTONS::ChannelType::DVCS) {
}

DVCSRCModule::DVCSRCModule(const DVCSRCModule &other) :
        RCModule<DVCSKinematic>(other) {
}

DVCSRCModule::~DVCSRCModule() {
}

bool DVCSRCModule::runTest() const {

    // TODO
    return true;
}

void DVCSRCModule::initModule() {
}

void DVCSRCModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

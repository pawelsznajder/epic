/*
 * GAM2RCModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/radiative_corrections/GAM2/GAM2RCModule.h"

#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string GAM2RCModule::GAM2_RC_MODULE_CLASS_NAME = "GAM2RCModule";

GAM2RCModule::GAM2RCModule(const std::string &className) :
        RCModule<GAM2Kinematic>(className, PARTONS::ChannelType::GAM2) {
}

GAM2RCModule::GAM2RCModule(const GAM2RCModule &other) :
        RCModule<GAM2Kinematic>(other) {
}

GAM2RCModule::~GAM2RCModule() {
}

bool GAM2RCModule::runTest() const {

    // TODO
    return true;
}

void GAM2RCModule::initModule() {
}

void GAM2RCModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

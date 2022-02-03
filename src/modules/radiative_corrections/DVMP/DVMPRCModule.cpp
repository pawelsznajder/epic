/*
 * DVMPRCModule.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../../include/modules/radiative_corrections/DVMP/DVMPRCModule.h"

#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string DVMPRCModule::DVMP_RC_MODULE_CLASS_NAME = "DVMPRCModule";

DVMPRCModule::DVMPRCModule(const std::string &className) :
        RCModule<DVMPKinematic>(className, PARTONS::ChannelType::DVMP) {
}

DVMPRCModule::DVMPRCModule(const DVMPRCModule &other) :
        RCModule<DVMPKinematic>(other) {
}

DVMPRCModule::~DVMPRCModule() {
}

bool DVMPRCModule::runTest() const {

    // TODO
    return true;
}

void DVMPRCModule::initModule() {
}

void DVMPRCModule::isModuleWellConfigured() {
}

} /* namespace EPIC */

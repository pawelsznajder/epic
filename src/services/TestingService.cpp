/*
 * TestingService.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/services/TestingService.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <iterator>
#include <memory>

#include "../../include/Epic.h"
#include "../../include/managers/ModuleObjectFactory.h"
#include "../../include/modules/EpicModuleObject.h"

namespace EPIC {

TestingService *TestingService::m_pInstance = nullptr;

TestingService *TestingService::getInstance() {

  if (m_pInstance == nullptr) {
    m_pInstance = new TestingService();
  }

  return m_pInstance;
}

TestingService::TestingService() : PARTONS::BaseObject("TestingService") {}

TestingService::~TestingService() {
  if (m_pInstance != nullptr) {
    delete m_pInstance;
    m_pInstance = nullptr;
  }
}

bool TestingService::runTest(bool stopIfFailed) const {

  // status
  bool status = true;

  // get factory
  EPIC::ModuleObjectFactory *pTestModuleObjectFactory =
      EPIC::Epic::getInstance()->getModuleObjectFactory();

  // iterator
  std::vector<std::string>::const_iterator it;

  // run
  for (it = m_modules.begin(); it != m_modules.end(); it++) {

    // get
    std::shared_ptr<EpicModuleObject> module =
        pTestModuleObjectFactory->newTestModuleObject(*it);

    // check
    bool thisStatus = module->runTest();

    // status
    info(__func__, ElemUtils::Formatter()
                       << "Testing: name: " << (*it)
                       << " result: " << ((thisStatus) ? ("OK") : ("FAILED")));

    // if stopIfFailed and failed exit
    if ((!thisStatus) && stopIfFailed)
      return false;

    // add
    status &= thisStatus;
  }

  // return
  return status;
}

void TestingService::registerModuleObject(const std::string &moduleName) {

  // iterator
  std::vector<std::string>::const_iterator it;

  // run
  bool isNotThere = true;

  for (it = m_modules.begin(); it != m_modules.end(); it++) {
    if ((*it) == moduleName) {

      isNotThere = false;
      break;
    }
  }

  // add
  if (isNotThere) {
    m_modules.push_back(moduleName);
  }
}

} /* namespace EPIC */

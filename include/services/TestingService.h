/*
 * TestingService.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_TESTINGSERVICE_H_
#define SERVICES_TESTINGSERVICE_H_

#include <partons/BaseObject.h>
#include <string>
#include <vector>

namespace EPIC {

/**
 * @class TestingService
 *
 * @brief Service used for automatic test of modules.
 *
 * This class is used to automatically test module. It is intended to have a
 * unique instance accessed
 * trough TestingService::getInstance() method.
 */
class TestingService : public PARTONS::BaseObject {

public:
  /**
   * Share a unique pointer of this class
   */
  static TestingService *getInstance();

  /**
   * Destructor.
   */
  virtual ~TestingService();

  /**
   * Perform test.
   */
  bool runTest(bool stopIfFailed = true) const;

  /**
   * Register test module.
   */
  void registerModuleObject(const std::string &moduleName);

private:
  /**
   * Default constructor.
   */
  TestingService();

  /**
   * Private pointer of this class for a unique instance.
   */
  static TestingService *m_pInstance;

  /**
   * Modules.
   */
  std::vector<std::string> m_modules;
};

} /* namespace EPIC */

#endif /* SERVICES_TESTINGSERVICE_H_ */

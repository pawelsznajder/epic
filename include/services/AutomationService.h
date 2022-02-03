/*
 * AutomationService.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef SERVICES_AUTOMATIONSERVICE_H_
#define SERVICES_AUTOMATIONSERVICE_H_

#include <memory>
#include <partons/services/automation/Qt4XMLValidator.h>
#include <string>

#include "../automation/MonteCarloParser.h"

namespace EPIC {
class MonteCarloScenario;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class AutomationService
 *
 * @brief Service used for automatic test of modules.
 *
 * This class is used to automatically test module. It is intended to have a
 * unique instance accessed
 * trough AutomationService::getInstance() method.
 */
class AutomationService : public PARTONS::BaseObject {

public:
  static const std::string
      PROPERTY_NAME_XML_SCHEMA_FILE_PATH; ///< Property name XML schema.

  /**
   * Share a unique pointer of this class
   */
  static AutomationService *getInstance();

  /**
   * Destructor.
   */
  virtual ~AutomationService();

  /**
   * Validate and parse XML file to MonteCarloScenario object.
   */
  std::shared_ptr<MonteCarloScenario>
  parseXMLFile(const std::string &xmlFilePath);

  /**
   * Execute given scenario.
   */
  void playScenario(std::shared_ptr<MonteCarloScenario> pScenario) const;

private:
  /**
   * Default constructor.
   */
  AutomationService();

  /**
   * Private pointer of this class for a unique instance.
   */
  static AutomationService *m_pInstance;

  PARTONS::Qt4XMLValidator m_validator; ///< Validator.
  MonteCarloParser m_parser;            ///< Parser.
};

} /* namespace EPIC */

#endif /* SERVICES_AUTOMATIONSERVICE_H_ */

/*
 * AutomationService.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../include/services/AutomationService.h"

#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../include/automation/MonteCarloScenario.h"

namespace EPIC {

const std::string AutomationService::PROPERTY_NAME_XML_SCHEMA_FILE_PATH =
    "xml.schema.file.path";

AutomationService *AutomationService::m_pInstance = nullptr;

AutomationService *AutomationService::getInstance() {

  if (m_pInstance == nullptr) {
    m_pInstance = new AutomationService();
  }

  return m_pInstance;
}

AutomationService::AutomationService()
    : PARTONS::BaseObject("AutomationService") {}

AutomationService::~AutomationService() {

  if (m_pInstance != nullptr) {
    delete m_pInstance;
    m_pInstance = nullptr;
  }
}

std::shared_ptr<MonteCarloScenario>
AutomationService::parseXMLFile(const std::string &xmlFilePath) {

  std::string file = ElemUtils::FileUtils::read(xmlFilePath);

  bool isValid = m_validator.isValidXMLDocument(
      ElemUtils::FileUtils::read(
          ElemUtils::PropertiesManager::getInstance()->getString(
              AutomationService::PROPERTY_NAME_XML_SCHEMA_FILE_PATH)),
      file);

  if (isValid) {
    info(__func__, ElemUtils::Formatter() << "File " << xmlFilePath
                                          << " is valid");
  } else {
    throw ElemUtils::CustomException(
        getClassName(), __func__,
        ElemUtils::Formatter() << "File " << xmlFilePath << " is not valid");
  }

  return m_parser.parseScenario(file);
}

void AutomationService::playScenario(
    std::shared_ptr<MonteCarloScenario> pScenario) const {}

} /* namespace EPIC */

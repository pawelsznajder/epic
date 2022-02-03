/*
 * MonteCarloParser.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../include/automation/MonteCarloParser.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../include/automation/MonteCarloScenario.h"
#include "../../include/automation/MonteCarloTask.h"

namespace EPIC {

const std::string MonteCarloParser::SCENARIO_NODE_NAME = "scenario";
const std::string MonteCarloParser::SCENARIO_ATTRIBUTE_DESCRIPTION_NAME =
    "description";
const std::string MonteCarloParser::SCENARIO_ATTRIBUTE_DATE_NAME = "date";

const std::string MonteCarloParser::TASK_NODE_NAME = "task";
const std::string MonteCarloParser::TASK_ATRIBUTE_SERVICE_NAME = "service";
const std::string MonteCarloParser::TASK_ATRIBUTE_METHOD_NAME = "method";

const std::string MonteCarloParser::GENERAL_CONFIGURATION_NODE_NAME =
    "general_configuration";
const std::string MonteCarloParser::KINEMATIC_RANGE_NODE_NAME =
    "kinematic_range";
const std::string MonteCarloParser::EXPERIMENTAL_CONDIDTIONS_NODE_NAME =
    "experimental_conditions";
const std::string MonteCarloParser::COMPUTATION_CONFIGURATION_NODE_NAME =
    "computation_configuration";
const std::string MonteCarloParser::GENERATOR_CONFIGURATION_NODE_NAME =
    "generator_configuration";
const std::string MonteCarloParser::KINEMATIC_CONFIGURATION_NODE_NAME =
    "kinematic_configuration";
const std::string MonteCarloParser::RC_CONFIGURATION_NODE_NAME =
    "rc_configuration";
const std::string MonteCarloParser::WRITER_CONFIGURATION_NODE_NAME =
    "writer_configuration";

const std::string MonteCarloParser::PARAM_NODE_NAME = "param";
const std::string MonteCarloParser::PARAM_ATTRIBUTE_NAME_NAME = "name";
const std::string MonteCarloParser::PARAM_ATTRIBUTE_VALUE_NAME = "value";

const std::string MonteCarloParser::MODULE_NODE_NAME = "module";
const std::string MonteCarloParser::MODULE_TYPE_ATTRIBUT_NAME = "type";
const std::string MonteCarloParser::MODULE_NAME_ATTRIBUT_NAME = "name";

MonteCarloParser::MonteCarloParser()
    : ElemUtils::XMLParser(), PARTONS::BaseObject("MonteCarloParser"),
      m_pScenario(nullptr), m_isNew(false) {}

MonteCarloParser::MonteCarloParser(const MonteCarloParser &other)
    : ElemUtils::XMLParser(other), PARTONS::BaseObject(other),
      m_pScenario(other.m_pScenario), m_isNew(other.m_isNew) {

  m_description = other.m_description;
  m_date = other.m_date;

  m_serviceName = other.m_serviceName;
  m_methodName = other.m_methodName;

  m_generalConfiguration = other.m_generalConfiguration;
  m_kinematicRange = other.m_kinematicRange;
  m_experimentalConditions = other.m_experimentalConditions;
  m_computationConfiguration = other.m_computationConfiguration;
  m_generatorConfiguration = other.m_generatorConfiguration;
  m_kinematicConfiguration = other.m_kinematicConfiguration;
  m_rcConfiguration = other.m_rcConfiguration;
  m_writerConfiguration = other.m_writerConfiguration;

  m_currentModuleHierarchy = other.m_currentModuleHierarchy;
}

MonteCarloParser::~MonteCarloParser() {}

std::shared_ptr<MonteCarloScenario>
MonteCarloParser::parseScenario(const std::string &xmlFile) {

  std::shared_ptr<MonteCarloScenario> pScenario =
      std::make_shared<MonteCarloScenario>();

  m_pScenario = pScenario;

  analyse(xmlFile);

  m_pScenario = nullptr;

  return pScenario;
}

std::string MonteCarloParser::toString() const {

  ElemUtils::Formatter formatter;

  formatter << "m_description: " << m_description << '\n';
  formatter << "m_date: " << m_date << '\n';
  formatter << "m_serviceName: " << m_serviceName << '\n';
  formatter << "m_methodName: " << m_methodName << '\n';

  formatter << "m_generalConfiguration:\n"
            << m_generalConfiguration.toString() << '\n';
  formatter << "m_kinematicRange:\n" << m_kinematicRange.toString() << '\n';
  formatter << "m_experimentalConditions:\n"
            << m_experimentalConditions.toString() << '\n';
  formatter << "m_computationConfiguration:\n"
            << m_computationConfiguration.toString() << '\n';
  formatter << "m_generatorConfiguration:\n"
            << m_generatorConfiguration.toString() << '\n';
  formatter << "m_kinematicConfiguration:\n"
            << m_kinematicConfiguration.toString() << '\n';
  formatter << "m_rcConfiguration:\n"
              << m_rcConfiguration.toString() << '\n';
  formatter << "m_writerConfiguration:\n"
            << m_writerConfiguration.toString() << '\n';

  return formatter.str();
}

void MonteCarloParser::startElement(const std::string &elementName,
                                    ElemUtils::Parameters attributes,
                                    const std::string &elementData) {

  // check
  if (m_pScenario == nullptr) {
    throw ElemUtils::CustomException(getClassName(), __func__,
                                     "Pointer to current scenario is null");
  }

  // if reached start element node is scenario
  // then retrieve its description attribute
  if (ElemUtils::StringUtils::equals(elementName,
                                     MonteCarloParser::SCENARIO_NODE_NAME)) {

    if (attributes.isAvailable(
            MonteCarloParser::SCENARIO_ATTRIBUTE_DESCRIPTION_NAME)) {
      m_description = attributes.getLastAvailable().getString();
    }

    if (attributes.isAvailable(
            MonteCarloParser::SCENARIO_ATTRIBUTE_DATE_NAME)) {
      m_date = attributes.getLastAvailable().getString();
    }
  }

  // if reached start element node is task
  // then retrieve its service, method and storeInDB attributes
  else if (ElemUtils::StringUtils::equals(elementName,
                                          MonteCarloParser::TASK_NODE_NAME)) {

    if (attributes.isAvailable(MonteCarloParser::TASK_ATRIBUTE_SERVICE_NAME)) {
      m_serviceName = attributes.getLastAvailable().getString();
    }

    if (attributes.isAvailable(MonteCarloParser::TASK_ATRIBUTE_METHOD_NAME)) {
      m_methodName = attributes.getLastAvailable().getString();
    }
  }

  // if reached start element node is general_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::GENERAL_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.push_back(&m_generalConfiguration);
  }

  // if reached start element node is kinematic_range
  else if (ElemUtils::StringUtils::equals(
               elementName, MonteCarloParser::KINEMATIC_RANGE_NODE_NAME)) {
    m_currentModuleHierarchy.push_back(&m_kinematicRange);
  }

  // if reached start element node is experimental_condition
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::EXPERIMENTAL_CONDIDTIONS_NODE_NAME)) {
    m_currentModuleHierarchy.push_back(&m_experimentalConditions);
  }

  // if reached start element node is computation_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::COMPUTATION_CONFIGURATION_NODE_NAME)) {

    m_currentModuleHierarchy.push_back(&m_computationConfiguration);
    m_isNew = true;
  }

  // if reached start element node is generator_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::GENERATOR_CONFIGURATION_NODE_NAME)) {

    m_currentModuleHierarchy.push_back(&m_generatorConfiguration);
    m_isNew = true;
  }

  // if reached start element node is kinematic_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::KINEMATIC_CONFIGURATION_NODE_NAME)) {

    m_currentModuleHierarchy.push_back(&m_kinematicConfiguration);
    m_isNew = true;
  }

  // if reached start element node is rc_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::RC_CONFIGURATION_NODE_NAME)) {

    m_currentModuleHierarchy.push_back(&m_rcConfiguration);
    m_isNew = true;
  }

  // if reached start element node is writer_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName, MonteCarloParser::WRITER_CONFIGURATION_NODE_NAME)) {

    m_currentModuleHierarchy.push_back(&m_writerConfiguration);
    m_isNew = true;
  }

  // if reached start element node is module
  else if (ElemUtils::StringUtils::equals(elementName,
                                          MonteCarloParser::MODULE_NODE_NAME)) {

    std::string moduleType;
    std::string moduleClassName;

    if (attributes.isAvailable(MonteCarloParser::MODULE_TYPE_ATTRIBUT_NAME)) {
      moduleType = attributes.getLastAvailable().getString();
    }

    if (attributes.isAvailable(MonteCarloParser::MODULE_NAME_ATTRIBUT_NAME)) {
      moduleClassName = attributes.getLastAvailable().getString();
    }

    if (m_isNew) {

      m_currentModuleHierarchy.back()->setModuleType(moduleType);
      m_currentModuleHierarchy.back()->setModuleClassName(moduleClassName);

      m_isNew = false;
    } else {

      m_currentModuleHierarchy.push_back(
          &m_currentModuleHierarchy.back()->addSubModule(moduleType,
                                                         moduleClassName));
    }
  }
}

void MonteCarloParser::emptyStartElement(const std::string &elementName,
                                         ElemUtils::Parameters attributes) {

  // check
  if (m_pScenario == nullptr) {
    throw ElemUtils::CustomException(getClassName(), __func__,
                                     "Pointer to current scenario is null");
  }

  // module
  if (ElemUtils::StringUtils::equals(elementName,
                                     MonteCarloParser::MODULE_NODE_NAME)) {

    std::string moduleType;
    std::string moduleClassName;

    if (attributes.isAvailable(MonteCarloParser::MODULE_TYPE_ATTRIBUT_NAME)) {
      moduleType = attributes.getLastAvailable().getString();
    }

    if (attributes.isAvailable(MonteCarloParser::MODULE_NAME_ATTRIBUT_NAME)) {
      moduleClassName = attributes.getLastAvailable().getString();
    }

    m_currentModuleHierarchy.back()->addSubModule(moduleType, moduleClassName);
  }

  // parameter
  else if (ElemUtils::StringUtils::equals(elementName,
                                          MonteCarloParser::PARAM_NODE_NAME)) {

    std::string paramName;
    std::string paramValue;

    if (attributes.isAvailable(MonteCarloParser::PARAM_ATTRIBUTE_NAME_NAME)) {
      paramName = attributes.getLastAvailable().getString();
    }

    if (attributes.isAvailable(MonteCarloParser::PARAM_ATTRIBUTE_VALUE_NAME)) {
      paramValue = attributes.getLastAvailable().getString();
    }

    m_currentModuleHierarchy.back()->addParameter(
        ElemUtils::Parameter(paramName, paramValue));
  }
}

void MonteCarloParser::endElement(const std::string &elementName) {

  // check
  if (m_pScenario == nullptr) {
    throw ElemUtils::CustomException(getClassName(), __func__,
                                     "Pointer to current scenario is null");
  }

  // if reached end element node is scenario
  if (ElemUtils::StringUtils::equals(elementName,
                                     MonteCarloParser::SCENARIO_NODE_NAME)) {

    m_pScenario->setDescription(m_description);
    m_pScenario->setDate(m_date);
  }

  // if reached end element node is task
  else if (ElemUtils::StringUtils::equals(elementName,
                                          MonteCarloParser::TASK_NODE_NAME)) {

    MonteCarloTask task;

    task.setServiceName(m_serviceName);
    task.setMethodName(m_methodName);

    task.setGeneralConfiguration(m_generalConfiguration);
    task.setKinematicRange(m_kinematicRange);
    task.setExperimentalConditions(m_experimentalConditions);
    task.setComputationConfiguration(m_computationConfiguration);
    task.setGeneratorConfiguration(m_generatorConfiguration);
    task.setKinematicConfiguration(m_kinematicConfiguration);
    task.setRCConfiguration(m_rcConfiguration);
    task.setWriterConfiguration(m_writerConfiguration);

    m_pScenario->addTask(task);
  }

  // if reached end element node is general_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::GENERAL_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is kinematic_range
  else if (ElemUtils::StringUtils::equals(
               elementName, MonteCarloParser::KINEMATIC_RANGE_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is experimental_condition
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::EXPERIMENTAL_CONDIDTIONS_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is computation_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::COMPUTATION_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is generator_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::GENERATOR_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is kinematic_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::KINEMATIC_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is rc_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName,
               MonteCarloParser::RC_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }


  // if reached end element node is writer_configuration
  else if (ElemUtils::StringUtils::equals(
               elementName, MonteCarloParser::WRITER_CONFIGURATION_NODE_NAME)) {
    m_currentModuleHierarchy.pop_back();
  }

  // if reached end element node is module
  else if (ElemUtils::StringUtils::equals(elementName,
                                          MonteCarloParser::MODULE_NODE_NAME)) {

    if (m_currentModuleHierarchy.size() > 1)
      m_currentModuleHierarchy.pop_back();
  }
}

} /* namespace EPIC */

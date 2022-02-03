/*
 * MonteCarloParser.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef AUTOMATION_MONTECARLOPARSER_H_
#define AUTOMATION_MONTECARLOPARSER_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/parser/XMLParser.h>
#include <memory>
#include <partons/beans/automation/BaseObjectData.h>
#include <string>
#include <vector>

namespace EPIC {
class MonteCarloScenario;
} /* namespace EPIC */

namespace EPIC {

class MonteCarloParser : public ElemUtils::XMLParser,
                         public PARTONS::BaseObject {

public:
  static const std::string SCENARIO_NODE_NAME; ///< Scenario node name.
  static const std::string SCENARIO_ATTRIBUTE_DESCRIPTION_NAME; ///< Scenario
                                                                /// attribute
  /// description
  /// name.
  static const std::string
      SCENARIO_ATTRIBUTE_DATE_NAME; ///< Scenario attribute date name.

  static const std::string TASK_NODE_NAME; ///< Task node name.
  static const std::string
      TASK_ATRIBUTE_SERVICE_NAME; ///< Task attribute service name.
  static const std::string
      TASK_ATRIBUTE_METHOD_NAME; ///<Task attribute method name.

  static const std::string GENERAL_CONFIGURATION_NODE_NAME; ///< Name of node in
  /// XML automation to
  /// set general
  /// configuration.
  static const std::string KINEMATIC_RANGE_NODE_NAME; ///< Name of node in XML
                                                      /// automation to set
  /// kinematic range.
  static const std::string EXPERIMENTAL_CONDIDTIONS_NODE_NAME; ///< Name of node
                                                               /// in XML
  /// automation to
  /// set
  /// experimental
  /// condition.
  static const std::string
      COMPUTATION_CONFIGURATION_NODE_NAME; ///< Name of node in XML automation
                                           /// to set computation configuration.
  static const std::string GENERATOR_CONFIGURATION_NODE_NAME; ///< Name of node
                                                              /// in XML
  /// automation to
  /// set event generator
  /// configuration.
  static const std::string KINEMATIC_CONFIGURATION_NODE_NAME; ///< Name of node
                                                              /// in XML
  /// automation to
  /// set kinematic
  /// configuration.
  static const std::string RC_CONFIGURATION_NODE_NAME; ///< Name of node
                                                               /// in XML
   /// automation to
   /// set kinematic
   /// configuration.
  static const std::string WRITER_CONFIGURATION_NODE_NAME; ///< Name of node in
                                                           /// XML automation to
  /// set writer
  /// configuration.

  static const std::string PARAM_NODE_NAME; ///< Parameter node name.
  static const std::string
      PARAM_ATTRIBUTE_NAME_NAME; ///< Parameter attribute name name.
  static const std::string
      PARAM_ATTRIBUTE_VALUE_NAME; ///< Parameter attribute value name.

  static const std::string MODULE_NODE_NAME; ///< Module node name.
  static const std::string
      MODULE_TYPE_ATTRIBUT_NAME; ///< Module attribute type name.
  static const std::string
      MODULE_NAME_ATTRIBUT_NAME; ///< Module attribute name name.

  /**
   * Default constructor.
   */
  MonteCarloParser();

  /**
   * Copy constructor.
   */
  MonteCarloParser(const MonteCarloParser &other);

  /**
   * Destructor.
   */
  virtual ~MonteCarloParser();

  virtual std::string toString() const;

  virtual void startElement(const std::string &elementName,
                            ElemUtils::Parameters attributes,
                            const std::string &elementData);
  virtual void emptyStartElement(const std::string &elementName,
                                 ElemUtils::Parameters attributes);
  virtual void endElement(const std::string &elementName);

  /**
   * Parse XML file.
   */
  std::shared_ptr<MonteCarloScenario> parseScenario(const std::string &xmlFile);

private:
  std::shared_ptr<MonteCarloScenario> m_pScenario;

  std::string m_description;
  std::string m_date;

  std::string m_serviceName;
  std::string m_methodName;

  PARTONS::BaseObjectData m_generalConfiguration;
  PARTONS::BaseObjectData m_kinematicRange;
  PARTONS::BaseObjectData m_experimentalConditions;
  PARTONS::BaseObjectData m_computationConfiguration;
  PARTONS::BaseObjectData m_generatorConfiguration;
  PARTONS::BaseObjectData m_kinematicConfiguration;
  PARTONS::BaseObjectData m_rcConfiguration;
  PARTONS::BaseObjectData m_writerConfiguration;

  std::vector<PARTONS::BaseObjectData *> m_currentModuleHierarchy;

  bool m_isNew;
};

} /* namespace EPIC */

#endif /* AUTOMATION_MONTECARLOPARSER_H_ */

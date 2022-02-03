/*
 * MonteCarloScenario.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef AUTOMATION_MONTECARLOSCENARIO_H_
#define AUTOMATION_MONTECARLOSCENARIO_H_

#include <string>
#include <vector>

#include "MonteCarloTask.h"

namespace EPIC {

/**
 * @class MonteCarloScenario
 *
 * @brief Container to store information from xml scenario.
 *
 * This class contains structured information store in a signle xml scenario.
 */
class MonteCarloScenario : public PARTONS::BaseObject {

public:
  /**
   * Default constructor.
   */
  MonteCarloScenario();

  /**
   * Copy constructor.
   * @param other Object to be copied.
   */
  MonteCarloScenario(const MonteCarloScenario &other);

  /**
   * Destructor.
   */
  virtual ~MonteCarloScenario();

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get description.
   */
  const std::string &getDescription() const;

  /**
   * Set description.
   */
  void setDescription(const std::string &description);

  /**
   * Get date.
   */
  const std::string &getDate() const;

  /**
   * Set date.
   */
  void setDate(const std::string &date);

  /**
   * Get tasks.
   */
  const std::vector<MonteCarloTask> &getTasks() const;

  /**
   * Set tasks.
   */
  void setTasks(const std::vector<MonteCarloTask> &tasks);

  /**
   * Add single task.
   */
  void addTask(const MonteCarloTask &task);

private:
  /**
   * Description of this scenario.
   */
  std::string m_description;

  /**
   * Date.
   */
  std::string m_date;

  /**
   * Vector containing tasks associated to this scenario.
   */
  std::vector<MonteCarloTask> m_tasks;
};

} /* namespace EPIC */

#endif /* AUTOMATION_MONTECARLOSCENARIO_H_ */

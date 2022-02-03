/*
 * MonteCarloScenario.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../include/automation/MonteCarloScenario.h"

namespace EPIC {

MonteCarloScenario::MonteCarloScenario()
    : PARTONS::BaseObject("MonteCarloScenario") {}

MonteCarloScenario::MonteCarloScenario(const MonteCarloScenario &other)
    : PARTONS::BaseObject(other) {

  m_description = other.m_description;
  m_tasks = other.m_tasks;
}

MonteCarloScenario::~MonteCarloScenario() {}

const std::string &MonteCarloScenario::getDescription() const {
  return m_description;
}

void MonteCarloScenario::setDescription(const std::string &description) {
  m_description = description;
}

const std::string &MonteCarloScenario::getDate() const { return m_date; }

void MonteCarloScenario::setDate(const std::string &date) { m_date = date; }

const std::vector<MonteCarloTask> &MonteCarloScenario::getTasks() const {
  return m_tasks;
}

void MonteCarloScenario::setTasks(const std::vector<MonteCarloTask> &tasks) {
  m_tasks = tasks;
}

void MonteCarloScenario::addTask(const MonteCarloTask &task) {
  m_tasks.push_back(task);
}

} /* namespace EPIC */

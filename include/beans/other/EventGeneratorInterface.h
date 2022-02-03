/*
 * EventGeneratorInterface.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_EVENTGENERATORINTERFACE_H_
#define BEANS_EVENTGENERATORINTERFACE_H_

#include <vector>

namespace EPIC {

/**
 * @class EventGeneratorInterface
 *
 * @brief Interface to EventGeneratorModule.
 *
 * This class is used to provide an interface to EventGeneratorModule.
 */
class EventGeneratorInterface {

public:
  /**
   * Default constructor.
   */
  EventGeneratorInterface();

  /**
   * Copy constructor.
   */
  EventGeneratorInterface(const EventGeneratorInterface &other);

  /**
   * Destructor.
   */
  virtual ~EventGeneratorInterface();

  /**
   * This function is used by EventGeneratorModule to probe event distribution
   * during the initialization step.
   */
  virtual double getEventDistribution(const std::vector<double> &kin) const = 0;
};

} /* namespace EPIC */

#endif /* BEANS_EVENTGENERATORINTERFACE_H_ */

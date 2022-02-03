/*
 * RandomNumberGSL.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RANDOM_GENERATOR_RANDOMNUMBERGSL_H_
#define MODULES_RANDOM_GENERATOR_RANDOMNUMBERGSL_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <gsl/gsl_rng.h>
#include <string>

#include "RandomNumberModule.h"

namespace EPIC {

/**
 * @class RandomNumberGSL
 *
 * @brief Generation of random numbers based on GSL.
 *
 * Module dedicated to the generation of random numbers using algorithms
 * available in GSL.
 */
class RandomNumberGSL : public RandomNumberModule {

public:
  static const std::string
      PARAMETER_NAME_ALGORITHM_NAME; ///< String used to set algorithm through
                                     /// RandomNumberGSL::configure().

  static const unsigned int classId; ///< Unique ID to automatically register
                                     /// the class in the registry.

  /**
   * Default constructor.
   */
  RandomNumberGSL(const std::string &className);

  /**
   * Copy constructor.
   */
  RandomNumberGSL(const RandomNumberGSL &other);

  /**
   * Destructor.
   */
  virtual ~RandomNumberGSL();

  virtual RandomNumberGSL *clone() const;
  virtual void configure(const ElemUtils::Parameters &parameters);

  virtual double diceFlat();
  virtual double diceNormal();

  /**
   * Get name of currently used algorithm.
   */
  std::string getAlgorithmName() const;

private:
  /**
   * Set generator.
   */
  void setGenerator(const gsl_rng_type *type);

  /**
   * Random generator.
   */
  gsl_rng *m_pGenerator;
};

} /* namespace EPIC */

#endif /* MODULES_RANDOM_GENERATOR_RANDOMNUMBERGSL_H_ */

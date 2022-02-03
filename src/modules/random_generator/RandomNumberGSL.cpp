/*
 * RandomNumberGSL.cpp
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../include/modules/random_generator/RandomNumberGSL.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_randist.h>
#include <partons/BaseObjectRegistry.h>

namespace EPIC {

const std::string RandomNumberGSL::PARAMETER_NAME_ALGORITHM_NAME =
    "algorithm_name";

const unsigned int RandomNumberGSL::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
        new RandomNumberGSL("RandomNumberGSL"));

RandomNumberGSL::RandomNumberGSL(const std::string &className)
    : RandomNumberModule(className), m_pGenerator(nullptr) {
  setGenerator(gsl_rng_default);
}

RandomNumberGSL::RandomNumberGSL(const RandomNumberGSL &other)
    : RandomNumberModule(other), m_pGenerator(nullptr) {
  setGenerator(other.m_pGenerator->type);
}

RandomNumberGSL::~RandomNumberGSL() {

  if (m_pGenerator != nullptr) {

    gsl_rng_free(m_pGenerator);
    m_pGenerator = nullptr;
  }
}

RandomNumberGSL *RandomNumberGSL::clone() const {
  return new RandomNumberGSL(*this);
}

void RandomNumberGSL::configure(const ElemUtils::Parameters &parameters) {

  // run for mother
  RandomNumberModule::configure(parameters);

  // look for algorithm name
  if (parameters.isAvailable(RandomNumberGSL::PARAMETER_NAME_ALGORITHM_NAME)) {

    // get name
    std::string name = parameters.getLastAvailable().getString();

    // look for associated type
    const gsl_rng_type *type = nullptr;

    // get availible types
    const gsl_rng_type **availTypes = gsl_rng_types_setup();

    // loop
    for (const gsl_rng_type **thisType = availTypes; *thisType != 0;
         thisType++) {
      if (std::string((*thisType)->name) == name) {

        type = (*thisType);
        break;
      }
    }

    // if not set
    if (type == nullptr) {
      throw ElemUtils::CustomException(getClassName(), __func__,
                                       ElemUtils::Formatter()
                                           << "Random number algorithm " << name
                                           << " is not available in your GSL");
    }

    // set
    setGenerator(type);

    // print status
    info(__func__, ElemUtils::Formatter() << "Random number algorithm set to: "
                                          << name);
  }
}

double RandomNumberGSL::diceFlat() { return gsl_rng_uniform(m_pGenerator); }

double RandomNumberGSL::diceNormal() {
  return gsl_ran_gaussian(m_pGenerator, 1.);
}

std::string RandomNumberGSL::getAlgorithmName() const {
  return std::string(gsl_rng_name(m_pGenerator));
}

void RandomNumberGSL::setGenerator(const gsl_rng_type *type) {

  // destroy if needed
  if (m_pGenerator != nullptr) {

    gsl_rng_free(m_pGenerator);
    m_pGenerator = nullptr;
  }

  // create
  m_pGenerator = gsl_rng_alloc(type);

  // seed
  gsl_rng_set(m_pGenerator, m_seed);
}

} /* namespace EPIC */

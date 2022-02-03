/*
 * RandomNumberModule.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/modules/random_generator/RandomNumberModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <partons/beans/channel/ChannelType.h>

#include "../../../include/Epic.h"
#include "../../../include/managers/RandomSeedManager.h"

namespace EPIC {

const std::string RandomNumberModule::RANDOM_NUMBER_MODULE_CLASS_NAME =
    "RandomNumberModule";

RandomNumberModule::RandomNumberModule(const std::string &className)
    : EpicModuleObject(className, PARTONS::ChannelType::UNDEFINED) {
}

RandomNumberModule::RandomNumberModule(const RandomNumberModule &other)
    : EpicModuleObject(other) {
}

RandomNumberModule::~RandomNumberModule() {}

bool RandomNumberModule::runTest() const {

  // TODO
  return true;
}

void RandomNumberModule::initModule() {}

void RandomNumberModule::isModuleWellConfigured() {}

double RandomNumberModule::diceFlat(double min, double max) {

  if (min >= max) {
    throw ElemUtils::CustomException("RandomGenerator", __func__,
                                     ElemUtils::Formatter()
                                         << "Illegal values of min = " << min
                                         << " and max = " << max);
  }

  return min + (max - min) * diceFlat();
}

double RandomNumberModule::diceNormal(double mean, double sigma) {

  if (sigma <= 0.) {
    throw ElemUtils::CustomException(
        "RandomGenerator", __func__,
        ElemUtils::Formatter() << "Illegal value of sigma = " << sigma);
  }

  return mean + sigma * diceNormal();
}

double RandomNumberModule::diceNormalFromFlat() {

  // from http://www.design.caltech.edu/erik/Misc/Gaussian.html

  double x1, x2, w;

  do {
    x1 = 2 * diceFlat() - 1.;
    x2 = 2 * diceFlat() - 1.;
    w = x1 * x1 + x2 * x2;
  } while (w >= 1.);

  w = sqrt((-2. * log(w)) / w);

  return x1 * w;
}

} /* namespace EPIC */

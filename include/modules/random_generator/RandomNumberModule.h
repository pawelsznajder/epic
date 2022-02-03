/*
 * RandomNumberModule.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_RANDOMGENERATOR_RANDOMNUMBERMODULE_H_
#define MODULES_RANDOMGENERATOR_RANDOMNUMBERMODULE_H_

#include <stddef.h>
#include <string>

#include "../EpicModuleObject.h"

namespace EPIC {

/**
 * @class RandomNumberModule
 *
 * @brief Definition of module for generation of random numbers.
 *
 * This class provides a definition of modules used to generate random numbers.
 */
class RandomNumberModule: public EpicModuleObject {

public:
	static const std::string RANDOM_NUMBER_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

	/**
	 * Constructor.
	 */
	RandomNumberModule(const std::string &className);

	/**
	 * Copy constructor.
	 */
	RandomNumberModule(const RandomNumberModule &other);

	/**
	 * Destructor.
	 */
	virtual ~RandomNumberModule();

	virtual bool runTest() const;

	/**
	 * Dice double precision number from flat distribution in range [0, 1)
	 */
	virtual double diceFlat() = 0;

	/**
	 * Dice double precision number from flat distribution in given range
	 * @param min Min
	 * @param max Max
	 */
	double diceFlat(double min, double max);

	/**
	 * Dice double precision number from normal distribution with mean = 0 and
	 * sigma = 1
	 */
	virtual double diceNormal() = 0;

	/**
	 * Dice double precision number from normal distribution with given mean and
	 * sigma
	 * @param mean Mean
	 * @param sigma Sigma
	 */
	double diceNormal(double mean, double sigma);

protected:
	virtual void initModule();
	virtual void isModuleWellConfigured();

	/**
	 * Generate normal from flat distribution.
	 * Useful if used method does not provide a genuine method for generation
	 * according to normal distribution.
	 */
	double diceNormalFromFlat();
};

} /* namespace EPIC */

#endif /* MODULES_RANDOMGENERATOR_RANDOMNUMBERMODULE_H_ */

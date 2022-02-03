/*
 * RandomSeedManager.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef ALGORITHMS_RANDOM_GENERATOR_SEEDMANAGER_H_
#define ALGORITHMS_RANDOM_GENERATOR_SEEDMANAGER_H_

#include <stddef.h>

namespace EPIC {

/**
 * @class RandomSeedManager
 *
 * @brief Manager for random number seeds.
 *
 * This class provides manager for random number seeds. It contains a counter.
 * With this manager two modules will not have the same seed.
 */
class RandomSeedManager {

public:
  /**
   * Destructor.
   */
  virtual ~RandomSeedManager();

  /**
   * Get seed.
   */
  size_t getSeed();

  /**
   * Set seed count.
   */
  void setSeedCount(size_t seedCount);

private:
  friend class Epic; ///< Only Epic can make an instance of this class.

  /**
   * Constructor.
   */
  RandomSeedManager();



  size_t m_seedCount; ///< Seed count.
};

} /* namespace EPIC */

#endif /* ALGORITHMS_RANDOM_GENERATOR_SEEDMANAGER_H_ */

/*
 * Epic.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef EPIC_H_
#define EPIC_H_

namespace EPIC {
class RandomSeedManager;
class ServiceObjectRegistry;
class ModuleObjectFactory;
} /* namespace EPIC */
namespace PARTONS {
class Partons;
} /* namespace PARTONS */

namespace EPIC {

/**
 * @class Epic
 *
 * @brief The main class of generator.
 *
 * This class is the main one for this software. It is responsible for the
 * initialization and program stopping,
 * as well as provides pointers to factories and managers.
 */
class Epic {

public:
  /**
   * Share a unique pointer of this class.
   */
  static Epic *getInstance();

  /**
   * Default destructor.
   */
  virtual ~Epic();

  /**
   * Initialize.
   * @param argc Number of arguments
   * @param argv Table of arguments
   */
  void init(int argc, char **argv);

  /**
   * Close.
   */
  void close();

  /**
   * Print version.
   */
  void printVersion() const;

  /**
   * Get pointer to ServiceObjectRegistry.
   */
  ServiceObjectRegistry *getServiceObjectRegistry() const;

  /**
   * Get pointer to ModuleObjectFactory.
   */
  ModuleObjectFactory *getModuleObjectFactory() const;

  /**
   * Get pointer to RandomSeedManager.
   */
  RandomSeedManager *getRandomSeedManager() const;

private:
  /**
   * Default constructor.
   */
  Epic();

  /**
   * Private pointer of this class for a unique instance.
   */
  static Epic *m_pInstance;

  /**
   * Pointer to PARTONS.
   */
  PARTONS::Partons *m_pPartons;

  /**
   * Pointer to ServiceObjectRegistry.
   */
  ServiceObjectRegistry *m_pServiceObjectRegistry;

  /**
   * Pointer to FitsModuleObjectFactory.
   */
  ModuleObjectFactory *m_pModuleObjectFactory;

  /**
   * Pointer to RandomSeedManager.
   */
  RandomSeedManager *m_pRandomSeedManager;
};

} /* namespace EPIC */

#endif /* EPIC_H_ */

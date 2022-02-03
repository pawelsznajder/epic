/*
 * ExperimentalConditions.h
 *
 *  Created on: Feb 9, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef BEANS_EXPERIMENTALCONDITIONS_H_
#define BEANS_EXPERIMENTALCONDITIONS_H_

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <string>

#include "../types/ParticleType.h"

namespace EPIC {
class MonteCarloTask;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class ExperimentalConditions
 *
 * @brief Container to store experimental conditions.
 *
 * This class acts as a container to store experimental conditions.
 */
class ExperimentalConditions : public PARTONS::BaseObject {

  static const std::string
      EXPERIMENTAL_CONDITION_LEPTON_ENERGY; ///< Key to set lepton energy.
  static const std::string
      EXPERIMENTAL_CONDITION_LEPTON_PARTICLE; ///< Key to set lepton particle.
  static const std::string
      EXPERIMENTAL_CONDITION_LEPTON_HELICITY; ///< Key to set lepton helicity.
  static const std::string
      EXPERIMENTAL_CONDITION_HADRON_ENERGY; ///< Key to set hadron energy.
  static const std::string
      EXPERIMENTAL_CONDITION_HADRON_PARTICLE; ///< Key to set hadron particle.
  static const std::string
      EXPERIMENTAL_CONDITION_HADRON_POLARISATION; ///< Key to set hadron
                                                  /// polarisation.

public:
  /**
   * Default constructor.
   */
  ExperimentalConditions();

  /**
   * Assignment constructor.
   */
  ExperimentalConditions(double leptonEnergy, ParticleType::Type leptonType,
                         int leptonHelicity, double hadronEnergy,
                         ParticleType::Type hadronType,
                         const NumA::Vector3D &hadronPolarisation);

  /**
   * Copy constructor.
   */
  ExperimentalConditions(const ExperimentalConditions &other);

  /**
   * Destructor.
   */
  virtual ~ExperimentalConditions();

  virtual std::string toString() const;

  /**
   * Build from task.
   */
  static ExperimentalConditions fromTask(const MonteCarloTask &task);

  /**
   * Get lepton energy fixed target equivalent.
   */
  double getLeptonEnergyFixedTargetEquivalent() const;

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get lepton energy.
   */
  double getLeptonEnergy() const;

  /**
   * Set lepton energy.
   */
  void setLeptonEnergy(double leptonEnergy);

  /**
   * Get lepton type.
   */
  ParticleType::Type getLeptonType() const;

  /**
   * Set lepton type.
   */
  void setLeptonType(ParticleType::Type leptonType);

  /**
   * Get lepton helicity.
   */
  int getLeptonHelicity() const;

  /**
   * Set lepton helicity.
   */
  void setLeptonHelicity(int leptonHelicity);

  /**
   * Get hadron energy.
   */
  double getHadronEnergy() const;

  /**
   * Set hadron energy.
   */
  void setHadronEnergy(double hadronEnergy);

  /**
   * Get hadron type.
   */
  ParticleType::Type getHadronType() const;

  /**
   * Set hadron type.
   */
  void setHadronType(ParticleType::Type hadronType);

  /**
   * Get hadron polarisation.
   */
  const NumA::Vector3D &getHadronPolarisation() const;

  /**
   * Set hadron polarisation.
   */
  void setHadronPolarisation(const NumA::Vector3D &hadronPolarisation);

private:
  double m_leptonEnergy;           ///< Lepton energy.
  ParticleType::Type m_leptonType; ///< Lepton type.
  int m_leptonHelicity;            ///< Lepton helicity.

  double m_hadronEnergy;               ///< Hadron energy.
  ParticleType::Type m_hadronType;     ///< Hadron type.
  NumA::Vector3D m_hadronPolarisation; ///< Hadron polarisation.
};

} /* namespace EPIC */

#endif /* BEANS_EXPERIMENTALCONDITIONS_H_ */

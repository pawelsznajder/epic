/**
 * @file Particle.h
 * @author Pawel Sznajder (NCBJ)
 * @date 20 June 2020
 * @version 1.0
 */

#ifndef BEANS_PARTICLE_H_
#define BEANS_PARTICLE_H_

#include <TLorentzVector.h>
#include <TVector3.h>
#include <string>

#include "../types/AxisType.h"
#include "../types/ParticleType.h"

namespace EPIC {

/**
 * @class Particle
 *
 * @brief Single particle.
 *
 * This class defines a single particle.
 */
class Particle : public PARTONS::BaseObject {

public:
  /**
   * Default constructor.
   */
  Particle();

  /**
   * Constructor with type.
   */
  Particle(ParticleType::Type type);

  /**
   * Constructor with four-vector.
   */
  Particle(ParticleType::Type type, const TLorentzVector &lv);

  /**
   * Constructor with type and momentum.
   * Mass according to the particle type.
   */
  Particle(ParticleType::Type type, const TVector3 &mom);

  /**
   * Constructor with type, direction and energy.
   * Mass according to the particle type.
   */
  Particle(ParticleType::Type type, const TVector3 &dir, double E);

  /**
   * Copy constructor.
   */
  Particle(const Particle &other);

  /**
   * Destructor.
   */
  virtual ~Particle();

  /**
   * Get energy evaluated from the associated four-momentum.
   */
  double getEnergy() const;

  /**
   * Get mass evaluated from the associated four-momentum.
   */
  double getMass() const;

  /**
   * Get momentum evaluated from the associated four-momentum.
   */
  TVector3 getMomentum() const;

  /**
   * Get pseudo-rapidity.
   */
  double getPseudoRapidity() const;

  /**
   * Rotate around X, Y or Z axis.
   * @param axis Rotate around this axis.
   * @param angle Rotation angle (in radians).
   */
  void rotate(AxisType::Type axis, double angle);

  /**
   * Rotate around given direction.
   * @param particle Direction.
   * @param angle Rotation angle (in radians).
   */
  void rotate(const TVector3 &dir, double angle);

  /**
   * Rotate around trajectory of given particle.
   * @param particle Reference particle.
   * @param angle Rotation angle (in radians).
   */
  void rotate(const Particle &particle, double angle);

  /**
   * Get angle between particle trajectory and given axis.
   * @param axis Axis.
   */
  double getAngle(AxisType::Type axis) const;

  /**
   * Get angle between particle trajectory and given direction.
   * @param particle Direction.
   */
  double getAngle(const TVector3 &dir) const;

  /**
   * Get angle between particle trajectory and other particle trajectory.
   * @param particle Other particle.
   */
  double getAngle(const Particle &particle) const;

  /**
   * Boost.
   * @param v Boost vector.
   */
  void boost(const TVector3 &v);

  /**
   * Boost to CMS of given particle.
   * @param particle Reference particle.
   */
  void boost(const Particle &particle);

  /**
   * Get string associated to the object of this class.
   */
  std::string toString() const;

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get four momentum.
   */
  const TLorentzVector &getFourMomentum() const;

  /**
   * Set four momentum.
   */
  void setFourMomentum(const TLorentzVector &lv);

  /**
   * Get type.
   */
  ParticleType::Type getType() const;

  /**
   * Set type.
   */
  void setType(ParticleType::Type type);

private:
  ParticleType::Type m_type; ///< Type.
  TLorentzVector m_lv;       ///< Four-momentum.
};
}

#endif /* BEANS_PARTICLE_H_ */

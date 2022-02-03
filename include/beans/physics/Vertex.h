/*
 * Vertex.h
 *
 *  Created on: Feb 10, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_VERTEX_H_
#define BEANS_VERTEX_H_

#include <memory>
#include <partons/BaseObject.h>
#include <string>
#include <vector>

namespace EPIC {
class Particle;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class Vertex
 *
 * @brief Vertex representation.
 *
 * This class represents a single vertex. It contains pointers to incoming and
 * outgoing particles.
 */
class Vertex : public PARTONS::BaseObject {

public:
  /**
   * Default constructor.
   */
  Vertex();

  /**
   * Copy constructor.
   */
  Vertex(const Vertex &other);

  /**
   * Destructor.
   */
  virtual ~Vertex();

  virtual std::string toString() const;

  /**
   * Add incoming particle.
   */
  void addParticleIn(const std::shared_ptr<Particle> &particle);

  /**
   * Add outgoing particle.
   */
  void addParticleOut(const std::shared_ptr<Particle> &particle);

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get references to incoming particles.
   */
  const std::vector<std::shared_ptr<Particle>> &getParticlesIn() const;

  /**
   * Set references to incoming particles.
   */
  void
  setParticlesIn(const std::vector<std::shared_ptr<Particle>> &particlesIn);

  /**
   * Get references to outgoing particles.
   */
  const std::vector<std::shared_ptr<Particle>> &getParticlesOut() const;

  /**
   * Set references to outgoing particles.
   */
  void
  setParticlesOut(const std::vector<std::shared_ptr<Particle>> &particlesOut);

private:
  std::vector<std::shared_ptr<Particle>>
      m_particlesIn; ///< References to incoming particles.
  std::vector<std::shared_ptr<Particle>>
      m_particlesOut; ///< References to outgoing particles.
};

} /* namespace EPIC */

#endif /* BEANS_VERTEX_H_ */

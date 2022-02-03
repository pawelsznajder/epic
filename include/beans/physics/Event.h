/*
 * Event.h
 *
 *  Created on: Feb 10, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_EVENT_H_
#define BEANS_EVENT_H_

#include <stddef.h>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../types/EventAttributeType.h"
#include "../types/ParticleCodeType.h"

namespace EPIC {
class Particle;
class Vertex;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class Event
 *
 * @brief Single event.
 *
 * This class defines a single event containing events and vertices, and
 * eventually a weight.
 */
class Event : public PARTONS::BaseObject {

public:
  /**
   * Default constructor.
   */
  Event();

  /**
   * Copy constructor.
   */
  Event(const Event &other);

  /**
   * Destructor.
   */
  virtual ~Event();

  std::string toString() const;

  /**
   * Alter code of particle specified by iterator.
   */
  void alterParticleCode(size_t i, ParticleCodeType::Type code);

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get particles.
   */
  const std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > &getParticles() const;

  /**
   * Set particles.
   */
  void setParticles(const std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > &particles);

  /**
   * Add particle.
   */
  void addParticle(const std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > &particle);

  /**
   * Get vertices.
   */
  const std::vector<std::shared_ptr<Vertex> > &getVertices() const;

  /**
   * Set vertices.
   */
  void setVertices(const std::vector<std::shared_ptr<Vertex> > &vertices);

  /**
   * Add vertex.
   */
  void addVertex(std::shared_ptr<Vertex> vertex);

  /**
   * Get attributes.
   */
  const std::map<EventAttributeType::Type, double> &getAttributes() const;

  /**
   * Set attributes.
   */
  void
  setAttributes(const std::map<EventAttributeType::Type, double> &attributes);

  /**
   * Add attribute.
   */
  void addAttribute(EventAttributeType::Type type, double value);

private:

  std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > m_particles; ///< Particles.
  std::vector<std::shared_ptr<Vertex> > m_vertices;                     ///< Vertices.

  std::map<EventAttributeType::Type, double> m_attributes; ///< Attributes.
};

} /* namespace EPIC */

#endif /* BEANS_EVENT_H_ */

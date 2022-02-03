/*
 * Vertex.cpp
 *
 *  Created on: Feb 10, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/physics/Vertex.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <iterator>

#include "../../../include/beans/physics/Particle.h"

namespace EPIC {

Vertex::Vertex() : PARTONS::BaseObject("Vertex") {}

Vertex::Vertex(const Vertex &other) : PARTONS::BaseObject(other) {

  m_particlesIn = other.m_particlesIn;
  m_particlesOut = other.m_particlesOut;
}

Vertex::~Vertex() {}

std::string Vertex::toString() const {

  std::vector<std::shared_ptr<Particle>>::const_iterator it;

  ElemUtils::Formatter formatter;

  formatter << "Incoming particles:\n";

  for (it = m_particlesIn.begin(); it != m_particlesIn.end(); it++) {
    formatter << (*it)->toString() << '\n';
  }

  formatter << '\n';

  formatter << "Outgoing particles:\n";

  for (it = m_particlesOut.begin(); it != m_particlesOut.end(); it++) {
    formatter << (*it)->toString() << '\n';
  }

  return formatter.str();
}

void Vertex::addParticleIn(const std::shared_ptr<Particle> &particle) {
  m_particlesIn.push_back(particle);
}

void Vertex::addParticleOut(const std::shared_ptr<Particle> &particle) {
  m_particlesOut.push_back(particle);
}

const std::vector<std::shared_ptr<Particle>> &Vertex::getParticlesIn() const {
  return m_particlesIn;
}

void Vertex::setParticlesIn(
    const std::vector<std::shared_ptr<Particle>> &particlesIn) {
  m_particlesIn = particlesIn;
}

const std::vector<std::shared_ptr<Particle>> &Vertex::getParticlesOut() const {
  return m_particlesOut;
}

void Vertex::setParticlesOut(
    const std::vector<std::shared_ptr<Particle>> &particlesOut) {
  m_particlesOut = particlesOut;
}

} /* namespace EPIC */

/*
 * Event.cpp
 *
 *  Created on: Feb 10, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/physics/Event.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iterator>

#include "../../../include/beans/physics/Particle.h"
#include "../../../include/beans/physics/Vertex.h"

namespace EPIC {

Event::Event() : PARTONS::BaseObject("Event") {}

Event::Event(const Event &other) : PARTONS::BaseObject(other) {

  m_particles = other.m_particles;
  m_vertices = other.m_vertices;
  m_attributes = other.m_attributes;
}

Event::~Event() {}

std::string Event::toString() const {

  ElemUtils::Formatter formatter;

  formatter << "Particles:\n";

  for (std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > >::const_iterator it =
           m_particles.begin();
       it != m_particles.end(); it++) {
    formatter << ParticleCodeType(it->first).toString() << '\t' << (it->second)->toString() << '\n';
  }

  formatter << '\n';

  formatter << "Vertices:\n";

  for (std::vector<std::shared_ptr<Vertex> >::const_iterator it = m_vertices.begin();
       it != m_vertices.end(); it++) {
    formatter << (*it)->toString() << '\n';
  }

  formatter << '\n';

  formatter << "Attributes:\n";

  for (std::map<EventAttributeType::Type, double>::const_iterator it =
           m_attributes.begin();
       it != m_attributes.end(); it++) {
    formatter << EventAttributeType(it->first).toString() << '\t' << it->second
              << '\n';
  }

  return formatter.str();
}

void Event::alterParticleCode(size_t i, ParticleCodeType::Type code){

	if(i >= m_particles.size()){
		throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Index out of range, i = " << i);
	}

	m_particles.at(i).first = code;
}

const std::map<EventAttributeType::Type, double> &Event::getAttributes() const {
  return m_attributes;
}

void Event::setAttributes(
    const std::map<EventAttributeType::Type, double> &attributes) {
  m_attributes = attributes;
}

void Event::addAttribute(EventAttributeType::Type type, double value) {
  m_attributes.insert(std::make_pair(type, value));
}

const std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > &Event::getParticles() const {
  return m_particles;
}

void Event::setParticles(
    const std::vector<std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > > &particles) {
  m_particles = particles;
}

void Event::addParticle(const std::pair<ParticleCodeType::Type, std::shared_ptr<Particle> > &particle){
	m_particles.push_back(particle);
}

const std::vector<std::shared_ptr<Vertex> > &Event::getVertices() const { return m_vertices; }

void Event::setVertices(const std::vector<std::shared_ptr<Vertex> > &vertices) {
  m_vertices = vertices;
}

void Event::addVertex(std::shared_ptr<Vertex> vertex){
	m_vertices.push_back(vertex);
}

} /* namespace EPIC */

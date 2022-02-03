#include "../../../include/beans/physics/Particle.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>

namespace EPIC {

Particle::Particle()
    : PARTONS::BaseObject("Particle"), m_type(ParticleType::UNDEFINED) {}

Particle::Particle(ParticleType::Type type)
    : PARTONS::BaseObject("Particle"), m_type(type) {}

Particle::Particle(ParticleType::Type type, const TLorentzVector &lv)
    : PARTONS::BaseObject("Particle"), m_type(type), m_lv(lv) {}

Particle::Particle(ParticleType::Type type, const TVector3 &mom)
    : PARTONS::BaseObject("Particle"), m_type(type) {
  m_lv.SetVectM(mom, ParticleType(type).getMass());
}

Particle::Particle(ParticleType::Type type, const TVector3 &dir, double E)
    : PARTONS::BaseObject("Particle"), m_type(type) {

  double mom = sqrt(pow(E, 2) - pow(ParticleType(type).getMass(), 2));
  m_lv.SetVectM(mom * dir.Unit(), ParticleType(type).getMass());
}

Particle::Particle(const Particle &other)
    : PARTONS::BaseObject(other), m_type(other.m_type), m_lv(other.m_lv) {}

Particle::~Particle() {}

double Particle::getEnergy() const { return m_lv.E(); }

double Particle::getMass() const { return m_lv.Mag(); }

TVector3 Particle::getMomentum() const { return m_lv.Vect(); }

double Particle::getPseudoRapidity() const { return m_lv.PseudoRapidity(); }

void Particle::rotate(AxisType::Type axis, double angle) {

  switch (axis) {

  case AxisType::X:
    m_lv.RotateX(angle);
    break;
  case AxisType::Y:
    m_lv.RotateY(angle);
    break;
  case AxisType::Z:
    m_lv.RotateZ(angle);
    break;

  default:
    throw ElemUtils::CustomException(getClassName(), __func__,
                                     ElemUtils::Formatter()
                                         << "Axis type is: "
                                         << AxisType(axis).toString());
  }
}

void Particle::rotate(const TVector3 &dir, double angle) {
  m_lv.Rotate(angle, dir);
}

void Particle::rotate(const Particle &particle, double angle) {
  m_lv.Rotate(angle, particle.getFourMomentum().Vect());
}

void Particle::boost(const TVector3 &v) { m_lv.Boost(v); }

void Particle::boost(const Particle &particle) {
  m_lv.Boost(-particle.getFourMomentum().BoostVector());
}

std::string Particle::toString() const {

  ElemUtils::Formatter formatter;

  formatter << "type: " << ParticleType(m_type).toString() << ' ';
  formatter << "px: " << m_lv.X() << ' ';
  formatter << "py: " << m_lv.Y() << ' ';
  formatter << "pz: " << m_lv.Z() << ' ';
  formatter << "E: " << m_lv.E() << ' ';
  formatter << "M: " << m_lv.M();

  return formatter.str();
}

double Particle::getAngle(AxisType::Type axis) const {

  switch (axis) {

  case AxisType::X:
    return m_lv.Vect().Angle(TVector3(1., 0., 0.));
    break;
  case AxisType::Y:
    return m_lv.Vect().Angle(TVector3(0., 1., 0.));
    break;
  case AxisType::Z:
    return m_lv.Vect().Angle(TVector3(0., 0., 1.));
    break;

  default:
    throw ElemUtils::CustomException(getClassName(), __func__,
                                     ElemUtils::Formatter()
                                         << "Axis type is: "
                                         << AxisType(axis).toString());
  }

  return 0.;
}

double Particle::getAngle(const TVector3 &dir) const {
  return m_lv.Vect().Angle(dir);
}

double Particle::getAngle(const Particle &particle) const {
  return m_lv.Vect().Angle(particle.getFourMomentum().Vect());
}

const TLorentzVector &Particle::getFourMomentum() const { return m_lv; }

void Particle::setFourMomentum(const TLorentzVector &lv) { m_lv = lv; }

ParticleType::Type Particle::getType() const { return m_type; }

void Particle::setType(ParticleType::Type type) { m_type = type; }
}

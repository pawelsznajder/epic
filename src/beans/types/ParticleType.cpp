#include "../../../include/beans/types/ParticleType.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <partons/FundamentalPhysicalConstants.h>

namespace EPIC {

ParticleType::ParticleType()
    : PARTONS::BaseObject("ParticleType"), m_type(ParticleType::UNDEFINED) {}

ParticleType::ParticleType(Type type)
    : PARTONS::BaseObject("ParticleType"), m_type(type) {}

ParticleType::ParticleType(const ParticleType &other)
    : PARTONS::BaseObject(other), m_type(other.m_type) {}

ParticleType::operator ParticleType::Type() const { return m_type; }

std::string ParticleType::toString() const {

  switch (m_type) {

  case ELECTRON:
    return "e-";
    break;
  case POSITRON:
    return "e+";
    break;
  case MUON_MINUS:
    return "muon-";
    break;
  case MUON_PLUS:
    return "muon+";
    break;
  case TAU_MINUS:
    return "tau-";
    break;
  case TAU_PLUS:
    return "tau-";
    break;
  case PHOTON:
    return "gamma";
    break;
  case PI0:
    return "pi0";
    break;
  case PROTON:
    return "p";
    break;
  case JPSI:
    return "J/Psi";
    break;

  default:
    return "UNDEFINED";
  }
}

ParticleType::Type ParticleType::fromString(const std::string &str) {

  std::string strTrimmed = str;
  ElemUtils::StringUtils::trimAll(strTrimmed);

  ParticleType::Type type = ParticleType::UNDEFINED;

  if (strTrimmed == "e-") {
    type = ParticleType::ELECTRON;
  } else if (strTrimmed == "e+") {
    type = ParticleType::POSITRON;
  } else if (strTrimmed == "mu-") {
    type = ParticleType::MUON_MINUS;
  } else if (strTrimmed == "mu+") {
    type = ParticleType::MUON_PLUS;
  } else if (strTrimmed == "tau-") {
    type = ParticleType::TAU_MINUS;
  } else if (strTrimmed == "tau+") {
    type = ParticleType::TAU_PLUS;
  } else if (strTrimmed == "gamma") {
    type = ParticleType::PHOTON;
  } else if (strTrimmed == "pi0") {
    type = ParticleType::PI0;
  } else if (strTrimmed == "p") {
    type = ParticleType::PROTON;
  } else if (strTrimmed == "J/Psi") {
  type = ParticleType::JPSI;
  }
  return type;
}

double ParticleType::getMass() const {

  switch (m_type) {

  case ELECTRON:
    return PARTONS::Constant::ELECTRON_MASS;
    break;
  case POSITRON:
    return PARTONS::Constant::ELECTRON_MASS;
    break;
  case MUON_MINUS:
    return PARTONS::Constant::MUON_MASS;
    break;
  case MUON_PLUS:
    return PARTONS::Constant::MUON_MASS;
    break;
  case TAU_MINUS:
    return PARTONS::Constant::TAU_MASS;
    break;
  case TAU_PLUS:
    return PARTONS::Constant::TAU_MASS;
    break;
  case PHOTON:
    return 0.;
    break;
  case PI0:
    return PARTONS::Constant::PI_ZERO_MASS;
    break;
  case PROTON:
    return PARTONS::Constant::PROTON_MASS;
    break;
  case JPSI:
    return PARTONS::Constant::MESON_JPSI_MASS;
    break;

  default:
    throw ElemUtils::CustomException(
        getClassName(), __func__, ElemUtils::Formatter()
                                      << "Not able to return mass for particle "
                                      << toString());
  }

  return 0.;
}

int ParticleType::getCharge() const {

  switch (m_type) {

  case ELECTRON:
    return -1;
    break;
  case POSITRON:
    return 1;
    break;
  case MUON_MINUS:
    return -1;
    break;
  case MUON_PLUS:
    return 1;
    break;
  case TAU_MINUS:
    return -1;
    break;
  case TAU_PLUS:
    return 1;
    break;
  case PHOTON:
    return 0;
    break;
  case PI0:
    return 0;
    break;
  case PROTON:
    return 1;
    break;
  case JPSI:
    return 0;
    break;

  default:
    throw ElemUtils::CustomException(
        getClassName(), __func__,
        ElemUtils::Formatter() << "Not able to return charge for particle "
                               << toString());
  }

  return 0.;
}

ParticleType::Type ParticleType::getType() const { return m_type; }

void ParticleType::setType(Type type) { m_type = type; }
}

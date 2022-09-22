#include "../../../include/beans/types/ParticleCodeType.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <partons/FundamentalPhysicalConstants.h>

namespace EPIC {

ParticleCodeType::ParticleCodeType()
    : PARTONS::BaseObject("ParticleCodeType"), m_type(ParticleCodeType::UNDEFINED) {}

ParticleCodeType::ParticleCodeType(Type type)
    : PARTONS::BaseObject("ParticleCodeType"), m_type(type) {}

ParticleCodeType::ParticleCodeType(const ParticleCodeType &other)
    : PARTONS::BaseObject(other), m_type(other.m_type) {}

ParticleCodeType::operator ParticleCodeType::Type() const { return m_type; }

std::string ParticleCodeType::toString() const {

  switch (m_type) {

  case UNDECAYED:
    return "undecayed";
    break;
  case DECAYED:
    return "decayed";
    break;
  case DOCUMENTATION:
    return "documentation";
    break;
  case BEAM:
    return "beam";
    break;
  case SCATTERED:
    return "scattered";
    break;
  case VIRTUAL:
    return "virtual";
    break;

  default:
    return "UNDEFINED";
  }
}

ParticleCodeType::Type ParticleCodeType::fromString(const std::string &str) {

  std::string strTrimmed = str;
  ElemUtils::StringUtils::trimAll(strTrimmed);

  ParticleCodeType::Type type = ParticleCodeType::UNDEFINED;

  if (strTrimmed == "undecayed") {
    type = ParticleCodeType::UNDECAYED;
  } else if (strTrimmed == "decayed") {
    type = ParticleCodeType::DECAYED;
  } else if (strTrimmed == "documentation") {
    type = ParticleCodeType::DOCUMENTATION;
  } else if (strTrimmed == "beam") {
    type = ParticleCodeType::BEAM;
  } else if (strTrimmed == "virtual") {
    type = ParticleCodeType::VIRTUAL;
  }

  return type;
}

ParticleCodeType::Type ParticleCodeType::getType() const { return m_type; }

void ParticleCodeType::setType(Type type) { m_type = type; }
}

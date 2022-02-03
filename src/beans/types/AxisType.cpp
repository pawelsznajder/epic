#include "../../../include/beans/types/AxisType.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

namespace EPIC {

AxisType::AxisType()
    : PARTONS::BaseObject("AxisType"), m_type(AxisType::UNDEFINED) {}

AxisType::AxisType(Type type) : PARTONS::BaseObject("AxisType"), m_type(type) {}

AxisType::AxisType(const AxisType &other)
    : PARTONS::BaseObject("AxisType"), m_type(other.m_type) {}

AxisType::operator AxisType::Type() const { return m_type; }

std::string AxisType::toString() const {

  switch (m_type) {

  case X:
    return "X";
    break;
  case Y:
    return "Y";
    break;
  case Z:
    return "Z";
    break;

  default:
    return "UNDEFINED";
  }
}

AxisType::Type AxisType::fromString(const std::string &str) {

  std::string strTrimmed = str;
  ElemUtils::StringUtils::trimAll(strTrimmed);

  AxisType::Type type = AxisType::UNDEFINED;

  if (strTrimmed == "X") {
    type = AxisType::X;
  } else if (strTrimmed == "Y") {
    type = AxisType::Y;
  } else if (strTrimmed == "Z") {
    type = AxisType::Z;
  }

  return type;
}

AxisType::Type AxisType::getType() const { return m_type; }

void AxisType::setType(Type type) { m_type = type; }
}

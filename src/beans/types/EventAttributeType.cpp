#include "../../../include/beans/types/EventAttributeType.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

namespace EPIC {

EventAttributeType::EventAttributeType()
    : PARTONS::BaseObject("EventAttributeType"),
      m_type(EventAttributeType::UNDEFINED) {}

EventAttributeType::EventAttributeType(Type type)
    : PARTONS::BaseObject("EventAttributeType"), m_type(type) {}

EventAttributeType::EventAttributeType(const EventAttributeType &other)
    : PARTONS::BaseObject("EventAttributeType"), m_type(other.m_type) {}

EventAttributeType::operator EventAttributeType::Type() const { return m_type; }

std::string EventAttributeType::toString() const {

  switch (m_type) {

  case ID:
    return "id";
    break;

  case WEIGHT:
    return "weight";
    break;

  default:
    return "UNDEFINED";
  }
}

EventAttributeType::Type
EventAttributeType::fromString(const std::string &str) {

  std::string strTrimmed = str;
  ElemUtils::StringUtils::trimAll(strTrimmed);

  EventAttributeType::Type type = EventAttributeType::UNDEFINED;

  if (strTrimmed == "id") {
    type = EventAttributeType::ID;
  } else if (strTrimmed == "weight") {
    type = EventAttributeType::WEIGHT;
  }

  return type;
}

EventAttributeType::Type EventAttributeType::getType() const { return m_type; }

void EventAttributeType::setType(Type type) { m_type = type; }
}

#include "../../../include/modules/writer/WriterHepMC3Type.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

namespace EPIC {

WriterHepMC3Type::WriterHepMC3Type()
    : PARTONS::BaseObject("WriterHepMC3Type"),
      m_type(WriterHepMC3Type::UNDEFINED) {}

WriterHepMC3Type::WriterHepMC3Type(Type type)
    : PARTONS::BaseObject("WriterHepMC3Type"), m_type(type) {}

WriterHepMC3Type::WriterHepMC3Type(const WriterHepMC3Type &other)
    : PARTONS::BaseObject("WriterHepMC3Type"), m_type(other.m_type) {}

WriterHepMC3Type::operator WriterHepMC3Type::Type() const { return m_type; }

std::string WriterHepMC3Type::toString() const {

  switch (m_type) {

  case ASCII:
    return "ascii";
    break;
  case HEPEVT:
    return "hepevt";
    break;
  case ROOT:
    return "root";
    break;

  default:
    return "UNDEFINED";
  }
}

WriterHepMC3Type::Type WriterHepMC3Type::fromString(const std::string &str) {

  std::string strTrimmed = str;
  ElemUtils::StringUtils::trimAll(strTrimmed);

  WriterHepMC3Type::Type type = WriterHepMC3Type::UNDEFINED;

  if (strTrimmed == "ascii") {
    type = WriterHepMC3Type::ASCII;
  } else if (strTrimmed == "hepevt") {
    type = WriterHepMC3Type::HEPEVT;
  } else if (strTrimmed == "root") {
    type = WriterHepMC3Type::ROOT;
  }

  return type;
}

WriterHepMC3Type::Type WriterHepMC3Type::getType() const { return m_type; }

void WriterHepMC3Type::setType(Type type) { m_type = type; }
}

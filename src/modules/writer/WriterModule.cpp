/*
 * RecordModule.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/modules/writer/WriterModule.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/channel/ChannelType.h>

namespace EPIC {

const std::string WriterModule::WRITER_MODULE_CLASS_NAME = "WriterModule";

const std::string WriterModule::PARAMETER_NAME_PATH = "output_file_path";

WriterModule::WriterModule(const std::string &className)
    : EpicModuleObject(className, PARTONS::ChannelType::UNDEFINED) {}

WriterModule::WriterModule(const WriterModule &other)
    : EpicModuleObject(other) {
  m_path = other.m_path;
}

WriterModule::~WriterModule() {}

void WriterModule::configure(const ElemUtils::Parameters &parameters) {

  // run for mother
  EpicModuleObject::configure(parameters);

  // look for name
  if (parameters.isAvailable(WriterModule::PARAMETER_NAME_PATH)) {

    // get
    m_path = parameters.getLastAvailable().getString();

    // print status
    info(__func__, ElemUtils::Formatter() << "Output file name set to: "
                                          << m_path);
  }
}

bool WriterModule::runTest() const {

  // TODO
  return true;
}

void WriterModule::initModule() {}

void WriterModule::isModuleWellConfigured() {}

const std::string &WriterModule::getPath() const { return m_path; }

void WriterModule::setPath(const std::string &path) { m_path = path; }

} /* namespace EPIC */

/*
 * RecordModule.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_WRITER_WRITERMODULE_H_
#define MODULES_WRITER_WRITERMODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../beans/physics/Event.h"
#include "../EpicModuleObject.h"

namespace EPIC {
class GenerationInformation;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class RecordModule
 *
 * @brief Definition of module for generation of random numbers.
 *
 * This class provides a definition of modules used to generate random numbers.
 */
class WriterModule : public EpicModuleObject {

public:
  static const std::string
      WRITER_MODULE_CLASS_NAME; ///< Class name used in parsing XML.

  static const std::string PARAMETER_NAME_PATH; ///< String used to set path
                                                /// through
                                                /// WriterModule::configure().

  /**
   * Constructor.
   */
  WriterModule(const std::string &className);

  /**
   * Copy constructor.
   */
  WriterModule(const WriterModule &other);

  /**
   * Destructor.
   */
  virtual ~WriterModule();

  virtual void configure(const ElemUtils::Parameters &parameters);

  virtual bool runTest() const;

  /**
   * Open file.
   */
  virtual void open() = 0;

  /**
   * Save run information.
   */
  virtual void saveGenerationInformation(const GenerationInformation& generationInformation) = 0;

  /**
   * Close file.
   */
  virtual void close() = 0;

  /**
   * Write single event (file must be open).
   */
  virtual void write(const Event &event) = 0;

  /**
   * Write vector of events (file must be open).
   */
  virtual void write(const std::vector<Event> &events) = 0;

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get path.
   */
  const std::string &getPath() const;

  /**
   * Set path.
   */
  void setPath(const std::string &path);

protected:
  virtual void initModule();
  virtual void isModuleWellConfigured();

  std::string m_path; ///< Path to output file.
};

} /* namespace EPIC */

#endif /* MODULES_WRITER_WRITERMODULE_H_ */

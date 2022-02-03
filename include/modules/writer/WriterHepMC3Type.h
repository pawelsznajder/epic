#ifndef MODULES_WRITER_WRITERHEPMC3TYPE_H
#define MODULES_WRITER_WRITERHEPMC3TYPE_H

/**
 * @file WriterHepMC3Type.h
 * @author Pawel Sznajder (NCBJ)
 * @date 20 June 2020
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace EPIC {

/**
 * @class WriterHepMC3Type
 *
 * @brief Definition of enumeration values for writers available in HepMC3.
 *
 * This class defines a set of enumeration values that are used to distinguish
 * between writer availibles in HepMC3 library.
 * In addition, a declared object of this class is always associated to one type
 * (see WriterHepMC3Type::m_type), so member functions can act on it.
 */
class WriterHepMC3Type : public PARTONS::BaseObject {

public:
  /**
   * Definition of enumerate values corresponding to axis types.
   */
  enum Type {

    UNDEFINED = 0, //!< Undefined type.

    ASCII = 1,  //!<  WriterAscii
    HEPEVT = 2, //!<  WriterHEPEVT
    ROOT = 3    //!<  WriterRoot
  };

  /**
   * Default constructor.
   */
  WriterHepMC3Type();

  /**
   * Assignment constructor.
   */
  WriterHepMC3Type(Type type);

  /**
   * Copy constructor.
   */
  WriterHepMC3Type(const WriterHepMC3Type &other);

  /**
   * Automatic cast to enum.
   */
  operator Type() const;

  virtual std::string toString() const;

  /**
   * Get type from string.
   */
  static WriterHepMC3Type::Type fromString(const std::string &str);

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get type being assigned to a declared object of this class.
   */
  WriterHepMC3Type::Type getType() const;

  /**
   * Assign type to a declared object of this class.
   */
  void setType(Type type);

private:
  /**
   * Type associated to a declared object of this class.
   */
  WriterHepMC3Type::Type m_type;
};
}

#endif /* MODULES_WRITER_WRITERHEPMC3TYPE_H */

#ifndef BEANS_AXIS_TYPE_H
#define BEANS_AXIS_TYPE_H

/**
 * @file AxisType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 20 June 2020
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace EPIC {

/**
 * @class AxisType
 *
 * @brief Definition of enumeration values for axis types.
 *
 * This class defines a set of enumeration values that are used to distinguish
 * between axis types.
 * In addition, a declared object of this class is always associated to one type
 * (see AxisType::m_type), so member functions can act on it.
 */
class AxisType : public PARTONS::BaseObject {

public:
  /**
   * Definition of enumerate values corresponding to axis types.
   */
  enum Type {

    UNDEFINED = 0, //!< Undefined type.

    X = 1, //!<  x
    Y = 2, //!<  y
    Z = 3  //!<  z
  };

  /**
   * Default constructor.
   */
  AxisType();

  /**
   * Assignment constructor.
   */
  AxisType(Type type);

  /**
   * Copy constructor.
   */
  AxisType(const AxisType &other);

  /**
   * Automatic cast to enum.
   */
  operator Type() const;

  virtual std::string toString() const;

  /**
   * Get type from string.
   */
  static AxisType::Type fromString(const std::string &str);

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get type being assigned to a declared object of this class.
   */
  AxisType::Type getType() const;

  /**
   * Assign type to a declared object of this class.
   */
  void setType(Type type);

private:
  /**
   * Type associated to a declared object of this class.
   */
  AxisType::Type m_type;
};
}

#endif /* BEANS_AXIS_TYPE_H */

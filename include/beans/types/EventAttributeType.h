#ifndef BEANS_EVENT_ATTRIBUTE_TYPE_H
#define BEANS_EVENT_ATTRIBUTE_TYPE_H

/**
 * @file EventAttributeType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 20 June 2020
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace EPIC {

/**
 * @class EventAttributeType
 *
 * @brief Definition of event attribute types.
 *
 * This class defines a set of enumeration values that are used to distinguish
 * between event attributes types.
 * In addition, a declared object of this class is always associated to one type
 * (see EventAttributeType::m_type), so member functions can act on it.
 */
class EventAttributeType : public PARTONS::BaseObject {

public:
  /**
   * Definition of enumerate values corresponding to axis types.
   */
  enum Type {

    UNDEFINED = 0, //!< Undefined type.

    ID = 1,    //!< Id
    WEIGHT = 2 //!< Weight
  };

  /**
   * Default constructor.
   */
  EventAttributeType();

  /**
   * Assignment constructor.
   */
  EventAttributeType(Type type);

  /**
   * Copy constructor.
   */
  EventAttributeType(const EventAttributeType &other);

  /**
   * Automatic cast to enum.
   */
  operator Type() const;

  virtual std::string toString() const;

  /**
   * Get type from string.
   */
  static EventAttributeType::Type fromString(const std::string &str);

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get type being assigned to a declared object of this class.
   */
  EventAttributeType::Type getType() const;

  /**
   * Assign type to a declared object of this class.
   */
  void setType(Type type);

private:
  /**
   * Type associated to a declared object of this class.
   */
  EventAttributeType::Type m_type;
};
}

#endif /* BEANS_EVENT_ATTRIBUTE_TYPE_H */

#ifndef BEANS_PARTICLE_CODE_TYPE_H
#define BEANS_PARTICLE_CODE_TYPE_H

/**
 * @file ParticleCodeType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 20 June 2020
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace EPIC {

/**
 * @class ParticleCodeType
 *
 * @brief Definition of enumeration values for particle types.
 *
 * This class defines a set of enumeration values that are used to distinguish
 * between particle code types. In addition, a declared object of this class is
 * always associated to one type (see ParticleCodeType::m_type), so member functions
 * can act on it.
 */
class ParticleCodeType : public PARTONS::BaseObject {

public:
  /**
   * Definition of enumerate values corresponding to particle code types.
   * According to Table 5 from https://arxiv.org/pdf/1912.08005.pdf
   */
  enum Type {

    UNDEFINED = 0, 	//!< Undefined type.

    UNDECAYED = 1,   	//!< Undecayed.
    DECAYED = 2,  	    //!< Decayed.
    DOCUMENTATION = 3, 	//!< Documentation.
    BEAM = 4, 		    //!< Beam.
    SCATTERED = 5,      //!< Scattered beam.
    VIRTUAL = 6	        //!< Virtual.
  };

  /**
   * Default constructor.
   */
  ParticleCodeType();

  /**
   * Assignment constructor.
   * @param type Type to be assigned.
   */
  ParticleCodeType(Type type);

  /**
   * Copy constructor.
   * @param other Object to be copied.
   */
  ParticleCodeType(const ParticleCodeType &other);

  /**
   * Automatic cast to enum.
   */
  operator Type() const;

  std::string toString() const;

  /**
   * Get type from string.
   * @param String.
   * @return Associated type.
   */
  static ParticleCodeType::Type fromString(const std::string &str);

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get type being assigned to a declared object of this class.
   */
  ParticleCodeType::Type getType() const;

  /**
   * Assign type to a declared object of this class.
   */
  void setType(Type type);

private:
  /**
   * Type associated to a declared object of this class.
   */
  ParticleCodeType::Type m_type;
};
}

#endif /* BEANS_PARTICLE_CODE_TYPE_H */

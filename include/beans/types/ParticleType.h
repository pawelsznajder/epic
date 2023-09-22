#ifndef BEANS_PARTICLE_TYPE_H
#define BEANS_PARTICLE_TYPE_H

/**
 * @file ParticleType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 20 June 2020
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <string>

namespace EPIC {

/**
 * @class ParticleType
 *
 * @brief Definition of enumeration values for particle types.
 *
 * This class defines a set of enumeration values that are used to distinguish
 * between particle types. In addition, a declared object of this class is
 * always associated to one type (see ParticleType::m_type), so member functions
 * can act on it.
 */
class ParticleType : public PARTONS::BaseObject {

public:
  /**
   * Definition of enumerate values corresponding to particle types.
   * According to PDG codes: http://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
   */
  enum Type {

    UNDEFINED = 0, //!< Undefined type.

    ELECTRON = 11,   //!<  electron
    POSITRON = -11,  //!<  positron
    MUON_MINUS = 13, //!<  muon -
    MUON_PLUS = -13, //!<  muon +
    TAU_MINUS = 15,  //!<  tau -
    TAU_PLUS = -15,  //!<  tau +
    PHOTON = 22,     //!<  gamma
    PI0 = 111,       //!<  pi0
    PROTON = 2212,   //!<  proton
	JPSI = 443		 //!<  J/Psi
  };

  /**
   * Default constructor.
   */
  ParticleType();

  /**
   * Assignment constructor.
   * @param type Type to be assigned.
   */
  ParticleType(Type type);

  /**
   * Copy constructor.
   * @param other Object to be copied.
   */
  ParticleType(const ParticleType &other);

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
  static ParticleType::Type fromString(const std::string &str);

  /**
   * Get mass of associated particle.
   */
  double getMass() const;

  /**
   * Get charge of associated particle.
   */
  int getCharge() const;

  //********************************************************
  //*** SETTERS AND GETTERS ********************************
  //********************************************************

  /**
   * Get type being assigned to a declared object of this class.
   */
  ParticleType::Type getType() const;

  /**
   * Assign type to a declared object of this class.
   */
  void setType(Type type);

private:
  /**
   * Type associated to a declared object of this class.
   */
  ParticleType::Type m_type;
};
}

#endif /* BEANS_PARTICLE_TYPE_H */

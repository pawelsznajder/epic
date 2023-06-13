/*
 * KinematicModule.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_KINEMATIC_KINEMATICMODULE_H_
#define MODULES_KINEMATIC_KINEMATICMODULE_H_

#include <partons/beans/channel/ChannelType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <string>
#include <vector>

#include "../EpicModuleObject.h"
#include "../../beans/containers/KinematicRange.h"

namespace EPIC {
class Event;
} /* namespace EPIC */

namespace EPIC {
class ExperimentalConditions;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class KinematicModule
 *
 * @brief Template for kinematic module.
 *
 * This is a template for kinematic modules.
 */
template <typename KinematicRangeType, typename KinematicType>
class KinematicModule : public EpicModuleObject {

public:
  /**
   * Destructor.
   */
  virtual ~KinematicModule() {}

  /**
   * Get correct kinematic ranges.
   */
  virtual std::vector<KinematicRange> getKinematicRanges(const ExperimentalConditions &conditions, const KinematicRangeType& ranges) = 0;

  /**
   * Check if kinematics is valid.
   */
  virtual bool checkIfValid(const ExperimentalConditions &conditions,
                            const KinematicType &kin) = 0;

  /**
   * Get experimental conditions (like beam types), kinematics and return a set
   * of particles.
   */
  virtual Event evaluate(const ExperimentalConditions &conditions,
                         const KinematicType &kin) = 0;

protected:
  /**
   * Constructor.
   * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
   *
   * @param className name of child class.
   * @param channelType Channel type.
   */
  KinematicModule(const std::string &className,
                  PARTONS::ChannelType::Type channelType)
      : EpicModuleObject(className, channelType) {}

  /**
   * Copy constructor
   * @param other Object to be copied.
   */
  KinematicModule(const KinematicModule &other) : EpicModuleObject(other) {}

  void changeKinematicRange(KinematicRange& range, bool isMin, double value, const std::string& name, double divisionFactor = 10) const {

      KinematicRange old = range;

     if(isMin){  
         range.setMinMax(value/divisionFactor, range.getMax());
     }else{
         range.setMinMax(range.getMin(), value/divisionFactor);
     }

     warn(__func__, ElemUtils::Formatter() << "Range for variable " << name << " changed from " << old.toString() << " to " << range.toString());
  }

};
}

#endif /* MODULES_KINEMATIC_KINEMATICMODULE_H_ */

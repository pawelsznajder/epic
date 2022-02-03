/*
 * EpicModuleObject.h
 *
 *  Created on: Feb 7, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef MODULES_EPICMODULEOBJECT_H_
#define MODULES_EPICMODULEOBJECT_H_

#include <partons/ModuleObject.h>
#include <partons/beans/channel/ChannelType.h>
#include <string>

namespace EPIC {

/**
 * @class EpicModuleObject
 *
 * @brief Testable version of PARTONS::ModuleObject.
 *
 * This class is an extension of PARTONS::ModuleObject for a test method defined
 * in
 * EpicModuleObject::runTest().
 */
class EpicModuleObject: public PARTONS::ModuleObject {

public:
	/**
	 * Destructor.
	 */
	virtual ~EpicModuleObject();

	/**
	 * Definition of test.
	 */
	virtual bool runTest() const = 0;

	/**
	 * Get random seed.
	 */
	size_t getSeed() const;

protected:

	/**
	 * Default constructor.
	 */
	EpicModuleObject(const std::string &className,
			PARTONS::ChannelType::Type channelType);

	/**
	 * Copy constructor.
	 */
	EpicModuleObject(const EpicModuleObject &other);

	size_t m_seed; ///< Random seed.
};

} /* namespace EPIC */

#endif /* MODULES_EPICMODULEOBJECT_H_ */

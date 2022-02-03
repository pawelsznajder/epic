/*
 * KinematicRange.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_KINEMATICRANGE_H_
#define BEANS_KINEMATICRANGE_H_

#include <partons/BaseObject.h>
#include <string>
#include <utility>
#include <vector>

namespace EPIC {
class MonteCarloTask;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class KinematicRange
 *
 * @brief Container to store a single kinematic range.
 *
 * This class acts as a container to store a single kinematic range.
 */
class KinematicRange: public PARTONS::BaseObject {

public:
	/**
	 * Default constructor.
	 */
	KinematicRange();

	/**
	 * Assignment constructor.
	 */
	KinematicRange(double min, double max);

	/**
	 * Assignment constructor.
	 */
	KinematicRange(const std::pair<double, double> &minMax);

	/**
	 * Assignment constructor.
	 */
	KinematicRange(const std::vector<double> &minMax);

	/**
	 * Copy constructor.
	 */
	KinematicRange(const KinematicRange &other);

	/**
	 * Destructor
	 */
	virtual ~KinematicRange();

	virtual std::string toString() const;

	/**
	 * Check if value in range: [min, max].
	 */
	bool inRange(double value) const;

//********************************************************
//*** SETTERS AND GETTERS ********************************
//********************************************************

	/**
	 * Get range.
	 */
	const std::pair<double, double> &getMinMax() const;

	/**
	 * Get min.
	 */
	double getMin() const;

	/**
	 * Get max.
	 */
	double getMax() const;

	/**
	 * Set range.
	 */
	void setMinMax(double min, double max);

	/**
	 * Set range.
	 */
	void setMinMax(const std::pair<double, double> &minMax);

	/**
	 * Set range.
	 */
	void setMinMax(const std::vector<double> &minMax);

private:

	/**
	 * Check if range is valid.
	 */
	void checkIfValid() const;

	std::pair<double, double> m_minMax; ///< Range.
};

} /* namespace EPIC */

#endif /* BEANS_KINEMATICRANGE_H_ */

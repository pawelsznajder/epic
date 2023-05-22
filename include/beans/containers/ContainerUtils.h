/*
 * Container.h
 *
 *  Created on: Mar 24, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef BEANS_CONTAINERS_CONTAINERUTILS_H_
#define BEANS_CONTAINERS_CONTAINERUTILS_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <stddef.h>
#include <string>
#include <vector>

namespace EPIC {

namespace ContainerUtils {

/**
 * Find and parse unsigned int.
 */
size_t findAndParseUInt(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key,
        bool peg = false);

/**
 * Find and parse integer.
 */
int findAndParseInt(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key,
        bool peg = false);

/**
 * Find and parse double.
 */
double findAndParseDouble(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key,
        bool peg = false);

/**
 * Find and parse double vector.
 */
std::vector<double> findAndParseVectorDouble(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key,
        size_t nElements,
        bool peg = false);

/**
 * Find and parse string.
 */
std::string findAndParseString(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key);

/**
 * Parse std::string to unsigned int, optionally using Parsing Expression Grammar.
 */
size_t stdStringToUInt(const std::string& className, const std::string& str,
        bool peg = false);

/**
 * Parse std::string to int, optionally using Parsing Expression Grammar.
 */
int stdStringToInt(const std::string& className, const std::string& str,
        bool peg = false);

/**
 * Parse std::string to double, optionally using Parsing Expression Grammar.
 */
double stdStringToDouble(const std::string& className, const std::string& str,
        bool peg = false);
}

}

#endif /* BEANS_CONTAINERS_CONTAINERUTILS_H_ */

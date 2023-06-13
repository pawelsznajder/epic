/*
 * ContainerUtils.cpp
 *
 *  Created on: Mar 24, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#include "../../../include/beans/containers/ContainerUtils.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <stddef.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <TFormula.h>

namespace EPIC {

namespace ContainerUtils {

size_t findAndParseUInt(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key, bool peg) {

    size_t value;

    if (data.isAvailable(key)) {

        std::string valueString = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(valueString);
        value = stdStringToUInt(className, valueString, peg);

    } else {
        throw ElemUtils::CustomException(className, __func__,
                ElemUtils::Formatter() << "Key " << key << " is missing");
    }

    return value;
}

int findAndParseInt(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key, bool peg) {

    int value;

    if (data.isAvailable(key)) {

        std::string valueString = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(valueString);
        value = stdStringToInt(className, valueString, peg);

    } else {
        throw ElemUtils::CustomException(className, __func__,
                ElemUtils::Formatter() << "Key " << key << " is missing");
    }

    return value;
}

double findAndParseDouble(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key, bool peg) {

    double value;

    if (data.isAvailable(key)) {

        std::string valueString = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(valueString);
        value = stdStringToDouble(className, valueString, peg);

    } else {
        throw ElemUtils::CustomException(className, __func__,
                ElemUtils::Formatter() << "Key " << key << " is missing");
    }

    return value;
}

std::vector<double> findAndParseVectorDouble(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key,
        size_t nElements, bool peg) {

    std::vector<double> valueDouble(nElements);

    if (data.isAvailable(key)) {

        std::string valueString = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(valueString);

        std::vector<std::string> valueStrings = ElemUtils::StringUtils::split(
                valueString, '|');

        if (valueStrings.size() != nElements) {
            throw ElemUtils::CustomException(className, __func__,
                    ElemUtils::Formatter()
                            << "Can not create object from string: "
                            << valueString);
        }

        for (size_t i = 0; i < nElements; i++) {
            valueDouble.at(i) = stdStringToDouble(className, valueStrings.at(i),
                    peg);
        }

    } else {
        throw ElemUtils::CustomException(className, __func__,
                ElemUtils::Formatter() << "Key " << key << " is missing");
    }

    return valueDouble;
}

std::string findAndParseString(const std::string& className,
        const ElemUtils::Parameters& data, const std::string& key){

    std::string value;

    if (data.isAvailable(key)) {

        value = data.getLastAvailable().getString();
        ElemUtils::StringUtils::trimAll(value);

    } else {
        throw ElemUtils::CustomException(className, __func__,
                ElemUtils::Formatter() << "Key " << key << " is missing");
    }

    return value;
}

size_t stdStringToUInt(const std::string& className, const std::string& str,
        bool peg) {

    int result;

    if (peg) {
        result = size_t(stdStringToDouble(className, str, peg));

    } else {

        std::stringstream sstream(str);

        if ((sstream >> result).fail()) {
            throw ElemUtils::CustomException(className, __func__,
                    ElemUtils::Formatter()
                            << "Cast from std::string to unsigned int failed for: "
                            << str);
        }
    }

    return result;
}

int stdStringToInt(const std::string& className, const std::string& str,
        bool peg) {

    int result;

    if (peg) {
        result = int(stdStringToDouble(className, str, peg));

    } else {

        std::stringstream sstream(str);

        if ((sstream >> result).fail()) {
            throw ElemUtils::CustomException(className, __func__,
                    ElemUtils::Formatter()
                            << "Cast from std::string to int failed for: "
                            << str);
        }
    }

    return result;
}

double stdStringToDouble(const std::string& className, const std::string& str,
        bool peg) {

    double result;

    if (peg) {

        std::stringstream ss;
        ss << PARTONS::Constant::PROTON_MASS;   

        std::string strMod = std::regex_replace(str, std::regex("Mp"), ss.str());

        TFormula formula("", strMod.c_str());

        if (formula.GetNpar() != 0) {
            throw ElemUtils::CustomException(className, __func__,
                    ElemUtils::Formatter()
                            << "Cast from std::string to double failed for: "
                            << str);
        }

        result = formula.Eval(0.);

    } else {

        std::stringstream sstream(str);

        if ((sstream >> result).fail()) {
            throw ElemUtils::CustomException(className, __func__,
                    ElemUtils::Formatter()
                            << "Cast from std::string to double failed for: "
                            << str);
        }
    }

    return result;
}

}
}


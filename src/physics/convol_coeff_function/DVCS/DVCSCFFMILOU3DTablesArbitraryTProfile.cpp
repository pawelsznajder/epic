#include "../../../../include/physics/convol_coeff_function/DVCS/DVCSCFFMILOU3DTablesArbitraryTProfile.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>

namespace PARTONS {

const std::string DVCSCFFCMILOU3DTablesArbitraryTProfile::PARAMETER_NAME_T_PROFILE =
        "t_profile";

const unsigned int DVCSCFFCMILOU3DTablesArbitraryTProfile::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFCMILOU3DTablesArbitraryTProfile("DVCSCFFCMILOU3DTablesArbitraryTProfile"));

DVCSCFFCMILOU3DTablesArbitraryTProfile::DVCSCFFCMILOU3DTablesArbitraryTProfile(const std::string &className) :
        DVCSCFFCMILOU3DTables(className) {

    //assign function
    m_listOfCFFComputeFunctionAvailable.clear();

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVCSConvolCoeffFunctionModule::computeCFF));

    //t-profile
    m_tProfile = "";
}

DVCSCFFCMILOU3DTablesArbitraryTProfile::DVCSCFFCMILOU3DTablesArbitraryTProfile(const DVCSCFFCMILOU3DTablesArbitraryTProfile &other) :
        DVCSCFFCMILOU3DTables(other) {

    //t-profile
    m_tProfile = other.m_tProfile;
    m_tFormula = other.m_tFormula;
}

DVCSCFFCMILOU3DTablesArbitraryTProfile* DVCSCFFCMILOU3DTablesArbitraryTProfile::clone() const {
    return new DVCSCFFCMILOU3DTablesArbitraryTProfile(*this);
}

DVCSCFFCMILOU3DTablesArbitraryTProfile::~DVCSCFFCMILOU3DTablesArbitraryTProfile() {
}

void DVCSCFFCMILOU3DTablesArbitraryTProfile::initModule() {
    DVCSCFFCMILOU3DTables::initModule();
}

void DVCSCFFCMILOU3DTablesArbitraryTProfile::isModuleWellConfigured() {
    DVCSCFFCMILOU3DTables::isModuleWellConfigured();
}

std::complex<double> DVCSCFFCMILOU3DTablesArbitraryTProfile::computeCFF() {

    double thisT = m_t;

    m_t = 0.;
    std::complex<double> result = DVCSCFFCMILOU3DTables::computeCFF();

    result *= m_tFormula.Eval(m_xi, 0., 0., thisT);

    return std::complex<double>(0., result.imag());
}

void DVCSCFFCMILOU3DTablesArbitraryTProfile::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    DVCSCFFCMILOU3DTables::configure(parameters);

    if (parameters.isAvailable(
            DVCSCFFCMILOU3DTablesArbitraryTProfile::PARAMETER_NAME_T_PROFILE)) {

        // get name
        setTProfile(parameters.getLastAvailable().getString());
        m_tFormula = TFormula("DVCSCFFCMILOU3DTablesArbitraryTProfile::m_tFormula", m_tProfile.c_str());
    }else{
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Parameter " << DVCSCFFCMILOU3DTablesArbitraryTProfile::PARAMETER_NAME_T_PROFILE << " is missing");
    }
}

const std::string& DVCSCFFCMILOU3DTablesArbitraryTProfile::getTProfile() const {
    return m_tProfile;
}

void DVCSCFFCMILOU3DTablesArbitraryTProfile::setTProfile(const std::string& tProfile) {

    info(__func__, ElemUtils::Formatter() << "t-profile changed to: " << tProfile);

    m_tProfile = tProfile;
}


} /* namespace PARTONS */

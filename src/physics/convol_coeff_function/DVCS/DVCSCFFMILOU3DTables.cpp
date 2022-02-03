#include "../../../../include/physics/convol_coeff_function/DVCS/DVCSCFFMILOU3DTables.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
#include <stddef.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>
#include <TFile.h>
#include <TNtuple.h>

namespace PARTONS {

const std::string DVCSCFFCMILOU3DTables::PARAMETER_NAME_SET_FILE =
        "cff_set_file";

const unsigned int DVCSCFFCMILOU3DTables::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFCMILOU3DTables("DVCSCFFCMILOU3DTables"));

DVCSCFFCMILOU3DTables::DVCSCFFCMILOU3DTables(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className), m_printOutsideGridWarnings(
                true) {

    //not depend on GPD
    setIsGPDModuleDependent(false);

    //assign function
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVCSConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DVCSConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DVCSConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DVCSConvolCoeffFunctionModule::computeCFF));

    //initialize map
    m_map.insert(
            std::make_pair(GPDType::H,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));
    m_map.insert(
            std::make_pair(GPDType::E,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));
    m_map.insert(
            std::make_pair(GPDType::Ht,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));
    m_map.insert(
            std::make_pair(GPDType::Et,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));

    //paths
    m_cffSetFile = "data/DVCSCFFCMILOU3DTables/tables_GK.root";

    m_tablesLoaded = false;
}

DVCSCFFCMILOU3DTables::DVCSCFFCMILOU3DTables(const DVCSCFFCMILOU3DTables &other) :
        DVCSConvolCoeffFunctionModule(other), m_printOutsideGridWarnings(true) {

    //initialize map
    m_map.insert(
            std::make_pair(GPDType::H,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));
    m_map.insert(
            std::make_pair(GPDType::E,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));
    m_map.insert(
            std::make_pair(GPDType::Ht,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));
    m_map.insert(
            std::make_pair(GPDType::Et,
                    std::vector<std::shared_ptr<NDInterpolator_3_ML> >(8)));

    //paths
    m_cffSetFile = other.m_cffSetFile;

    m_tablesLoaded = false;
}

DVCSCFFCMILOU3DTables* DVCSCFFCMILOU3DTables::clone() const {
    return new DVCSCFFCMILOU3DTables(*this);
}

DVCSCFFCMILOU3DTables::~DVCSCFFCMILOU3DTables() {
}

void DVCSCFFCMILOU3DTables::initModule() {
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSCFFCMILOU3DTables::isModuleWellConfigured() {
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSCFFCMILOU3DTables::computeCFF() {

    //check if loaded
    if (!m_tablesLoaded) {
        loadGrids();
    }

    //get xB and scales
    double xB = xiToxB(m_xi);

    //check range
    if (!checkRange(xB, m_ranges.at(0), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);
    if (!checkRange(m_t, m_ranges.at(1), std::make_pair(false, true)))
        return std::complex<double>(0., 0.);
    if (!checkRange(m_Q2, m_ranges.at(2), std::make_pair(true, false)))
        return std::complex<double>(0., 0.);

    //arguments
    array<double, 3> args;

    args[0] = log10(xB);
    args[1] = m_t;
    args[2] = log10(m_Q2);

    //evaluate
    std::map<GPDType::Type, std::vector<std::shared_ptr<NDInterpolator_3_ML> > >::iterator it =
            m_map.find(m_currentGPDComputeType);

    double re = 0.;
    double im = 0.;

    for (size_t i = 0; i < 8; i++) {
        if (bool(i % 2)) {
            im += it->second.at(i)->interp(args.begin());
        } else {
            re += it->second.at(i)->interp(args.begin());
        }
    }

    return std::complex<double>(re, im);
}

void DVCSCFFCMILOU3DTables::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    DVCSConvolCoeffFunctionModule::configure(parameters);

    if (parameters.isAvailable(
            DVCSCFFCMILOU3DTables::PARAMETER_NAME_SET_FILE)) {

        // get name
        m_cffSetFile = parameters.getLastAvailable().getString();

        //load for the next evaluation
        m_tablesLoaded = false;
    }
}

void DVCSCFFCMILOU3DTables::loadGrids() {

    //ranges
    m_ranges = std::vector<std::pair<double, double> >(3,
            std::make_pair(0., 0.));

    //flavors
    std::vector<std::string> flavors(4);

    flavors.at(0) = "g";
    flavors.at(1) = "u";
    flavors.at(2) = "d";
    flavors.at(3) = "s";

    //load
    std::vector<std::shared_ptr<NDInterpolator_3_ML> > interpolatorsGPD;

    //H
    interpolatorsGPD.clear();

    for (size_t i = 0; i < 4; i++) {

        std::pair<std::shared_ptr<NDInterpolator_3_ML>,
                std::shared_ptr<NDInterpolator_3_ML> > interpolatorsFlavor =
                loadGrid("H", flavors.at(i));

        interpolatorsGPD.push_back(interpolatorsFlavor.first);
        interpolatorsGPD.push_back(interpolatorsFlavor.second);
    }

    m_map.find(GPDType::H)->second = interpolatorsGPD;

    //E
    interpolatorsGPD.clear();

    for (size_t i = 0; i < 4; i++) {

        std::pair<std::shared_ptr<NDInterpolator_3_ML>,
                std::shared_ptr<NDInterpolator_3_ML> > interpolatorsFlavor =
                loadGrid("E", flavors.at(i));

        interpolatorsGPD.push_back(interpolatorsFlavor.first);
        interpolatorsGPD.push_back(interpolatorsFlavor.second);
    }

    m_map.find(GPDType::E)->second = interpolatorsGPD;

    //Ht
    interpolatorsGPD.clear();

    for (size_t i = 0; i < 4; i++) {

        std::pair<std::shared_ptr<NDInterpolator_3_ML>,
                std::shared_ptr<NDInterpolator_3_ML> > interpolatorsFlavor =
                loadGrid("Ht", flavors.at(i));

        interpolatorsGPD.push_back(interpolatorsFlavor.first);
        interpolatorsGPD.push_back(interpolatorsFlavor.second);
    }

    m_map.find(GPDType::Ht)->second = interpolatorsGPD;

    //Et
    interpolatorsGPD.clear();

    for (size_t i = 0; i < 4; i++) {

        std::pair<std::shared_ptr<NDInterpolator_3_ML>,
                std::shared_ptr<NDInterpolator_3_ML> > interpolatorsFlavor =
                loadGrid("Et", flavors.at(i));

        interpolatorsGPD.push_back(interpolatorsFlavor.first);
        interpolatorsGPD.push_back(interpolatorsFlavor.second);
    }

    m_map.find(GPDType::Et)->second = interpolatorsGPD;

    //mark as true
    m_tablesLoaded = true;
}

std::pair<std::shared_ptr<NDInterpolator_3_ML>,
        std::shared_ptr<NDInterpolator_3_ML> > DVCSCFFCMILOU3DTables::loadGrid(
        const std::string& type, const std::string& flavor) {

    //read grid
    std::vector<std::vector<double> > grid = readGrid(type, flavor);

    //grid nodes
    std::vector<double> nodesXb;
    std::vector<double> nodesT;
    std::vector<double> nodesQ2;

    //iterator
    std::vector<std::vector<double> >::const_iterator it;

    //loop
    for (it = grid.begin(); it != grid.end(); it++) {

        //set nodes
        if (checkIfUnique(nodesXb, log10(it->at(0))))
            nodesXb.push_back(log10(it->at(0)));
        if (checkIfUnique(nodesT, it->at(1)))
            nodesT.push_back(it->at(1));
        if (checkIfUnique(nodesQ2, log10(it->at(2))))
            nodesQ2.push_back(log10(it->at(2)));
    }

    //sort
    std::sort(nodesXb.begin(), nodesXb.end());
    std::sort(nodesT.begin(), nodesT.end());
    std::sort(nodesQ2.begin(), nodesQ2.end());

    //range
    std::pair<double, double> rangeXb = std::make_pair(pow(10., nodesXb.at(0)),
            pow(10., nodesXb.at(nodesXb.size() - 1)));
    std::pair<double, double> rangeT = std::make_pair(nodesT.at(0),
            nodesT.at(nodesT.size() - 1));
    std::pair<double, double> rangeQ2 = std::make_pair(pow(10., nodesQ2.at(0)),
            pow(10., nodesQ2.at(nodesQ2.size() - 1)));

    checkAndSetRange(m_ranges.at(0), rangeXb);
    checkAndSetRange(m_ranges.at(1), rangeT);
    checkAndSetRange(m_ranges.at(2), rangeQ2);

    //store
    std::vector<std::vector<double>::iterator> grid_iter_list;

    grid_iter_list.push_back(nodesXb.begin());
    grid_iter_list.push_back(nodesT.begin());
    grid_iter_list.push_back(nodesQ2.begin());

    array<int, 3> grid_sizes;

    grid_sizes[0] = nodesXb.size();
    grid_sizes[1] = nodesT.size();
    grid_sizes[2] = nodesQ2.size();

    //values
    int num_elements = grid_sizes[0] * grid_sizes[1] * grid_sizes[2];

    boost::multi_array<double, 3> f_values_re(grid_sizes);
    boost::multi_array<double, 3> f_values_im(grid_sizes);
    array<int, 3> index;

    for (it = grid.begin(); it != grid.end(); it++) {

        index[0] = findIndex(nodesXb, log10(it->at(0)));
        index[1] = findIndex(nodesT, it->at(1));
        index[2] = findIndex(nodesQ2, log10(it->at(2)));

        if (index[0] == -1 || index[1] == -1 || index[2] == -1) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Node not found for xB: "
                            << log10(it->at(0)) << ", t: " << it->at(1)
                            << ", Q2: " << log10(it->at(2)));
        }

        f_values_re(index) = it->at(3);
        f_values_im(index) = it->at(4);
    }

    //status
    info(__func__,
            ElemUtils::Formatter() << "CFF " << type << "^" << flavor
                    << " processed successfully:" << " xB (min, max, nNodes): ("
                    << rangeXb.first << ", " << rangeXb.second << ", "
                    << nodesXb.size() << ")," << " t (min, max, nNodes): ("
                    << rangeT.first << ", " << rangeT.second << ", "
                    << nodesT.size() << ")," << " Q2 (min, max, nNodes): ("
                    << rangeQ2.first << ", " << rangeQ2.second << ", "
                    << nodesQ2.size() << ")");

    //initialize
    return std::make_pair(
            std::make_shared<NDInterpolator_3_ML>(grid_iter_list.begin(),
                    grid_sizes.begin(), f_values_re.data(),
                    f_values_re.data() + num_elements),
            std::make_shared<NDInterpolator_3_ML>(grid_iter_list.begin(),
                    grid_sizes.begin(), f_values_im.data(),
                    f_values_im.data() + num_elements));

}

void DVCSCFFCMILOU3DTables::checkAndSetRange(std::pair<double, double>& target,
        const std::pair<double, double>& source) {

    if (target.first != target.second) {
        if ((target.first != source.first)
                || (target.second != source.second)) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Ranges not compatible: A: ("
                            << target.first << ", " << target.second << "), B: "
                            << source.first << ", " << source.second << ")");
        }
    } else {
        target = source;
    }
}

bool DVCSCFFCMILOU3DTables::checkRange(double& value,
        const std::pair<double, double>& range,
        const std::pair<bool, bool>& freeze) {

    if (value < range.first) {

        if (m_printOutsideGridWarnings) {

            warn(__func__,
                    "Value outside grid, freeze or return zero (more of these warning will be suppressed)");
            m_printOutsideGridWarnings = false;
        }

        if (freeze.first) {
            value = range.first;
        } else {
            return false;
        }
    }

    if (value > range.second) {

        if (m_printOutsideGridWarnings) {

            warn(__func__,
                    "Value outside grid, freeze or return zero (more of these warning will be suppressed)");
            m_printOutsideGridWarnings = false;
        }

        if (freeze.second) {
            value = range.second;
        } else {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<double> > DVCSCFFCMILOU3DTables::readGrid(
        const std::string& type, const std::string& flavor) const {

    //open file
    TFile rootFile(m_cffSetFile.c_str(), "READ");

    if (rootFile.IsZombie()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unable to open file: "
                        << m_cffSetFile);
    }

    //result
    std::vector<std::vector<double> > result;

    //tree name
    std::string id = type + "_" + flavor;

    //variables
    Float_t xB, t, Q2, Re, Im;

    //get
    TNtuple* tree = (TNtuple*) rootFile.Get(id.c_str());

    tree->SetBranchAddress("xB", &xB);
    tree->SetBranchAddress("t", &t);
    tree->SetBranchAddress("Q2", &Q2);
    tree->SetBranchAddress("Re", &Re);
    tree->SetBranchAddress("Im", &Im);

    //loop
    for (Int_t i = 0; i < (Int_t) tree->GetEntries(); i++) {

        tree->GetEntry(i);

        std::vector<double> resultThis(5);

        resultThis.at(0) = xB;
        resultThis.at(1) = t;
        resultThis.at(2) = Q2;
        resultThis.at(3) = Re;
        resultThis.at(4) = Im;

        result.push_back(resultThis);
    }

    //close
    rootFile.Close();

    //return
    return result;
}

int DVCSCFFCMILOU3DTables::findIndex(const std::vector<double>& vec,
        double val) const {

    //iterator
    std::vector<double>::const_iterator it;

    //loop
    for (it = vec.begin(); it != vec.end(); it++) {
        if (val == *it)
            return (it - vec.begin());
    }

    return -1;
}

bool DVCSCFFCMILOU3DTables::checkIfUnique(const std::vector<double>& vec,
        double val) const {

    //get index
    int index = findIndex(vec, val);

    //return
    return (index == -1);
}

const std::string& DVCSCFFCMILOU3DTables::getCFFSetFile() const {
    return m_cffSetFile;
}

void DVCSCFFCMILOU3DTables::setCFFSetFile(const std::string& cffSetFile) {
    m_cffSetFile = cffSetFile;
}

double DVCSCFFCMILOU3DTables::xiToxB(double xi) const {
    return 2 * xi / (1. + xi);
}

} /* namespace PARTONS */

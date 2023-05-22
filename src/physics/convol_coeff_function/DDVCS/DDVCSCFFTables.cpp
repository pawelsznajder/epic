#include "../../../../include/physics/convol_coeff_function/DDVCS/DDVCSCFFTables.h"

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

const std::string DDVCSCFFTables::PARAMETER_NAME_SET_FILE = "cff_set_file";

const unsigned int DDVCSCFFTables::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCFFTables("DDVCSCFFTables"));

DDVCSCFFTables::DDVCSCFFTables(const std::string &className) :
        DDVCSConvolCoeffFunctionModule(className), m_printOutsideGridWarnings(
                true) {

    //not depend on GPD
    setIsGPDModuleDependent(false);

    //assign function
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DDVCSConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DDVCSConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DDVCSConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DDVCSConvolCoeffFunctionModule::computeCFF));

    //initialize map
    m_map.insert(
            std::make_pair(GPDType::H,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));
    m_map.insert(
            std::make_pair(GPDType::E,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));
    m_map.insert(
            std::make_pair(GPDType::Ht,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));
    m_map.insert(
            std::make_pair(GPDType::Et,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));

    //paths
    m_cffSetFile = "data/DDVCSCFFTables/tables_GK.root";

    m_tablesLoaded = false;
}

DDVCSCFFTables::DDVCSCFFTables(const DDVCSCFFTables &other) :
        DDVCSConvolCoeffFunctionModule(other), m_printOutsideGridWarnings(true) {

    //initialize map
    m_map.insert(
            std::make_pair(GPDType::H,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));
    m_map.insert(
            std::make_pair(GPDType::E,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));
    m_map.insert(
            std::make_pair(GPDType::Ht,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));
    m_map.insert(
            std::make_pair(GPDType::Et,
                    std::vector < std::shared_ptr<NDInterpolator_4_ML> > (2)));

    //paths
    m_cffSetFile = other.m_cffSetFile;

    m_tablesLoaded = false;
}

DDVCSCFFTables* DDVCSCFFTables::clone() const {
    return new DDVCSCFFTables(*this);
}

DDVCSCFFTables::~DDVCSCFFTables() {
}

void DDVCSCFFTables::initModule() {
    DDVCSConvolCoeffFunctionModule::initModule();
}

void DDVCSCFFTables::isModuleWellConfigured() {
    DDVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DDVCSCFFTables::computeCFF() {

    //check if loaded
    if (!m_tablesLoaded) {
        loadGrids();
    }

    //check range
    if (!checkRange(m_xi, m_ranges.at(0), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);
    if (!checkRange(m_t, m_ranges.at(1), std::make_pair(false, true)))
        return std::complex<double>(0., 0.);
    if (!checkRange(m_Q2, m_ranges.at(2), std::make_pair(true, false)))
        return std::complex<double>(0., 0.);
    if (!checkRange(m_Q2Prim, m_ranges.at(3), std::make_pair(true, false)))
        return std::complex<double>(0., 0.);

    //arguments
    array<double, 4> args;

    args[0] = log10(m_xi);
    args[1] = m_t;
    args[2] = log10(m_Q2);
    args[3] = log10(m_Q2Prim);

    //evaluate
    std::map<GPDType::Type, std::vector<std::shared_ptr<NDInterpolator_4_ML> > >::iterator it =
            m_map.find(m_currentGPDComputeType);

    double re = it->second.at(0)->interp(args.begin());
    double im = it->second.at(1)->interp(args.begin());

    return std::complex<double>(re, im);
}

void DDVCSCFFTables::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    DDVCSConvolCoeffFunctionModule::configure(parameters);

    if (parameters.isAvailable(DDVCSCFFTables::PARAMETER_NAME_SET_FILE)) {

        // get name
        m_cffSetFile = parameters.getLastAvailable().getString();

        //load for the next evaluation
        m_tablesLoaded = false;
    }
}

void DDVCSCFFTables::loadGrids() {

    //ranges
    m_ranges = std::vector < std::pair<double, double>
            > (4, std::make_pair(0., 0.));

    //load
    std::vector < std::shared_ptr<NDInterpolator_4_ML> > interpolatorsGPD;

    std::pair<std::shared_ptr<NDInterpolator_4_ML>,
            std::shared_ptr<NDInterpolator_4_ML> > interpolatorsFlavor;

    //H
    interpolatorsGPD.clear();

    interpolatorsFlavor = loadGrid("H");

    interpolatorsGPD.push_back(interpolatorsFlavor.first);
    interpolatorsGPD.push_back(interpolatorsFlavor.second);

    m_map.find(GPDType::H)->second = interpolatorsGPD;

    //E
    interpolatorsGPD.clear();

    interpolatorsFlavor = loadGrid("E");

    interpolatorsGPD.push_back(interpolatorsFlavor.first);
    interpolatorsGPD.push_back(interpolatorsFlavor.second);

    m_map.find(GPDType::E)->second = interpolatorsGPD;

    //Ht
    interpolatorsGPD.clear();

    interpolatorsFlavor = loadGrid("Ht");

    interpolatorsGPD.push_back(interpolatorsFlavor.first);
    interpolatorsGPD.push_back(interpolatorsFlavor.second);

    m_map.find(GPDType::Ht)->second = interpolatorsGPD;

    //Et
    interpolatorsGPD.clear();

    interpolatorsFlavor = loadGrid("Et");

    interpolatorsGPD.push_back(interpolatorsFlavor.first);
    interpolatorsGPD.push_back(interpolatorsFlavor.second);

    m_map.find(GPDType::Et)->second = interpolatorsGPD;

    //mark as true
    m_tablesLoaded = true;
}

std::pair<std::shared_ptr<NDInterpolator_4_ML>,
        std::shared_ptr<NDInterpolator_4_ML> > DDVCSCFFTables::loadGrid(
        const std::string& type) {

    //read grid
    std::vector < std::vector<double> > grid = readGrid(type);

    //grid nodes
    std::vector<double> nodesXi;
    std::vector<double> nodesT;
    std::vector<double> nodesQ2;
    std::vector<double> nodesQ2Prim;

    //iterator
    std::vector<std::vector<double> >::const_iterator it;

    //loop
    for (it = grid.begin(); it != grid.end(); it++) {

        //set nodes
        if (checkIfUnique(nodesXi, log10(it->at(0))))
            nodesXi.push_back(log10(it->at(0)));
        if (checkIfUnique(nodesT, it->at(1)))
            nodesT.push_back(it->at(1));
        if (checkIfUnique(nodesQ2, log10(it->at(2))))
            nodesQ2.push_back(log10(it->at(2)));
        if (checkIfUnique(nodesQ2Prim, log10(it->at(3))))
            nodesQ2Prim.push_back(log10(it->at(3)));
    }

    //sort
    std::sort(nodesXi.begin(), nodesXi.end());
    std::sort(nodesT.begin(), nodesT.end());
    std::sort(nodesQ2.begin(), nodesQ2.end());
    std::sort(nodesQ2Prim.begin(), nodesQ2Prim.end());

    //range
    std::pair<double, double> rangeXb = std::make_pair(pow(10., nodesXi.at(0)),
            pow(10., nodesXi.at(nodesXi.size() - 1)));
    std::pair<double, double> rangeT = std::make_pair(nodesT.at(0),
            nodesT.at(nodesT.size() - 1));
    std::pair<double, double> rangeQ2 = std::make_pair(pow(10., nodesQ2.at(0)),
            pow(10., nodesQ2.at(nodesQ2.size() - 1)));
    std::pair<double, double> rangeQ2Prim = std::make_pair(
            pow(10., nodesQ2Prim.at(0)),
            pow(10., nodesQ2Prim.at(nodesQ2Prim.size() - 1)));

    checkAndSetRange(m_ranges.at(0), rangeXb);
    checkAndSetRange(m_ranges.at(1), rangeT);
    checkAndSetRange(m_ranges.at(2), rangeQ2);
    checkAndSetRange(m_ranges.at(3), rangeQ2Prim);

    //store
    std::vector < std::vector<double>::iterator > grid_iter_list;

    grid_iter_list.push_back(nodesXi.begin());
    grid_iter_list.push_back(nodesT.begin());
    grid_iter_list.push_back(nodesQ2.begin());
    grid_iter_list.push_back(nodesQ2Prim.begin());

    array<int, 4> grid_sizes;

    grid_sizes[0] = nodesXi.size();
    grid_sizes[1] = nodesT.size();
    grid_sizes[2] = nodesQ2.size();
    grid_sizes[3] = nodesQ2Prim.size();

    //values
    int num_elements = grid_sizes[0] * grid_sizes[1] * grid_sizes[2]
            * grid_sizes[3];

    boost::multi_array<double, 4> f_values_re(grid_sizes);
    boost::multi_array<double, 4> f_values_im(grid_sizes);
    array<int, 4> index;

    for (it = grid.begin(); it != grid.end(); it++) {

        index[0] = findIndex(nodesXi, log10(it->at(0)));
        index[1] = findIndex(nodesT, it->at(1));
        index[2] = findIndex(nodesQ2, log10(it->at(2)));
        index[3] = findIndex(nodesQ2Prim, log10(it->at(3)));

        if (index[0] == -1 || index[1] == -1 || index[2] == -1
                || index[3] == -1) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Node not found for xi: "
                            << log10(it->at(0)) << ", t: " << it->at(1)
                            << ", Q2: " << log10(it->at(2)) << ", Q'2: "
                            << log10(it->at(3)));
        }

        f_values_re(index) = it->at(4);
        f_values_im(index) = it->at(5);
    }

    //status
    info(__func__,
            ElemUtils::Formatter() << "CFF " << type
                    << " processed successfully:" << " xi (min, max, nNodes): ("
                    << rangeXb.first << ", " << rangeXb.second << ", "
                    << nodesXi.size() << ")," << " t (min, max, nNodes): ("
                    << rangeT.first << ", " << rangeT.second << ", "
                    << nodesT.size() << ")," << " Q2 (min, max, nNodes): ("
                    << rangeQ2.first << ", " << rangeQ2.second << ", "
                    << nodesQ2.size() << ")," << " Q'2 (min, max, nNodes): ("
                    << rangeQ2Prim.first << ", " << rangeQ2Prim.second << ", "
                    << nodesQ2Prim.size() << ")");

    //initialize
    return std::make_pair(
            std::make_shared < NDInterpolator_4_ML
                    > (grid_iter_list.begin(), grid_sizes.begin(), f_values_re.data(), f_values_re.data()
                            + num_elements),
            std::make_shared < NDInterpolator_4_ML
                    > (grid_iter_list.begin(), grid_sizes.begin(), f_values_im.data(), f_values_im.data()
                            + num_elements));

}

void DDVCSCFFTables::checkAndSetRange(std::pair<double, double>& target,
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

bool DDVCSCFFTables::checkRange(double& value,
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

std::vector<std::vector<double> > DDVCSCFFTables::readGrid(
        const std::string& type) const {

    //open file
    TFile rootFile(m_cffSetFile.c_str(), "READ");

    if (rootFile.IsZombie()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unable to open file: "
                        << m_cffSetFile);
    }

    //result
    std::vector < std::vector<double> > result;

    //tree name
    std::string id = type;

    //variables
    Float_t xi, t, Q2, Q2Prim, Re, Im;

    //get
    TNtuple* tree = (TNtuple*) rootFile.Get(id.c_str());

    tree->SetBranchAddress("xi", &xi);
    tree->SetBranchAddress("t", &t);
    tree->SetBranchAddress("Q2", &Q2);
    tree->SetBranchAddress("Q2Prim", &Q2Prim);
    tree->SetBranchAddress("Re", &Re);
    tree->SetBranchAddress("Im", &Im);

    //loop
    for (Int_t i = 0; i < (Int_t) tree->GetEntries(); i++) {

        tree->GetEntry(i);

        std::vector<double> resultThis(6);

        resultThis.at(0) = xi;
        resultThis.at(1) = t;
        resultThis.at(2) = Q2;
        resultThis.at(3) = Q2Prim;
        resultThis.at(4) = Re;
        resultThis.at(5) = Im;

        result.push_back(resultThis);
    }

    //close
    rootFile.Close();

    //return
    return result;
}

int DDVCSCFFTables::findIndex(const std::vector<double>& vec,
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

bool DDVCSCFFTables::checkIfUnique(const std::vector<double>& vec,
        double val) const {

    //get index
    int index = findIndex(vec, val);

    //return
    return (index == -1);
}

const std::string& DDVCSCFFTables::getCFFSetFile() const {
    return m_cffSetFile;
}

void DDVCSCFFTables::setCFFSetFile(const std::string& cffSetFile) {
    m_cffSetFile = cffSetFile;
}

} /* namespace PARTONS */

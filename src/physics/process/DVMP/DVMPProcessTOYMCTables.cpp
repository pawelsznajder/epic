#include "../../../../include/physics/process/DVMP/DVMPProcessTOYMCTables.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/MesonType.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <partons/modules/process/ProcessModule.h>
#include <partons/utils/type/PhysicalUnit.h>
#include <Rtypes.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TObject.h>
#include <TTree.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>

namespace PARTONS {

const std::string DVMPProcessTOYMCTables::PARAMETER_NAME_GRID_FILE = "grid_file";

const unsigned int DVMPProcessTOYMCTables::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPProcessTOYMCTables("DVMPProcessTOYMCTables"));

DVMPProcessTOYMCTables::DVMPProcessTOYMCTables(const std::string &className) :
        DVMPProcessModule(className), m_interpolator(nullptr), m_tablesLoaded(
                false), m_beamEnergy(0.), m_threshold(1.E-20), m_beamEnergyTolerance(
                1.E-3), m_printOutsideGridWarnings(true) {

    //no dependent on CFFs
    setIsCCFModuleDependent(false);

    //set
    m_gridPath = "";
}

DVMPProcessTOYMCTables::DVMPProcessTOYMCTables(
        const DVMPProcessTOYMCTables& other) :
        DVMPProcessModule(other), m_interpolator(nullptr), m_tablesLoaded(
                false), m_beamEnergy(0.), m_threshold(other.m_threshold), m_beamEnergyTolerance(
                other.m_beamEnergyTolerance), m_printOutsideGridWarnings(true) {

    //copy
    m_gridPath = other.m_gridPath;
}

DVMPProcessTOYMCTables* DVMPProcessTOYMCTables::clone() const {
    return new DVMPProcessTOYMCTables(*this);
}

DVMPProcessTOYMCTables::~DVMPProcessTOYMCTables() {
}

void DVMPProcessTOYMCTables::initModule() {
    DVMPProcessModule::initModule();
}

void DVMPProcessTOYMCTables::isModuleWellConfigured() {
    DVMPProcessModule::isModuleWellConfigured();
}

void DVMPProcessTOYMCTables::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    DVMPProcessModule::configure(parameters);

    if (parameters.isAvailable(
            DVMPProcessTOYMCTables::PARAMETER_NAME_GRID_FILE)) {

        // get name
        m_gridPath = parameters.getLastAvailable().getString();

        //load for the next evaluation
        m_tablesLoaded = false;
    }
}

PhysicalType<double> DVMPProcessTOYMCTables::CrossSection() {

    //check meson type
    if (m_mesonType != MesonType::PI0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No tables for meson "
                        << MesonType(m_mesonType).toString());
    }

    //check if loaded
    if (!m_tablesLoaded) {
        loadGrid();
    }

    //check range
    if (!checkRange(m_xB, m_ranges.at(0), std::make_pair(false, false)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);
    if (!checkRange(m_t, m_ranges.at(1), std::make_pair(false, true)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);
    if (!checkRange(m_Q2, m_ranges.at(2), std::make_pair(true, false)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);
    if (!checkRange(m_phi, m_ranges.at(3), std::make_pair(false, false)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);

    //check beam energy (with some tolerance)
    if (fabs(m_E - m_beamEnergy) > m_beamEnergyTolerance) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Wrong beam energy, expected (from tables): "
                        << m_beamEnergy << " is: " << m_E);
    }

    //point
    array<double, 4> args;

    args[0] = log10(m_xB);
    args[1] = m_t;
    args[2] = log10(m_Q2);
    args[3] = m_phi;

    //interpolate
    double resultLog10 = m_interpolator->interp(args.begin());
    double result =
            (resultLog10 <= log10(m_threshold)) ?
                    (0.) : (pow(10., resultLog10));

    return PhysicalType<double>(result, PhysicalUnit::NB).makeSameUnitAs(
            PhysicalUnit::GEVm2);
}

std::vector<std::vector<double> > DVMPProcessTOYMCTables::readGrid() const {

    //open file
    TFile rootFile(m_gridPath.c_str(), "READ");

    if (rootFile.IsZombie()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unable to open file: "
                        << m_gridPath);
    }

    //result
    std::vector<std::vector<double> > result;

    //tree name
    std::string id = "pi0_cross_section";

    //variables
    Float_t xB, t, Q2, Ee, Ep, phi, cs;

    //get
    TNtuple* tree = (TNtuple*) rootFile.Get(id.c_str());

    tree->SetBranchAddress("xB", &xB);
    tree->SetBranchAddress("t", &t);
    tree->SetBranchAddress("Q2", &Q2);
    tree->SetBranchAddress("Ee", &Ee);
    tree->SetBranchAddress("Ep", &Ep);
    tree->SetBranchAddress("phi", &phi);
    tree->SetBranchAddress("cs", &cs);

    //loop
    for (Int_t i = 0; i < (Int_t) tree->GetEntries(); i++) {

        tree->GetEntry(i);

        std::vector<double> resultThis(7);

        resultThis.at(0) = xB;
        resultThis.at(1) = t;
        resultThis.at(2) = Q2;
        resultThis.at(3) = Ee;
        resultThis.at(4) = Ep;
        resultThis.at(5) = phi;
        resultThis.at(6) = cs;

        result.push_back(resultThis);
    }

    //close
    rootFile.Close();

    //return
    return result;
}

void DVMPProcessTOYMCTables::loadGrid() {

    //read grid
    std::vector<std::vector<double> > grid = readGrid();

    //grid nodes
    std::vector<double> nodesXb;
    std::vector<double> nodesT;
    std::vector<double> nodesQ2;
    std::vector<double> nodesPhi;

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
        if (checkIfUnique(nodesPhi, it->at(5)))
            nodesPhi.push_back(it->at(5));

        //beam energy
        double beamEnergy = getLeptonEnergyFixedTargetEquivalent(it->at(3),
                it->at(4));

        if (it == grid.begin()) {
            m_beamEnergy = beamEnergy;
        } else {
            if (beamEnergy != m_beamEnergy) {
                throw ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter()
                                << "Points have various beam energies, expected: "
                                << m_beamEnergy << ", is: " << beamEnergy);
            }
        }
    }

    //sort
    std::sort(nodesXb.begin(), nodesXb.end());
    std::sort(nodesT.begin(), nodesT.end());
    std::sort(nodesQ2.begin(), nodesQ2.end());
    std::sort(nodesPhi.begin(), nodesPhi.end());

    //range
    std::pair<double, double> rangeXb = std::make_pair(pow(10., nodesXb.at(0)),
            pow(10., nodesXb.at(nodesXb.size() - 1)));
    std::pair<double, double> rangeT = std::make_pair(nodesT.at(0),
            nodesT.at(nodesT.size() - 1));
    std::pair<double, double> rangeQ2 = std::make_pair(pow(10., nodesQ2.at(0)),
            pow(10., nodesQ2.at(nodesQ2.size() - 1)));
    std::pair<double, double> rangePhi = std::make_pair(nodesPhi.at(0),
            nodesPhi.at(nodesPhi.size() - 1));

    m_ranges.clear();

    m_ranges.push_back(rangeXb);
    m_ranges.push_back(rangeT);
    m_ranges.push_back(rangeQ2);
    m_ranges.push_back(rangePhi);

    //store
    std::vector<std::vector<double>::iterator> grid_iter_list;

    grid_iter_list.push_back(nodesXb.begin());
    grid_iter_list.push_back(nodesT.begin());
    grid_iter_list.push_back(nodesQ2.begin());
    grid_iter_list.push_back(nodesPhi.begin());

    array<int, 4> grid_sizes;

    grid_sizes[0] = nodesXb.size();
    grid_sizes[1] = nodesT.size();
    grid_sizes[2] = nodesQ2.size();
    grid_sizes[3] = nodesPhi.size();

    //values
    int num_elements = grid_sizes[0] * grid_sizes[1] * grid_sizes[2]
            * grid_sizes[3];

    boost::multi_array<double, 4> f_values(grid_sizes);
    array<int, 4> index;

    for (it = grid.begin(); it != grid.end(); it++) {

        index[0] = findIndex(nodesXb, log10(it->at(0)));
        index[1] = findIndex(nodesT, it->at(1));
        index[2] = findIndex(nodesQ2, log10(it->at(2)));
        index[3] = findIndex(nodesPhi, it->at(5));

        if (index[0] == -1 || index[1] == -1 || index[2] == -1
                || index[3] == -1) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Node not found for xB: "
                            << log10(it->at(0)) << ", t: " << it->at(1)
                            << ", Q2: " << log10(it->at(2)) << ", phi: "
                            << it->at(3));
        }

        f_values(index) =
                (it->at(6) <= m_threshold) ?
                        (log10(m_threshold)) : (log10(it->at(6)));
    }

    //status
    info(__func__,
            ElemUtils::Formatter() << "Grid" << m_gridPath
                    << " processed successfully:" << " xB (min, max, nNodes): ("
                    << rangeXb.first << ", " << rangeXb.second << ", "
                    << nodesXb.size() << ")," << " t (min, max, nNodes): ("
                    << rangeT.first << ", " << rangeT.second << ", "
                    << nodesT.size() << ")," << " Q2 (min, max, nNodes): ("
                    << rangeQ2.first << ", " << rangeQ2.second << ", "
                    << nodesQ2.size() << ")," << " phi (min, max, nNodes): ("
                    << rangePhi.first << ", " << rangePhi.second << ", "
                    << nodesPhi.size() << ")");

    //initialize
    m_interpolator = std::make_shared<NDInterpolator_4_ML>(
            grid_iter_list.begin(), grid_sizes.begin(), f_values.data(),
            f_values.data() + num_elements);

    //mark as true
    m_tablesLoaded = true;
}

bool DVMPProcessTOYMCTables::checkRange(double& value,
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

int DVMPProcessTOYMCTables::findIndex(const std::vector<double>& vec,
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

bool DVMPProcessTOYMCTables::checkIfUnique(const std::vector<double>& vec,
        double val) const {

    //get index
    int index = findIndex(vec, val);

    //return
    return (index == -1);
}

double DVMPProcessTOYMCTables::getLeptonEnergyFixedTargetEquivalent(double E1,
        double E2) const {

    double m1 = PARTONS::Constant::ELECTRON_MASS;
    double m2 = PARTONS::Constant::PROTON_MASS;

    double p1 = sqrt(pow(E1, 2) - pow(m1, 2));
    double p2 = sqrt(pow(E2, 2) - pow(m2, 2));

    return (E1 * E2 + p1 * p2) / m2;
}

const std::string& DVMPProcessTOYMCTables::getGridPath() const {
    return m_gridPath;
}

void DVMPProcessTOYMCTables::setGridPath(const std::string& gridFile) {
    m_gridPath = gridFile;
}

} /* namespace PARTONS */

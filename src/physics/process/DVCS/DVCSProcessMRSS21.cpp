#include "../../../../include/physics/process/DVCS/DVCSProcessMRSS21.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <complex>
#include <sstream>
#include <TFile.h>
#include <TNtuple.h>

#include <partons/beans/gpd/GPDType.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <partons/utils/type/PhysicalUnit.h>

namespace PARTONS {

const std::string DVCSProcessMRSS21::PARAMETER_NAME_GRID_FILE = "grid_file";

const unsigned int DVCSProcessMRSS21::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSProcessMRSS21("DVCSProcessMRSS21"));

/*--------------------------------------- Constructors ---------------------------------*/

DVCSProcessMRSS21::DVCSProcessMRSS21(const std::string &className) :
        DVCSProcessModule(className), m_printOutsideGridWarnings(true) {

    //mark that this module does not need CFF one
    setIsCCFModuleDependent(false);

    //paths
    m_gridFile = "data/DVCSCrossSectionMRSS/tables_MRSS.root";

    m_tablesLoaded = false;
}

/*-------------------------------------- Destructor ------------------------------------*/

DVCSProcessMRSS21::~DVCSProcessMRSS21() {
}

DVCSProcessMRSS21::DVCSProcessMRSS21(const DVCSProcessMRSS21& other) :
        DVCSProcessModule(other), m_printOutsideGridWarnings(true) {

    //paths
    m_gridFile = other.m_gridFile;

    m_tablesLoaded = false;
}

DVCSProcessMRSS21* DVCSProcessMRSS21::clone() const {
    return new DVCSProcessMRSS21(*this);
}

void DVCSProcessMRSS21::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    DVCSProcessModule::configure(parameters);

    if (parameters.isAvailable(DVCSProcessMRSS21::PARAMETER_NAME_GRID_FILE)) {

        // get name
        m_gridFile = parameters.getLastAvailable().getString();

        //load for the next evaluation
        m_tablesLoaded = false;
    }
}

void DVCSProcessMRSS21::initModule() {

    // Init mother class
    DVCSProcessModule::initModule();

    //check if loaded
    if (!m_tablesLoaded) {
        loadGrids();
    }
}

void DVCSProcessMRSS21::isModuleWellConfigured() {

    //check mother class
    DVCSProcessModule::isModuleWellConfigured();
}

const std::string& DVCSProcessMRSS21::getGridFile() const {
    return m_gridFile;
}

void DVCSProcessMRSS21::setGridFile(const std::string& gridFile) {
    m_gridFile = gridFile;
}

void DVCSProcessMRSS21::loadGrids() {

    //ranges
    m_ranges = std::vector<std::pair<double, double> >(3,
            std::make_pair(0., 0.));

    //number of xs grids
    size_t nXsGrids = 4;

    //load
    std::vector<std::shared_ptr<NDInterpolator_3_ML> > interpolatorsGPD;

    //init
    m_interpolators = std::vector<std::shared_ptr<NDInterpolator_3_ML> >(
            nXsGrids);

    //load
    for (size_t i = 0; i < nXsGrids; i++) {
        m_interpolators.at(i) = loadGrid(i);
    }

    //mark as true
    m_tablesLoaded = true;
}

std::shared_ptr<NDInterpolator_3_ML> DVCSProcessMRSS21::loadGrid(
        size_t xsIndex) {

    //read grid
    std::vector<std::vector<double> > grid = readGrid(xsIndex);

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

    boost::multi_array<double, 3> f_values(grid_sizes);
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

        f_values(index) = it->at(3);
    }

    //status
    info(__func__,
            ElemUtils::Formatter() << "xs: " << xsIndex
                    << " processed successfully:" << " xB (min, max, nNodes): ("
                    << rangeXb.first << ", " << rangeXb.second << ", "
                    << nodesXb.size() << ")," << " t (min, max, nNodes): ("
                    << rangeT.first << ", " << rangeT.second << ", "
                    << nodesT.size() << ")," << " Q2 (min, max, nNodes): ("
                    << rangeQ2.first << ", " << rangeQ2.second << ", "
                    << nodesQ2.size() << ")");

    //initialize
    return std::make_shared<NDInterpolator_3_ML>(grid_iter_list.begin(),
            grid_sizes.begin(), f_values.data(), f_values.data() + num_elements);
}

std::vector<std::vector<double> > DVCSProcessMRSS21::readGrid(
        size_t xsIndex) const {

    //open file
    TFile rootFile(m_gridFile.c_str(), "READ");

    if (rootFile.IsZombie()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unable to open file: "
                        << m_gridFile);
    }

    //result
    std::vector<std::vector<double> > result;

    //variables
    Float_t xB, t, Q2, xs[4];

    //get
    TNtuple* tree = (TNtuple*) rootFile.Get("xsections");

    tree->SetBranchAddress("xB", &xB);
    tree->SetBranchAddress("t", &t);
    tree->SetBranchAddress("Q2", &Q2);
    tree->SetBranchAddress("xs0", &xs[0]);
    tree->SetBranchAddress("xs1", &xs[1]);
    tree->SetBranchAddress("xs2", &xs[2]);
    tree->SetBranchAddress("xs3", &xs[3]);

    //loop
    for (Int_t i = 0; i < (Int_t) tree->GetEntries(); i++) {

        tree->GetEntry(i);

        std::vector<double> resultThis(4);

        resultThis.at(0) = xB;
        resultThis.at(1) = t;
        resultThis.at(2) = Q2;

	if(xs[0] < 0. || xs[1] < 0.){
        	throw ElemUtils::CustomException(getClassName(), __func__,
        		ElemUtils::Formatter() << "Negative value of xs0 or xs1, " << xs[0] << ", " << xs[1]);
	}

	switch(xsIndex){

		case 0: {
        		resultThis.at(3) = log10(xs[0]);
			break;
		}

		case 1: {
        		resultThis.at(3) = log10(xs[1]);
			break;
		}

		case 2: {
        		resultThis.at(3) = xs[2]/(xs[0]+xs[1]);
			break;
		}

		case 3: {
        		resultThis.at(3) = xs[3]/(xs[0]+xs[1]);
			break;
		}
	}

        result.push_back(resultThis);
    }

    //close
    rootFile.Close();

    //return
    return result;
}

void DVCSProcessMRSS21::checkAndSetRange(std::pair<double, double>& target,
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

bool DVCSProcessMRSS21::checkRange(double& value,
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

int DVCSProcessMRSS21::findIndex(const std::vector<double>& vec,
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

bool DVCSProcessMRSS21::checkIfUnique(const std::vector<double>& vec,
        double val) const {

    //get index
    int index = findIndex(vec, val);

    //return
    return (index == -1);
}

PhysicalType<double> DVCSProcessMRSS21::CrossSectionBH() {
    return PhysicalType<double>(0., PhysicalUnit::GEVm2);
}

PhysicalType<double> DVCSProcessMRSS21::CrossSectionVCS() {

    //check if loaded
    if (!m_tablesLoaded) {
        loadGrids();
    }

    //check range
    if (!checkRange(m_xB, m_ranges.at(0), std::make_pair(false, false)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);
    if (!checkRange(m_t, m_ranges.at(1), std::make_pair(false, true)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);
    if (!checkRange(m_Q2, m_ranges.at(2), std::make_pair(true, false)))
        return PhysicalType<double>(0., PhysicalUnit::GEVm2);

    //arguments
    array<double, 3> args;

    args[0] = log10(m_xB);
    args[1] = m_t;
    args[2] = log10(m_Q2);

    //number of xs grids
    size_t nXsGrids = 4;

    //values
    std::vector<double> xsValues(nXsGrids);

    //evaluate
    for (size_t i = 0; i < nXsGrids; i++) {
        xsValues.at(i) = m_interpolators.at(i)->interp(args.begin());
    }

    //rescrumble 
    xsValues.at(0) = pow(10., xsValues.at(0));
    xsValues.at(1) = pow(10., xsValues.at(1));
    xsValues.at(2) = xsValues.at(2) * (xsValues.at(0) + xsValues.at(1));
    xsValues.at(3) = xsValues.at(3) * (xsValues.at(0) + xsValues.at(1));

    //evaluate y
    double y = m_Q2 / (2 * Constant::PROTON_MASS * m_xB * m_E);

    //get value
    double value = (1. - y) * xsValues.at(0)
            + (1. + pow(1. - y, 2)) / 4. * xsValues.at(1)
            + sqrt(2.) / 2. * (2. - y) * sqrt(1. - y) * xsValues.at(2)
                    * cos(m_phi) + (1. - y) * xsValues.at(3) * cos(2 * m_phi);

    //change to GeV units
    value /= Constant::CONV_GEVm2_TO_NBARN;

    //include artificial phiS dep.
    value /= 2 * M_PI;

  //  std::cout << value << std::endl;

    return PhysicalType<double>(value, PhysicalUnit::GEVm2);
}

PhysicalType<double> DVCSProcessMRSS21::CrossSectionInterf() {
    return PhysicalType<double>(0., PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */

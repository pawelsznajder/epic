#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <getopt.h>
#include <partons/Partons.h>
#include <QtCore/qcoreapplication.h>
#include <stddef.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>

#include <partons/services/automation/AutomationService.h>
#include <partons/ServiceObjectRegistry.h>

#include "../include/automation/MonteCarloScenario.h"
#include "../include/automation/MonteCarloTask.h"
#include "../include/Epic.h"
#include "../include/managers/RandomSeedManager.h"
#include "../include/managers/ServiceObjectRegistry.h"
#include "../include/services/AutomationService.h"
#include "../include/services/DVCSGeneratorService.h"
#include "../include/services/TCSGeneratorService.h"
#include "../include/services/DVMPGeneratorService.h"
#include "../include/services/GAM2GeneratorService.h"
#include "../include/services/DDVCSGeneratorService.h"
#include "../include/services/GeneratorService.h"

void printHelp(const std::string& path) {

    std::cout << "Usage: " << std::endl;
    std::cout << "\t" << path << " [OPTIONS]" << std::endl;
    std::cout << std::endl;

    std::cout << "Mendatory options:" << std::endl;
    std::cout << "\t--scenario=PATH\t\t" << "path to input scenario"
            << std::endl;

    std::cout << std::endl;

    std::cout << "Optional:" << std::endl;
    std::cout << "\t--seed=UINT\t\t" << "initial random seed" << std::endl;
    std::cout << "\t--partons\t\t" << "play scenario directly in PARTONS" << std::endl;
    std::cout << "\t--help, -h\t\t" << "print this help" << std::endl;

    std::cout << std::endl;

    std::cout << "For more information see https://pawelsznajder.github.io/epic and Eur. Phys. J. C 82 (2022) 9, 819."
            << std::endl;
}

/*
 * Main function.
 */
int main(int argc, char **argv) {

    // Initialize Qt
    QCoreApplication a(argc, argv);

    // Pointer to application
    EPIC::Epic* pEpic = nullptr;
    PARTONS::Partons* pPartons = nullptr;

    // Try.
    try {

        // Variables
        std::pair<bool, std::string> scenarioPath = std::make_pair(false, "");
        std::pair<bool, size_t> seed = std::make_pair(false, 0);
        bool isPartons = false;

        // Parse parameters
        static struct option longOptions[] = { { "help", no_argument, 0, 'h' },
                { "scenario", required_argument, 0, 0 }, { "seed",
                        required_argument, 0, 0 }, { "partons", no_argument, 0, 0 }};

        for (;;) {

            int optionIndex;
            int c = getopt_long(argc, argv, "h", longOptions, &optionIndex);

            if (c == -1)
                break;

            switch (c) {

            case 'h':

                printHelp(argv[0]);
                break;

            default:

                if (std::string(longOptions[optionIndex].name) == "scenario") {
                    scenarioPath = std::make_pair(true, std::string(optarg));
                }

                else if (std::string(longOptions[optionIndex].name) == "seed") {
                    seed = std::make_pair(true, std::atoi(optarg));
                }

                else if (std::string(longOptions[optionIndex].name) == "partons") {
                    isPartons = true; 
                }

                else {

                    std::cerr << argv[0] << ": parsing input option failed"
                            << std::endl;
                    exit(0);
                }

                break;
            }
        }

        // Check if all set
        if (!scenarioPath.first) {
            std::cerr << argv[0] << ": path to xml scenario is missing"
                    << std::endl;
            printHelp(argv[0]);
            exit(0);
        }

        if (!isPartons && !seed.first) {
            std::cerr << argv[0] << ": initial random seed is missing"
                    << std::endl;
            printHelp(argv[0]);
            exit(0);
        }

        // PARTONS =====================================================================
        
        if(isPartons){

            // Initialize application
            PARTONS::Partons* pPartons = PARTONS::Partons::getInstance();
            pPartons->init(argc, argv);

            // Parser 
            PARTONS::AutomationService* pAutomationService =
                pPartons->getServiceObjectRegistry()->getAutomationService();

            PARTONS::Scenario* pScenario = pAutomationService->parseXMLFile(
                scenarioPath.second);

            pAutomationService->playScenario(pScenario);
        }

        // GENERATOR ===================================================================
        
        else{

            // Initialize application
            pEpic = EPIC::Epic::getInstance();
            pEpic->init(argc, argv);

            pEpic->getRandomSeedManager()->setSeedCount(seed.second);

            // Parser
            std::shared_ptr<EPIC::MonteCarloScenario> scenario =
                    EPIC::AutomationService::getInstance()->parseXMLFile(
                            scenarioPath.second);

            std::vector<EPIC::MonteCarloTask>::const_iterator it;

            for (it = scenario->getTasks().begin();
                    it != scenario->getTasks().end(); it++) {

                if (it->getServiceName() == "DVCSGeneratorService") {

                    EPIC::DVCSGeneratorService *generatorService =
                            pEpic->getServiceObjectRegistry()->getDVCSGeneratorService();

                    generatorService->setScenarioDescription(
                            scenario->getDescription());
                    generatorService->setScenarioDate(scenario->getDate());

                    generatorService->computeTask(*it);
                }

                if (it->getServiceName() == "TCSGeneratorService") {

                    EPIC::TCSGeneratorService *generatorService =
                            pEpic->getServiceObjectRegistry()->getTCSGeneratorService();

                    generatorService->setScenarioDescription(
                            scenario->getDescription());
                    generatorService->setScenarioDate(scenario->getDate());

                    generatorService->computeTask(*it);
                }

                if (it->getServiceName() == "DVMPGeneratorService") {

                    EPIC::DVMPGeneratorService *generatorService =
                            pEpic->getServiceObjectRegistry()->getDVMPGeneratorService();

                    generatorService->setScenarioDescription(
                            scenario->getDescription());
                    generatorService->setScenarioDate(scenario->getDate());

                    generatorService->computeTask(*it);
                }

                if (it->getServiceName() == "GAM2GeneratorService") {

                    EPIC::GAM2GeneratorService *generatorService =
                            pEpic->getServiceObjectRegistry()->getGAM2GeneratorService();

                    generatorService->setScenarioDescription(
                            scenario->getDescription());
                    generatorService->setScenarioDate(scenario->getDate());

                    generatorService->computeTask(*it);
                }

                if (it->getServiceName() == "DDVCSGeneratorService") {

                    EPIC::DDVCSGeneratorService *generatorService =
                              pEpic->getServiceObjectRegistry()->getDDVCSGeneratorService();

                    generatorService->setScenarioDescription(
                             scenario->getDescription());
                    generatorService->setScenarioDate(scenario->getDate());

                    generatorService->computeTask(*it);
                }
            }
        }
    }
    // Catch ElemUtils exceptions.
    catch (const ElemUtils::CustomException &e) {

        // Display what happened
        PARTONS::Partons::getInstance()->getLoggerManager()->error(e);

        // Close application properly
        if (pEpic) {
            pEpic->close();
        }

        if (pPartons) {
            pPartons->close();
        }
    }
    // Catch std exceptions.
    catch (const std::exception &e) {

        // Display what happened
        PARTONS::Partons::getInstance()->getLoggerManager()->error("main",
                __func__, e.what());

        // Close application properly
        if (pEpic) {
            pEpic->close();
        }

        if (pPartons) {
            pPartons->close();
        }
    }

    // Close  application properly
    if (pEpic) {
        pEpic->close();
    }

    if (pPartons) {
        pPartons->close();
    }

    //sleep (allows to flush the logger running in independent thread)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}

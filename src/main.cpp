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
#include "../include/services/GeneratorService.h"

void printHelp(const std::string& path) {

    std::cout << "Usage: " << std::endl;
    std::cout << "\t" << path << " [OPTIONS]" << std::endl;
    std::cout << std::endl;

    std::cout << "Mendatory options:" << std::endl;
    std::cout << "\t--scenario=PATH\t\t" << "path to input scenario"
            << std::endl;
    std::cout << "\t--seed=UINT\t\t" << "initial random seed" << std::endl;

    std::cout << std::endl;

    std::cout << "Optional:" << std::endl;
    std::cout << "\t-h, --help\t\t" << "print this help" << std::endl;

    std::cout << std::endl;

    std::cout << "For more information see http://XXX and Nature XXX."
            << std::endl;
}

/*
 * Main function.
 */
int main(int argc, char **argv) {

    // Initialize Qt
    QCoreApplication a(argc, argv);

    // Pointer to application
    EPIC::Epic *pEpic = nullptr;

    // Try.
    try {

        // Variables
        std::pair<bool, std::string> scenarioPath = std::make_pair(false, "");
        std::pair<bool, size_t> seed = std::make_pair(false, 0);

        // Parse parameters
        static struct option longOptions[] = { { "help", no_argument, 0, 'h' },
                { "scenario", required_argument, 0, 0 }, { "seed",
                        required_argument, 0, 0 } };

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

        if (!seed.first) {
            std::cerr << argv[0] << ": initial random seed is missing"
                    << std::endl;
            printHelp(argv[0]);
            exit(0);
        }

        // Initialize application
        pEpic = EPIC::Epic::getInstance();
        pEpic->init(argc, argv);

        pEpic->getRandomSeedManager()->setSeedCount(seed.second);

        /*
         EPIC::TestModuleObjectFactory *pTestModuleObjectFactory =
         pEpic->getModuleObjectFactory();

         std::shared_ptr<EPIC::RandomNumberModule> x =
         pTestModuleObjectFactory->newRandomNumberModule(
         EPIC::RandomNumberGSL::classId);
         std::shared_ptr<EPIC::RandomNumberModule> y =
         pTestModuleObjectFactory->newRandomNumberModule(
         EPIC::RandomNumberGSL::classId);

         std::cout << "rnd: " << x->diceFlat() << std::endl;

         y->configure(ElemUtils::Parameter(
         EPIC::RandomNumberGSL::PARAMETER_NAME_ALGORITHM_NAME, "ranlux389"));

         std::cout << "rnd: " << y->diceFlat() << std::endl;

         EPIC::TestingService::getInstance()->runTest();

         // test scenario
         PARTONS::AutomationService *pAutomationService =
         PARTONS::Partons::getInstance()
         ->getServiceObjectRegistry()
         ->getAutomationService();

         PARTONS::Scenario *pScenario =
         pAutomationService->parseXMLFile("test.xml");

         EPIC::DVCSGeneratorService::getInstance()->getWriterModuleFromTask(
         pScenario->getTask(0));
         EPIC::DVCSGeneratorService::getInstance()->getKinematicModuleFromTask(
         pScenario->getTask(0));

         // writer
         std::shared_ptr<EPIC::WriterModule> w =
         pTestModuleObjectFactory->newWriterModule(EPIC::WriterHepMC3::classId);

         w->configure(ElemUtils::Parameter(EPIC::WriterModule::PARAMETER_NAME_PATH,
         "test.txt"));
         w->configure(ElemUtils::Parameter(
         EPIC::WriterHepMC3::PARAMETER_NAME_HEPMC3_WRITER_TYPE, "ascii"));

         EPIC::Event e;

         std::shared_ptr<EPIC::Particle> par1 = std::make_shared<EPIC::Particle>(
         EPIC::ParticleType::ELECTRON, TVector3(1, 2, 3));
         std::shared_ptr<EPIC::Particle> par2 = std::make_shared<EPIC::Particle>(
         EPIC::ParticleType::POSITRON, TVector3(4, 5, 6));
         std::shared_ptr<EPIC::Particle> par3 = std::make_shared<EPIC::Particle>(
         EPIC::ParticleType::PI0, TVector3(7, 8, 9));

         std::vector<std::shared_ptr<EPIC::Particle>> parAll;

         parAll.push_back(par1);
         parAll.push_back(par2);
         parAll.push_back(par3);

         EPIC::Vertex v1;

         v1.addParticleIn(par1);
         v1.addParticleIn(par2);
         v1.addParticleOut(par3);

         std::vector<EPIC::Vertex> vAll;

         vAll.push_back(v1);

         e.setParticles(parAll);
         e.setVertices(vAll);
         e.setBeamParticles(par1, par2);

         e.addAttribute(EPIC::EventAttributeType::ID, 888888);
         e.addAttribute(EPIC::EventAttributeType::CROSS_SECTION, 0.1);

         w->open();
         w->write(e);
         w->close();

         std::cout << e.toString() << std::endl;
         */

        //    EPIC::DVCSGeneratorService::getInstance()->getKinematicRangesFromTask(
        //        pScenario->getTask(0));
        //    EPIC::DVCSGeneratorService::getInstance()
        //        ->getExperimentalConditionsFromTask(pScenario->getTask(0));
        //    EPIC::DVCSGeneratorService::getInstance()->getProcessModuleFromTask(
        //           pScenario->getTask(0));
        // GENERATOR ===================================================================
        // parser
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
    }
    // Catch std exceptions.
    catch (const std::exception &e) {

        // Display what happened
        PARTONS::Partons::getInstance()->getLoggerManager()->error("main",
                __func__, e.what());

        // Close PARTONS application properly
        if (pEpic) {
            pEpic->close();
        }
    }

    // Close  application properly
    if (pEpic) {
        pEpic->close();
    }

    return 0;
}

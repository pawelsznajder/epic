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
#include "../include/services/DVMPGeneratorService.h"
#include "../include/services/GeneratorService.h"
#include "../include/services/TCSGeneratorService.h"

#include "../include/modules/kinematic/GAM2/GAM2KinematicModule.h"

/*
 * Main function.
 */
int main(int argc, char **argv) {

    // Initialize Qt
    QCoreApplication a(argc, argv);

    // Pointer to application
    EPIC::Epic *pEpic = nullptr;

    try {

        // Initialize application
        pEpic = EPIC::Epic::getInstance();
        pEpic->init(argc, argv);

        //use EPIC namespace fromo now on
        using namespace EPIC;

        //get module from factory
        std::shared_ptr<GAM2KinematicModule> gam2KinematicModule =
                pEpic->getModuleObjectFactory()->newGAM2KinematicModule(
                        "GAM2KinematicDefault");

        //create experimental conditions (information about beams / initial particles)
        ExperimentalConditions experimentalConditions;
        experimentalConditions.setLeptonEnergy(10.);

        //create kinematics
        GAM2Kinematic gam2Kinematic;
        gam2Kinematic.setT(-0.2);
        gam2Kinematic.setUPrim(-0.1);
        gam2Kinematic.setMgg2(-0.1);
        gam2Kinematic.setE(experimentalConditions.getLeptonEnergy()); //the same as in ExperimentalConditions
        gam2Kinematic.setPhi(0.); //ignore this angle for now

        //evaluate
        Event event = gam2KinematicModule->evaluate(experimentalConditions, gam2Kinematic);

        //print
        std::cout << event.toString() << std::endl;

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

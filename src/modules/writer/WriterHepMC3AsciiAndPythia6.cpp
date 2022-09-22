/*
 * WriterHepMC3AsciiAndPythia6.cpp
 *
 *  Created on: Feb 11, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#include "../../../include/modules/writer/WriterHepMC3AsciiAndPythia6.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <partons/BaseObjectRegistry.h>
#include <stddef.h>
#include <TLorentzVector.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <utility>

#include "../../../include/Epic.h"
#include "../../../include/managers/ModuleObjectFactory.h"
#include "../../../include/modules/writer/WriterHepMC3.h"
#include "../../../include/modules/writer/WriterHepMC3Type.h"
#include "../../../include/modules/writer/WriterPythia6.h"

namespace EPIC {

const unsigned int WriterHepMC3AsciiAndPythia6::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(
                new WriterHepMC3AsciiAndPythia6("WriterHepMC3AsciiAndPythia6"));

WriterHepMC3AsciiAndPythia6::WriterHepMC3AsciiAndPythia6(
        const std::string &className) :
        WriterModule(className) {

    m_pWriterHepMC3 = nullptr;
    m_pWriterPythia6 = nullptr;
}

WriterHepMC3AsciiAndPythia6::WriterHepMC3AsciiAndPythia6(
        const WriterHepMC3AsciiAndPythia6 &other) :
        WriterModule(other) {

    m_pWriterHepMC3 = nullptr;
    m_pWriterPythia6 = nullptr;
}

WriterHepMC3AsciiAndPythia6::~WriterHepMC3AsciiAndPythia6() {
}

WriterHepMC3AsciiAndPythia6 *WriterHepMC3AsciiAndPythia6::clone() const {
    return new WriterHepMC3AsciiAndPythia6(*this);
}

void WriterHepMC3AsciiAndPythia6::configure(
        const ElemUtils::Parameters &parameters) {

    // run for mother
    WriterModule::configure(parameters);
}

void WriterHepMC3AsciiAndPythia6::open() {

    // check path
    if (m_path.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Path not set");
    }

    //create
    if (m_pWriterHepMC3 == nullptr) {
        m_pWriterHepMC3 =
                Epic::getInstance()->getModuleObjectFactory()->newWriterModule(
                        WriterHepMC3::classId);
        std::static_pointer_cast < WriterHepMC3
                > (m_pWriterHepMC3)->setWriterHepMC3Type(
                        WriterHepMC3Type::ASCII);
    }

    if (m_pWriterPythia6 == nullptr) {
        m_pWriterPythia6 =
                Epic::getInstance()->getModuleObjectFactory()->newWriterModule(
                        WriterPythia6::classId);
    }

    //open
    m_pWriterHepMC3->setPath(m_path + ".HepMC3");
    m_pWriterHepMC3->open();

    m_pWriterPythia6->setPath(m_path + ".Pythia6");
    m_pWriterPythia6->open();
}

void WriterHepMC3AsciiAndPythia6::saveGenerationInformation(
        const GenerationInformation& generationInformation) {

    m_pWriterHepMC3->saveGenerationInformation(generationInformation);
    m_pWriterPythia6->saveGenerationInformation(generationInformation);
}

void WriterHepMC3AsciiAndPythia6::close() {

    m_pWriterHepMC3->close();
    m_pWriterPythia6->close();
}

void WriterHepMC3AsciiAndPythia6::write(const Event &event) {

    m_pWriterHepMC3->write(event);
    m_pWriterPythia6->write(event);
}

void WriterHepMC3AsciiAndPythia6::write(const std::vector<Event> &events) {

    for (std::vector<Event>::const_iterator it = events.begin();
            it != events.end(); it++) {
        write(*it);
    }
}

} /* namespace EPIC */

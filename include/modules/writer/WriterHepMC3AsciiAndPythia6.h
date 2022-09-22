/*
 * WriterHepMC3AsciiAndPythia6.h
 *
 *  Created on: Jun 06, 2022
 *      Author: Pawel Sznajder (NCBJ) 
 * 	Author: Alex Jentsch (BNL)
 */

#ifndef MODULES_WRITER_WriterHepMC3AsciiAndPythia6_H_
#define MODULES_WRITER_WriterHepMC3AsciiAndPythia6_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <fstream>
#include <locale>
#include <string>
#include <vector>

#include "../../beans/physics/Event.h"
#include "WriterModule.h"

namespace EPIC {
struct WriterHepMC3AsciiAndPythia6NoSeparator;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class WriterHepMC3AsciiAndPythia6
 *
 * @brief Event writer printing to two files: one in HEPMC3 format (Ascii), second in Pythia6 format.
 *
 * Module dedicated to writing events. It utilizes both WriterHepMC3 and WriterPythia6 formats.
 */
class WriterHepMC3AsciiAndPythia6: public WriterModule {

public:

	static const unsigned int classId; ///< Unique ID to automatically register
									   /// the class in the registry.

	/**
	 * Default constructor.
	 */
	WriterHepMC3AsciiAndPythia6(const std::string &className);

	/**
	 * Copy constructor.
	 */
	WriterHepMC3AsciiAndPythia6(const WriterHepMC3AsciiAndPythia6 &other);

	/**
	 * Destructor.
	 */
	virtual ~WriterHepMC3AsciiAndPythia6();

	virtual WriterHepMC3AsciiAndPythia6 *clone() const;
	virtual void configure(const ElemUtils::Parameters &parameters);

	virtual void open();
	virtual void saveGenerationInformation(
			const GenerationInformation& generationInformation);
	virtual void close();
	virtual void write(const Event &event);
	virtual void write(const std::vector<Event> &events);

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

private:

	std::shared_ptr<WriterModule> m_pWriterHepMC3; ///< Pointer to pWriterHepMC3 module.
	std::shared_ptr<WriterModule> m_pWriterPythia6; ///< Pointer to WriterPythia6 module.
};


} /* namespace EPIC */

#endif /* MODULES_WRITER_WriterHepMC3AsciiAndPythia6_H_ */

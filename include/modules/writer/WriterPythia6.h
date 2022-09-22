/*
 * WriterPythia6.h
 *
 *  Created on: Jun 06, 2022
 *      Author: Pawel Sznajder (NCBJ) 
 * 	Author: Alex Jentsch (BNL)
 */

#ifndef MODULES_WRITER_WRITERPYTHIA6_H_
#define MODULES_WRITER_WRITERPYTHIA6_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <fstream>
#include <locale>
#include <string>
#include <vector>

#include "../../beans/physics/Event.h"
#include "WriterModule.h"

namespace EPIC {
struct WriterPythia6NoSeparator;
} /* namespace EPIC */

namespace EPIC {

/**
 * @class WriterPythia6
 *
 * @brief Event writer to match PYTHIA 6 format.
 *
 * Module dedicated to writing events. It gives output compatible with that of PYTHIA 6. Auxiliary information is stored in .log file. 
 */
class WriterPythia6: public WriterModule {

public:

	static const unsigned int classId; ///< Unique ID to automatically register
									   /// the class in the registry.

	/**
	 * Default constructor.
	 */
	WriterPythia6(const std::string &className);

	/**
	 * Copy constructor.
	 */
	WriterPythia6(const WriterPythia6 &other);

	/**
	 * Destructor.
	 */
	virtual ~WriterPythia6();

	virtual WriterPythia6 *clone() const;
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

    /**
     * Get particle code.
     */
    inline int getParticleCode(ParticleCodeType::Type type) const;

	inline std::string trailSign(double v);

	std::ofstream m_ofstream; ///< Output stream used to save events.
	std::stringstream m_trailSign; ///< Used to trail sign.
	WriterPythia6NoSeparator* m_writerPythia6NoSeparator; ///< For proper locale.
};

/**
 * @class WriterPythia6NoSeparator
 *
 * @brief Structure to avoid comma separators.
 *
 * This structure is used to avoid comma separators in decimal numbers.
 */
struct WriterPythia6NoSeparator: std::numpunct<char> {
protected:
	virtual string_type do_grouping() const {
		return "\000";
	}
};

} /* namespace EPIC */

#endif /* MODULES_WRITER_WRITERPYTHIA6_H_ */

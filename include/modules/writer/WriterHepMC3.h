/*
 * WriterHepMC3.h
 *
 *  Created on: Feb 11, 2021
 *      Author: Pawel Sznajder (NCBJ) 
 */

#ifndef MODULES_WRITER_WRITERHEPMC3_H_
#define MODULES_WRITER_WRITERHEPMC3_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <memory>
#include <string>
#include <vector>

#include "../../beans/physics/Event.h"
#include "WriterHepMC3Type.h"
#include "WriterModule.h"

namespace HepMC3 {
class Writer;
class GenRunInfo;
} /* namespace HepMC3 */

namespace EPIC {

/**
 * @class WriterHepMC3
 *
 * @brief Event writer based on HepMC3.
 *
 * Module dedicated to writing events. It is based on HepMC3 library.
 */
class WriterHepMC3: public WriterModule {

public:
	static const std::string PARAMETER_NAME_HEPMC3_WRITER_TYPE; ///< String used to set HepMC3 writer
																/// type through
	/// WriterHepMC3::configure().

	static const unsigned int classId; ///< Unique ID to automatically register
									   /// the class in the registry.

	/**
	 * Default constructor.
	 */
	WriterHepMC3(const std::string &className);

	/**
	 * Copy constructor.
	 */
	WriterHepMC3(const WriterHepMC3 &other);

	/**
	 * Destructor.
	 */
	virtual ~WriterHepMC3();

	virtual WriterHepMC3 *clone() const;
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

	/**
	 * Get HepMC3 writer type.
	 */
	WriterHepMC3Type::Type getWriterHepMC3Type() const;

	/**
	 * Set HepMC3 writer type.
	 */
	void setWriterHepMC3Type(WriterHepMC3Type::Type writerHepMc3Type);

private:

	/**
	 * Get particle code.
	 */
	int getParticleCode(ParticleCodeType::Type type) const;

	WriterHepMC3Type::Type m_writerHepMC3Type;      ///< HepMC3 writer type.
	std::shared_ptr<HepMC3::Writer> m_writerHepMC3; ///< Writer.
	std::shared_ptr<HepMC3::GenRunInfo> m_runInfoHepMC3; ///< Run info.
};

} /* namespace EPIC */

#endif /* MODULES_WRITER_WRITERHEPMC3_H_ */

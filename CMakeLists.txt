# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.2)

# define project name and its language
project(EPIC CXX)

# define c++ standard and issue all the warning demanded by this standard
add_definitions(-pedantic -Wno-vla-extension)
set(CMAKE_CXX_STANDARD 14)

if (NOT DEFINED CMAKE_MACOSX_RPATH)
   set(CMAKE_MACOSX_RPATH 0)
endif()

# VERSION ==================================================================================

# read file
file(READ ${CMAKE_SOURCE_DIR}/data/version.txt VERSION_STR)

# strip spaces
string(STRIP ${VERSION_STR} VERSION_STR)

# get major, minor and patch versions
string(REGEX MATCH "^[0-9]*\\." TMP_STR ${VERSION_STR})
string(REGEX REPLACE "\\." "" EPIC_VERSION_MAJOR ${TMP_STR})

string(REGEX MATCH "\\.[0-9]*\\." TMP_STR ${VERSION_STR})
string(REGEX REPLACE "\\." "" EPIC_VERSION_MINOR ${TMP_STR})

string(REGEX MATCH "\\.[0-9]*$" TMP_STR ${VERSION_STR})
string(REGEX REPLACE "\\." "" EPIC_VERSION_PATCH ${TMP_STR})

message("-- EPIC version is: " ${EPIC_VERSION_MAJOR} "." ${EPIC_VERSION_MINOR} "." ${EPIC_VERSION_PATCH})

# configure
configure_file(${CMAKE_SOURCE_DIR}/cmake/Version/EpicVersion.h_in ${CMAKE_SOURCE_DIR}/include/EpicVersion.h)

# AUTOMOC ==================================================================================

# tell cmake to run Qt moc when necessary
set(CMAKE_AUTOMOC ON)
  
# as moc files are generated in the binary dir, tell cmake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# FIND LIBRARIES ===========================================================================

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: Qt
find_package(Qt5 COMPONENTS Core Sql XmlPatterns)

if(${Qt5_FOUND})

	find_package(Qt5Widgets)
	message("-- Found Qt5: " ${Qt5Widgets_VERSION})

	set(QT_LIBRARY_CORE "Qt5::Core")
	set(QT_LIBRARY_SQL "Qt5::Sql")
	set(QT_LIBRARY_XML "Qt5::XmlPatterns")
	
else()    

	find_package(Qt4 COMPONENTS QtCore QtSql QtXmlPatterns)
	
	if(${Qt4_FOUND})
					
		set(QT_LIBRARY_CORE "Qt4::QtCore")
		set(QT_LIBRARY_SQL "Qt4::QtSql")
		set(QT_LIBRARY_XML "Qt4::QtXmlPatterns")
	
	else()    
		message(FATAL_ERROR "Neither Qt5 nor Qt4 found in this system")
	endif()
endif()

# find libraries: gsl
find_package(GSL)

# find libraries: SFML
find_package(SFML COMPONENTS system REQUIRED)

# find libraries: ElementaryUtils
find_package(ElementaryUtils REQUIRED)

# find libraries: NumA++
find_package(NumA++ REQUIRED)

# find libraries: PARTONS
find_package(PARTONS REQUIRED)

# find libraries: Apfel++
find_package(Apfel++ REQUIRED)

# find libraries: Root
find_package(ROOT COMPONENTS Core Hist Minuit Foam REQUIRED)

# find libraries: HepMC3
find_package(HepMC3 REQUIRED)

# directories containing headers 
include_directories(${GSL_INCLUDE_DIRS} ${QT_INCLUDE_DIRS} ${SFML_INCLUDE_DIR} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${PARTONS_INCLUDE_DIR} ${Apfel++_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})

# FINALIZE ==================================================================================

# generate list of source files
file(

        GLOB_RECURSE

        source_files

        src/*
)

# define target executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(

       	epic	

        ${source_files}
)

# define libraries to be linked
target_link_libraries(

       	epic	

        ${GSL_LIBRARIES}

        ${QT_LIBRARIES}

	${SFML_LIBRARIES}

        ${ElementaryUtils_LIBRARIES}

        ${NumA++_LIBRARIES}

        ${PARTONS_LIBRARIES}

        ${Apfel++_LIBRARIES}
        
        ${ROOT_LIBRARIES}
        
        ${HEPMC3_LIBRARIES}

	${QT_LIBRARY_CORE}
	${QT_LIBRARY_SQL}
	${QT_LIBRARY_XML}
)

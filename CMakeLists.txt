# Check CMake version
cmake_minimum_required(VERSION 3.10)

# Define the project
project(obs2ioda)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# Set the Fortran compiler and flags
enable_language(Fortran)
set(NCEPLIBS-BUFR_LIBRARIES CACHE STRING "" )
if(CMAKE_Fortran_COMPILER_ID MATCHES GNU)
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS}   -ffree-line-length-none -mcmodel=medium")
endif()
if(CMAKE_Fortran_COMPILER_ID MATCHES Intel)
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -mcmodel medium")
endif()

# Find required packages
find_package(NetCDF REQUIRED COMPONENTS Fortran C)

# Set include directories
include_directories(${NetCDF_INCLUDE_DIRS} )

# Define sources
set(OBS2IODA_SRC
    define_mod.f90
    gnssro_mod.f90
    hsd.f90
    satwnd_mod.f90
    kinds.f90
    main.f90
    ncio_mod.f90
    netcdf_mod.f90
    prepbufr_mod.f90
    radiance_mod.f90
    ufo_variables_mod.F90
    utils_mod.f90
)
list(TRANSFORM OBS2IODA_SRC PREPEND "${PROJECT_SOURCE_DIR}/src/")

# Define executable
add_executable(obs2ioda-v2.x ${OBS2IODA_SRC})

# Link libraries
target_link_libraries(obs2ioda-v2.x ${NetCDF_LIBRARIES} ${NCEPLIBS-BUFR_LIBRARIES} )
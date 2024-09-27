# Check CMake version
cmake_minimum_required(VERSION 3.10)

# Define the project
project(obs2ioda)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -cpp")  # Use -fpp if using Intel Fortran
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -cpp")

# Set the Fortran compiler and flags
enable_language(Fortran)
set(OBS2IODA_VERSION CACHE STRING "2" )
set(NCEPLIBS-BUFR_LIBRARIES CACHE STRING "" )
set(IODA_UPGRADER_BIN CACHE STRING "" )
set(OBS2IODA_V2_TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/obs2ioda-v2")
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
list(TRANSFORM OBS2IODA_SRC PREPEND "${PROJECT_SOURCE_DIR}/obs2ioda-v2/src/")

# Define executable
add_executable(obs2ioda-v2.x ${OBS2IODA_SRC})
target_compile_definitions(
    obs2ioda-v2.x
    PUBLIC
    IODA_UPGRADER_BIN=\"${IODA_UPGRADER_BIN}\"
)
# Link libraries
target_link_libraries(obs2ioda-v2.x ${NetCDF_LIBRARIES} ${NCEPLIBS-BUFR_LIBRARIES} )

add_executable(
        obs2ioda-v2_test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/obs2ioda-v2/run.cpp
)
target_compile_definitions(
        obs2ioda-v2_test
        PUBLIC
        OBS2IODA_V2_TEST_DIRECTORY=\"${OBS2IODA_V2_TEST_DIRECTORY}\"
)

add_custom_command(TARGET obs2ioda-v2.x POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                   $<TARGET_FILE:obs2ioda-v2.x>
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/obs2ioda-v2/obs2ioda-v2.x
)

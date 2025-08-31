set(eckit_test_ioda_obs_schema_SOURCES ioda_obs_schema.eckit.test.cc)
set(eckit_test_ioda_obs_schema_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_ioda_obs_schema_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_ioda_obs_schema "${eckit_test_ioda_obs_schema_SOURCES}")
target_link_libraries(eckit_test_ioda_obs_schema PRIVATE "${eckit_test_ioda_obs_schema_LIBRARIES}")
add_test(
        NAME eckit_test_ioda_obs_schema
        COMMAND eckit_test_ioda_obs_schema
)

set(eckit_test_netcdf_file_SOURCES netcdf_file.eckit.test.cc)
set(eckit_test_netcdf_file_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_netcdf_file_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_netcdf_file "${eckit_test_netcdf_file_SOURCES}")
target_link_libraries(eckit_test_netcdf_file PRIVATE "${eckit_test_netcdf_file_LIBRARIES}")
add_test(
        NAME eckit_test_netcdf_file
        COMMAND eckit_test_netcdf_file
)

set(eckit_test_netcdf_group_SOURCES netcdf_group.eckit.test.cc)
set(eckit_test_netcdf_group_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_netcdf_group_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_netcdf_group "${eckit_test_netcdf_group_SOURCES}")
target_link_libraries(eckit_test_netcdf_group PRIVATE "${eckit_test_netcdf_group_LIBRARIES}")
add_test(
        NAME eckit_test_netcdf_group
        COMMAND eckit_test_netcdf_group
)

set(eckit_test_netcdf_variable_SOURCES netcdf_variable.eckit.test.cc)
set(eckit_test_netcdf_variable_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_netcdf_variable_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_netcdf_variable "${eckit_test_netcdf_variable_SOURCES}")
target_link_libraries(eckit_test_netcdf_variable PRIVATE "${eckit_test_netcdf_variable_LIBRARIES}")
add_test(
        NAME eckit_test_netcdf_variable
        COMMAND eckit_test_netcdf_variable
)

set(eckit_test_netcdf_error_SOURCES netcdf_error.eckit.test.cc)
set(eckit_test_netcdf_error_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_netcdf_error_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_netcdf_error "${eckit_test_netcdf_error_SOURCES}")
target_link_libraries(eckit_test_netcdf_error PRIVATE "${eckit_test_netcdf_error_LIBRARIES}")
add_test(
        NAME eckit_test_netcdf_error
        COMMAND eckit_test_netcdf_error
)

set(eckit_test_netcdf_dimension_SOURCES netcdf_dimension.eckit.test.cc)
set(eckit_test_netcdf_dimension_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_netcdf_dimension_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_netcdf_dimension "${eckit_test_netcdf_dimension_SOURCES}")
target_link_libraries(eckit_test_netcdf_dimension PRIVATE "${eckit_test_netcdf_dimension_LIBRARIES}")
add_test(
        NAME eckit_test_netcdf_dimension
        COMMAND eckit_test_netcdf_dimension
)

set(eckit_test_netcdf_attribute_SOURCES netcdf_attribute.eckit.test.cc)
set(eckit_test_netcdf_attribute_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_netcdf_attribute_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_netcdf_attribute "${eckit_test_netcdf_attribute_SOURCES}")
target_link_libraries(eckit_test_netcdf_attribute PRIVATE "${eckit_test_netcdf_attribute_LIBRARIES}")
add_test(
        NAME eckit_test_netcdf_attribute
        COMMAND eckit_test_netcdf_attribute
)

set(eckit_test_set_netcdf_group_SOURCES set_netcdf_group.eckit.test.cc)
set(eckit_test_set_netcdf_group_LIBRARIES eckit obs2ioda_cxx)
set(eckit_test_set_netcdf_group_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/cxx)
add_executable(eckit_test_set_netcdf_group "${eckit_test_set_netcdf_group_SOURCES}")
target_link_libraries(eckit_test_set_netcdf_group PRIVATE "${eckit_test_set_netcdf_group_LIBRARIES}")
add_test(
        NAME eckit_test_set_netcdf_group
        COMMAND eckit_test_set_netcdf_group
)



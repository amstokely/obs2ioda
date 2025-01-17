#include "netcdf_test_fixture.h"
#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_variable.h"
#include "netcdf_dimension.h"
#include <gtest/gtest.h>
#include <filesystem>
/**
 * @brief Test case for creating, managing, and closing NetCDF files using Obs2Ioda functionality.
 *
 * This test case verifies the behavior of the NetCDF file management system, including file creation,
 * retrieval, addition to the file map, and error handling for various edge cases. It ensures that
 * proper exceptions are thrown for invalid operations and that the system handles files correctly.
 *
 * @test Verifies the following scenarios:
 *
 * - **NetCDF File Creation**:
 *   - Calls `Obs2Ioda::netcdfCreate` to create a NetCDF file and ensures that the operation succeeds.
 *   - Confirms that the created file exists on the filesystem.
 *
 * - **Duplicate File Creation**:
 *   - Attempts to create a file with an already existing NetCDF ID and verifies that
 *     `Obs2Ioda::netcdfCreate` returns the error code `13`, indicating a `NcCantCreate` exception.
 *
 * - **Adding a File with an Existing ID to the File Map**:
 *   - Calls `Obs2Ioda::FileMap::addFile` with an existing ID and verifies that it throws
 *     a `netCDF::exceptions::NcCantCreate` exception.
 *
 * - **Retrieving a Non-Existent File**:
 *   - Calls `Obs2Ioda::FileMap::getFile` with a non-existent ID and verifies that it throws
 *     a `netCDF::exceptions::NcBadId` exception.
 *
 * - **NetCDF File Closure**:
 *   - Calls `Obs2Ioda::netcdfClose` to close an existing NetCDF file and ensures that the operation succeeds.
 *   - Attempts to close a file with a non-existent ID and verifies that `Obs2Ioda::netcdfClose` returns
 *     the error code `-33`, corresponding to a `NcBadId` exception.
 *
 * - **Removing a Non-Existent File**:
 *   - Calls `Obs2Ioda::FileMap::removeFile` with a non-existent ID and verifies that it throws
 *     a `netCDF::exceptions::NcBadId` exception.
 *
 * @see Obs2Ioda::netcdfCreate
 * @see Obs2Ioda::netcdfClose
 * @see Obs2Ioda::FileMap
 */
TEST_F(NetCDFTestFixture, NetCDFCreateTest) {
    int netcdfID{};
    std::shared_mutex mutex;
    std::lock_guard lock(mutex);
    // Test that netcdfCreate successfully creates a NetCDF file
    int status = Obs2Ioda::netcdfCreate(
        this->test_file_path.c_str(),
        &netcdfID
    );
    EXPECT_EQ(status, 0);
    // Test that the NetCDF file exists on the filesystem
    EXPECT_TRUE(std::filesystem::exists(this->test_file_path));
    // Test that the NcCantCreate exception is thrown when adding a file with an existing ID
    status = Obs2Ioda::netcdfCreate(
        this->test_file_path.c_str(),
        &netcdfID
    );
    EXPECT_EQ(status, 13);
    // Test that the NcCantCreate exception is thrown when adding a file to FileMap with an existing ID
    auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
    EXPECT_THROW(
        Obs2Ioda::FileMap::getInstance().addFile(netcdfID, file),
        netCDF::exceptions::NcCantCreate
    );
    // Test that NcBadId exception is thrown when retrieving a file from FileMap with a non-existent ID
    EXPECT_THROW(Obs2Ioda::FileMap::getInstance().getFile(1), netCDF::exceptions::NcBadId);
    // Test that netcdfClose successfully closes a NetCDF file
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
    // Test that the -33 error code, which corresponds to NcBadId, is returned when closing a non-existent file
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, -33);
    // Test that the NcBadId exception is thrown when removing a file from FileMap with a non-existent ID
    EXPECT_THROW(Obs2Ioda::FileMap::getInstance().removeFile(1), netCDF::exceptions::NcBadId);
}

/**
 * @brief Test case for adding groups to NetCDF files using Obs2Ioda functionality.
 */
TEST_F(NetCDFTestFixture, NetCDFAddGroupTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_group_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    // Add a top level group
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            this->test_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    // Add a nested group
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_nested_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

TEST_F(NetCDFTestFixture, NetCDFAddVarTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_var_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            this->test_group_name.c_str()
    );

    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            this->test_global_dim_name.c_str(),
            this->test_global_dim_len
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddDim(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_dim_name.c_str(),
            this->test_dim_len
    );

    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int_var_name.c_str(),
            NC_INT,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int64_var_name.c_str(),
            NC_INT64,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_real_var_name.c_str(),
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_string_var_name.c_str(),
            NC_STRING,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);

    status = Obs2Ioda::netcdfPutVarInt(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int_var_name.c_str(),
            this->test_int_var_data.data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfPutVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int64_var_name.c_str(),
            this->test_int64_var_data.data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfPutVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_real_var_name.c_str(),
            this->test_real_var_data.data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfPutVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_string_var_name.c_str(),
            this->test_string_var_data.data()
    );
    EXPECT_EQ(status, 0);

    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
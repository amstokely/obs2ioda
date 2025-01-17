#include "netcdf_test_fixture.h"
#include "netcdf_file.h"
#include <gtest/gtest.h>
#include <filesystem>

/**
 * @brief Test case for creating and closing a NetCDF file.
 *
 * This test verifies that a NetCDF file can be successfully created using
 * the Obs2Ioda::netcdfCreate function, and ensures the file exists on the filesystem
 * after creation. The test also checks that the NetCDF file can be closed
 * using the Obs2Ioda::netcdfClose function without errors.
 *
 * Steps:
 * - Call Obs2Ioda::netcdfCreate to create a NetCDF file at the test file path.
 * - Check that the function returns a success status (0).
 * - Verify that the created file exists on the filesystem.
 * - Close the created NetCDF file using Obs2Ioda::netcdfClose.
 * - Check that the function returns a success status (0) upon closing.
 *
 * @test Verifies the following:
 * - A NetCDF file can be created without errors.
 * - The file exists on the filesystem after creation.
 * - The file can be closed without errors.
 */
TEST_F(NetCDFTestFixture, NetCDFCreateTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_file_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    EXPECT_TRUE(std::filesystem::exists(this->test_file_path));

    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

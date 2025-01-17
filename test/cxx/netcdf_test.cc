#include "netcdf_test_fixture.h"
#include "netcdf_file.h"
#include "netcdf_group.h"
#include <gtest/gtest.h>

// Example test case using the fixture
TEST_F(NetCDFTestFixture, NetCDFCreateTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_file_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

TEST_F(NetCDFTestFixture, NetCDFAddGroupTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_group_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            this->test_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_nested_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
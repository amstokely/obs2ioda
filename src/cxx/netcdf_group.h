#ifndef NETCDF_GROUP_H
#define NETCDF_GROUP_H
#include "netcdf_file.h"
#include "netcdf_error.h"
#include "netcdf_utils.h"


namespace Obs2Ioda {
    template<typename FileType>
    void add_group(const std::shared_ptr<FileType> &file,
                        const std::string &parentGroupName, const std::string &groupName) {
        const auto iodaParentGroupName = iodaSchema.getGroup(parentGroupName).getValidName();
        auto iodaGroupName = iodaSchema.getGroup(groupName).getValidName();
        file->add_group(iodaParentGroupName, iodaGroupName);
    }


    extern "C" {
    /**
     * @brief Adds a new group to a NetCDF file, under a specified parent group.
     *
     * This function provides an interface for adding a new group to a NetCDF file
     * using its unique identifier (`netcdfID`)
     *
     * @param netcdfID
     *     The unique identifier for the NetCDF file, used to retrieve the associated file
     *     object from the internal file map.
     *  @param parentGroupName
     *      The name of the parent group under which the new group will be added.
     *      - If `parentGroupName` is `nullptr`, the root group of the file (represented by
     *        the `netCDF::NcFile` object) is used as the parent group.
     *      - If `parentGroupName` specifies the name of an existing group, that group will
     *        be used as the parent.
     *      - If the specified parent group does not exist, a `netCDF::exceptions::NcNullGrp`
     *        exception is raised, and the function returns `-1`.
     * @param groupName
     *     The name of the new group to be created within the specified parent group.
     *
     * @return
     *     - 0 on success.
     *     - A non-zero error code if an exception is encountered (e.g., invalid group name,
     *       parent group not found, or other NetCDF-related errors).
     */
    int c_netcdfAddGroup(int netcdfID, const char *parentGroupName,
                         const char *groupName) {
        try {
            if (groupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null group name to c_netcdfAddGroup", __FILE__,
                    __LINE__);
            }
            if (parentGroupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null parent group name to c_netcdfAddGroup", __FILE__,
                    __LINE__);
            }
            const auto file = FileMap::getInstance().getFile(netcdfID);
            add_group(file, parentGroupName, groupName);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }
    }
}

#endif //NETCDF_GROUP_H

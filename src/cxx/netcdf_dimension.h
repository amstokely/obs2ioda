#ifndef NETCDF_DIMENSION_H
#define NETCDF_DIMENSION_H
#include <netcdf>
#include "netcdf_file.h"
#include "netcdf_error.h"
#include "netcdf_utils.h"


namespace Obs2Ioda {
    template<typename FileType>
    inline void add_dim(const std::shared_ptr<FileType> &file,
                        const std::string &group_name,
                        const std::string &dim_name, int len, int &dim_id) {
        const auto ioda_dim_name = iodaSchema.getDimension(dim_name).
                getValidName();
        const auto ioda_group_name = iodaSchema.getGroup(group_name).
                getValidName();
        file->add_dim(ioda_group_name, ioda_dim_name, len, &dim_id);
    }

    extern "C" {
    /**
* @brief Adds a new dimension to a NetCDF file.
*
* This function adds a dimension to a NetCDF file, supporting both global dimensions
* and dimensions within a specific group.
*
* @param netcdfID
*     The unique identifier for the NetCDF file. This ID is used to retrieve the
*     corresponding file object from the internal file map.
* @param groupName
*     A null-terminated string specifying the name of the group in which the dimension
*     will be created. If `NULL`, the dimension will be added to the root group.
* @param dimName
*     A null-terminated string specifying the name of the new dimension. The name must
*     be unique within the target group.
* @param len
*     The length of the dimension.
*
* @param dimID
*     A pointer to an integer that will be set to the ID of the new dimension.
*
* @return
*     - 0 on success.
*     - A non-zero error code if an exception is encountered.
*/
    int c_netcdfAddDim(int netcdfID, const char *groupName, const char *dimName,
                       int len, int *dimID) {
        try {
            if (groupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null group name to c_netcdfAddDim", __FILE__,
                    __LINE__);
            }
            const auto file = Obs2Ioda::FileMap::getInstance().
                    getFile(netcdfID);
            add_dim(file, groupName, dimName, len, *dimID);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }
    }
}

#endif //NETCDF_DIMENSION_H

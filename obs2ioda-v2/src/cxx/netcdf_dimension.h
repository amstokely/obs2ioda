#ifndef NETCDF_DIMENSION_H
#define NETCDF_DIMENSION_H


namespace Obs2Ioda {
    extern "C" {
    /**
* @brief Adds a new dimension to a NetCDF file, either in the root group or a specified group.
*
* This function adds a dimension to a NetCDF file, supporting both global dimensions
* (in the root group) and group-specific dimensions. The implementation uses the
* NetCDF C++ API and ensures thread safety through a mutex lock.
*
* @param netcdfID
*     The unique identifier for the NetCDF file. This ID is used to retrieve the
*     corresponding file object from the internal file map.
* @param groupName
*     A null-terminated string specifying the name of the group in which the dimension
*     will be created. If `nullptr`, the dimension will be added to the root group.
* @param dimName
*     A null-terminated string specifying the name of the new dimension. The name must
*     be unique within the target group.
* @param len
*     The length of the dimension. Use `NC_UNLIMITED` to create an unlimited dimension.
*
* @return
*     - 0: Success.
*     - Non-zero error code: Failure. The error code and additional context are generated
*       by `netcdfErrorMessage`, which includes details like the line number and file name.
*
* @note
* - The `netcdfID` must be valid and correspond to an open NetCDF file.
* - Dimension names and lengths must adhere to NetCDF conventions.
* - Thread safety is ensured using a shared mutex (`sharedMutex`).
*/
    int netcdfAddDim(
        int netcdfID,
        const char *groupName,
        const char *dimName,
        int len
    );
    }
}

#endif //NETCDF_DIMENSION_H

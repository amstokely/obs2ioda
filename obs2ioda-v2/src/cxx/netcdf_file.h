#ifndef OBS2IODA_NETCDF_FILE_H
#define OBS2IODA_NETCDF_FILE_H

#include <netcdf>
#include <unordered_map>
#include <memory>
#include <shared_mutex>

namespace Obs2Ioda {

    /// A map that associates NetCDF file IDs with shared pointers to NcFile objects.
    /// This map is used for managing open NetCDF files.
    extern std::unordered_map<
               int, ///< The NetCDF file ID.
               std::shared_ptr<netCDF::NcFile>> NETCDF_FILE_MAP;

    /// A shared mutex to synchronize access to the NETCDF_FILE_MAP.
    extern std::shared_mutex map_mutex;

    extern "C" {

    /**
     * @brief Creates and opens a NetCDF file.
     *
     * This function creates and opens a new NetCDF file at the specified path.
     * It stores the NetCDF file object in a map for future reference.
     *
     * @param path The path to the NetCDF file to be created.
     * @param netcdfID Output parameter that will receive the ID of the created NetCDF file.
     *
     * @return 0 on success, or a non-zero error code on failure.
     */
    int netcdfCreate(
            const char *path, ///< The path to the NetCDF file to be created.
            int *netcdfID     ///< The ID of the created NetCDF file.
    );

    /**
     * @brief Closes the NetCDF file associated with the given ID.
     *
     * This function closes the NetCDF file and removes it from the internal map.
     *
     * @param netcdfID The ID of the NetCDF file to close.
     *
     * @return 0 on success, or a non-zero error code on failure.
     */
    int netcdfClose(int netcdfID); ///< The ID of the NetCDF file to be closed.

    }

} // namespace Obs2Ioda

#endif // OBS2IODA_NETCDF_FILE_H

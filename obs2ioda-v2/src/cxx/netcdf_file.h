#ifndef OBS2IODA_NETCDF_FILE_H
#define OBS2IODA_NETCDF_FILE_H

#include <netcdf>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace Obs2Ioda {
/**
 * @class NetcdfFileMap
 * @brief Singleton class for managing a thread-safe mapping of NetCDF file IDs to file objects.
 *
 * This class ensures thread-safe operations on a map that associates unique NetCDF IDs with
 * `std::shared_ptr` instances of `netCDF::NcFile`. It provides methods to add, remove, and
 * retrieve files, while enforcing singleton usage.
 */
class NetcdfFileMap {
public:
    /**
     * @brief Retrieves the singleton instance of the NetcdfFileMap.
     *
     * Ensures there is only one instance of the NetcdfFileMap throughout the application.
     *
     * @return A reference to the singleton instance of NetcdfFileMap.
     */
    static NetcdfFileMap& getInstance();

    /**
     * @brief Deleted copy constructor to enforce singleton behavior.
     */
    NetcdfFileMap(const NetcdfFileMap&) = delete;

    /**
     * @brief Deleted assignment operator to enforce singleton behavior.
     */
    NetcdfFileMap& operator=(const NetcdfFileMap&) = delete;

    /**
     * @brief Adds a NetCDF file to the map.
     *
     * Associates a unique NetCDF file ID with a `std::shared_ptr` to a `netCDF::NcFile` object.
     * Throws an exception if the ID already exists in the map.
     *
     * @param netcdfID The unique NetCDF file ID.
     * @param file A shared pointer to the NetCDF file to be added.
     * @return 0 on success, or an error code if an exception is caught.
     * @throws netCDF::exceptions::NcBadId if the `netcdfID` already exists in the map.
     */
    int addFile(int netcdfID, const std::shared_ptr<netCDF::NcFile>& file);

    /**
     * @brief Removes a NetCDF file from the map.
     *
     * Removes the association of the given NetCDF file ID from the map.
     * Throws an exception if the ID does not exist in the map.
     *
     * @param netcdfID The unique NetCDF file ID to be removed.
     * @return 0 on success, or an error code if an exception is caught.
     * @throws netCDF::exceptions::NcBadId if the `netcdfID` does not exist in the map.
     */
    int removeFile(int netcdfID);

    /**
     * @brief Retrieves a NetCDF file from the map.
     *
     * Retrieves the `std::shared_ptr` to the `netCDF::NcFile` object associated with the given
     * NetCDF file ID.
     *
     * @param netcdfID The unique NetCDF file ID to retrieve.
     * @return A shared pointer to the NetCDF file.
     * @throws std::runtime_error if the `netcdfID` does not exist in the map.
     */
    std::shared_ptr<netCDF::NcFile> getFile(int netcdfID);

private:
    /**
     * @brief Private constructor to prevent direct instantiation.
     */
    NetcdfFileMap() = default;

    /// Map associating NetCDF file IDs with their corresponding shared pointers to NetCDF files.
    std::unordered_map<int, std::shared_ptr<netCDF::NcFile>> netcdfFileMap;

    /// Mutex to ensure thread-safe access to the map.
    std::mutex netcdfFileMapMutex;
};


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
        int *netcdfID ///< The ID of the created NetCDF file.
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

#ifndef OBS2IODA_NETCDF_FILE_H
#define OBS2IODA_NETCDF_FILE_H

#include <netcdf>
#include <unordered_map>
#include <memory>
#include <shared_mutex>

namespace Obs2Ioda
{
    /**
    * @class NetCDFFileManager
    * @brief Singleton class for managing open NetCDF file objects.
    *
    * This class provides a centralized mechanism to manage NetCDF file handles
    * (`netCDF::NcFile` objects) and their associated IDs. It uses a thread-safe
    * approach to store and access NetCDF file objects through a shared map,
    * ensuring proper synchronization across multiple threads.
    *
    * @note This class is designed for internal use within the NetCDF management system
    * and is not intended to be instantiated directly.
    */
    class NetCDFFileManager
    {
    public:
        /**
        * @brief Retrieves the singleton instance of the `NetCDFFileManager`.
        *
        * @return Reference to the singleton instance of `NetCDFFileManager`.
        */
        static NetCDFFileManager& instance();

        /**
        * @brief Provides direct access to the internal map of NetCDF files.
        *
        * @return Reference to the map associating file IDs with `std::shared_ptr<netCDF::NcFile>` objects.
        */
        static std::unordered_map<int, std::shared_ptr<netCDF::NcFile>>& getMap();

        /**
        * @brief Provides access to the shared mutex for synchronizing file operations.
        *
        * @return Reference to the shared mutex used for thread-safe operations.
        */
        static std::shared_mutex& getMutex();

        // Delete copy/move operations
        NetCDFFileManager(const NetCDFFileManager&) = delete;
        NetCDFFileManager& operator=(const NetCDFFileManager&) = delete;

    private:
        NetCDFFileManager() = default; // Private constructor
        std::unordered_map<int, std::shared_ptr<netCDF::NcFile>> files;
        mutable std::shared_mutex mutex;
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
        const char* path, ///< The path to the NetCDF file to be created.
        int* netcdfID ///< The ID of the created NetCDF file.
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

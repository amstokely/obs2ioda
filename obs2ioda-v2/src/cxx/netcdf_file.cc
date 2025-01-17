#include "netcdf_file.h"
#include "netcdf_error.h"
#include <memory>

namespace Obs2Ioda
{
    NetCDFFileManager& NetCDFFileManager::instance()
    {
        static NetCDFFileManager instance;
        return instance;
    }

    std::unordered_map<int, std::shared_ptr<netCDF::NcFile>>
    & NetCDFFileManager::getMap()
    {
        return instance().files;
    }

    std::shared_mutex& NetCDFFileManager::getMutex()
    {
        return instance().mutex;
    }


    int netcdfCreate(
        const char* path,
        int* netcdfID
    )
    {
        try
        {
            std::lock_guard<std::shared_mutex> lock(NetCDFFileManager::getMutex());
            auto file = std::make_shared<netCDF::NcFile>(
                path,
                netCDF::NcFile::replace
            );
            *netcdfID = file->getId();
            NetCDFFileManager::getMap()[*netcdfID] = file;
            return 0;
        }
        catch (netCDF::exceptions::NcException& e)
        {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfClose(int netcdfID)
    {
        try
        {
            // Lock the mutex and access the map safely
            std::lock_guard<std::shared_mutex> lock(NetCDFFileManager::getMutex());
            auto& map = NetCDFFileManager::getMap();

            // Find the NetCDF ID
            const auto it = map.find(netcdfID);
            if (it == map.end())
            {
                netCDF::exceptions::NcBadId e("Invalid NetCDF ID", __FILE__, __LINE__);
                return netcdfErrorMessage(e, __LINE__, __FILE__);
            }

            // Erase the entry from the map
            map.erase(it);
            return 0;
        }
        catch (const netCDF::exceptions::NcException& e)
        {
            // Delegate exception handling
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }
}

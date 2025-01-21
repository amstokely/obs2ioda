#include "netcdf_file.h"
#include "netcdf_error.h"
#include <memory>

namespace Obs2Ioda {
    NetcdfFileMap &NetcdfFileMap::getInstance() {
        static NetcdfFileMap instance;
        return instance;
    }

    int NetcdfFileMap::addFile(const int netcdfID, const std::shared_ptr<netCDF::NcFile> &file) {
            std::lock_guard<std::mutex> lock(this->netcdfFileMapMutex);

            auto it = this->netcdfFileMap.find(netcdfID);
            if (it != this->netcdfFileMap.end()) {
                throw netCDF::exceptions::NcCantCreate(
                    "NetCDF ID already exists in the NetCDF file map",
                    __FILE__,
                    __LINE__
                );
            }
            this->netcdfFileMap[netcdfID] = file;
            return 0;
    }


    int NetcdfFileMap::removeFile(const int netcdfID) {
            std::lock_guard<std::mutex> lock(this->netcdfFileMapMutex);
            auto it = this->netcdfFileMap.find(netcdfID);
            if (it == this->netcdfFileMap.end()) {
                throw netCDF::exceptions::NcBadId(
                    "NetCDF ID not found in the NetCDF file map",
                    __FILE__,
                    __LINE__
                );
            }
            this->netcdfFileMap.erase(it);
            return 0;
    }

    std::shared_ptr<netCDF::NcFile> NetcdfFileMap::getFile(const int netcdfID) {
            std::lock_guard<std::mutex> lock(this->netcdfFileMapMutex);
            auto netcdfFileIterator = this->netcdfFileMap.find(netcdfID);
            if (netcdfFileIterator == this->netcdfFileMap.end()) {
                throw netCDF::exceptions::NcBadId(
                    "NetCDF ID not found in the NetCDF file map",
                    __FILE__,
                    __LINE__
                );
            }
            return netcdfFileIterator->second;
    }


    int netcdfCreate(
        const char *path,
        int *netcdfID
    ) {
        try {
            const auto file = std::make_shared<netCDF::NcFile>(
                path,
                netCDF::NcFile::replace
            );
            *netcdfID = file->getId();
            NetcdfFileMap::getInstance().addFile(
                *netcdfID,
                file
            );

            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfClose(const int netcdfID) {
        try {
            NetcdfFileMap::getInstance().getFile(netcdfID)->close();
            return NetcdfFileMap::getInstance().removeFile(netcdfID);
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }
}

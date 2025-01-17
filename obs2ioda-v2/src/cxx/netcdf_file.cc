#include "netcdf_file.h"
#include "netcdf_error.h"
#include <memory>

namespace Obs2Ioda {

    NetCDFFileManager &NetCDFFileManager::instance() {
        static NetCDFFileManager instance;
        return instance;
    }

    std::unordered_map<int, std::shared_ptr<netCDF::NcFile> >
    &NetCDFFileManager::getMap() {
        return instance().files;
    }

    std::shared_mutex &NetCDFFileManager::getMutex() {
        return instance().mutex;
    }


    int netcdfCreate(
            const char *path,
            int *netcdfID
    ) {
        try {
            NetCDFFileManager::getMutex().lock();
            auto file = std::make_shared<netCDF::NcFile>(
                    path,
                    netCDF::NcFile::replace
            );
            *netcdfID = file->getId();
            NetCDFFileManager::getMap()[*netcdfID] = file;
            NetCDFFileManager::getMutex().unlock();
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    __LINE__,
                    __FILE__
            );
        }
    }

    int netcdfClose(int netcdfID) {
        try {
            NetCDFFileManager::getMutex().lock();
            auto file = Obs2Ioda::NetCDFFileManager::getMap()[netcdfID];
            file->close();
            NetCDFFileManager::getMap().erase(netcdfID);
            NetCDFFileManager::getMutex().unlock();
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    __LINE__,
                    __FILE__
            );
        }
    }
}

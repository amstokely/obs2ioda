#include "netcdf_file.h"
#include "netcdf_error.h"
#include <memory>
#include "FilePathConfig.h"


namespace Obs2Ioda {
    std::string yamlPath = IODA_SCHEMA_YAML;
    auto iodaSchemaYamlRoot = std::make_unique<YamlEckitNode>(
            yamlPath);
    IodaObsSchema<EckitPolicy> iodaSchema(iodaSchemaYamlRoot);

    FileMap &FileMap::getInstance() {
        static FileMap instance;
        return instance;
    }

    void FileMap::addFile(
            const int netcdfID,
            const std::shared_ptr<NetcdfFile> &file
    ) {
        auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator != this->fileMap.end()) {
            throw netCDF::exceptions::NcCantCreate(
                    "NetCDF ID already exists in the NetCDF file map",
                    __FILE__,
                    __LINE__
            );
        }
        this->fileMap[netcdfID] = file;
    }


    void FileMap::removeFile(
            const int netcdfID
    ) {
        auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator == this->fileMap.end()) {
            throw netCDF::exceptions::NcBadId(
                    "NetCDF ID not found in the NetCDF file map",
                    __FILE__,
                    __LINE__
            );
        }
        this->fileMap.erase(netcdfFileIterator);
    }

    std::shared_ptr<NetcdfFile>
    FileMap::getFile(const int netcdfID) {
        const auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator == this->fileMap.end()) {
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
            int *netcdfID,
            int fileMode
    ) {
        try {
            const auto file = std::make_shared<NetcdfFile>(
                path, fileMode
            );
            *netcdfID = file->getId();
            FileMap::getInstance().addFile(
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
            FileMap::getInstance().getFile(netcdfID)->close();
            FileMap::getInstance().removeFile(netcdfID);
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

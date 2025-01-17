#ifndef NETCDF_UTILS_H
#define NETCDF_UTILS_H

#include <netcdf>
#include <unordered_map>
#include <memory>

namespace Obs2Ioda {

    std::string getIodaName(
            const char *name,
            const std::unordered_map<
                    std::string,
                    std::string
            > &iodaNameMap
    );

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e,
            int errorCode
    );

    std::shared_ptr<netCDF::NcGroup> getRootGroup(
            int netcdfID,
            const char *groupName
    );

    std::string removeWhiteSpace(
            const std::string &name
    );

}

#endif //NETCDF_UTILS_H

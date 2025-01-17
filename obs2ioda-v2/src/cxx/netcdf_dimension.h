#ifndef NETCDF_DIMENSION_H
#define NETCDF_DIMENSION_H


namespace Obs2Ioda {

    extern "C" {

        int netcdfAddDim(
                int netcdfID,
                const char *groupName,
                const char *dimName,
                int len
        );

    }
}

#endif //NETCDF_DIMENSION_H
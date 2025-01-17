#ifndef NETCDF_GROUP_H
#define NETCDF_GROUP_H

namespace Obs2Ioda {

    extern "C" {
        int netcdfAddGroup(
                int netcdfID,
                const char *parentGroupName,
                const char *groupName
        );

    }

}

#endif //NETCDF_GROUP_H

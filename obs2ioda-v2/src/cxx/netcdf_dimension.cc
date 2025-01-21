#include "netcdf_dimension.h"
#include "netcdf_utils.h"
#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda
{
    int netcdfAddDim(
        int netcdfID,
        const char* groupName,
        const char* dimName,
        int len
    )
    {
        try
        {
            std::lock_guard lock(sharedMutex);
            auto file = FileMap::getInstance().getFile(netcdfID);
            auto group = getRootGroup(netcdfID, groupName);
            auto dim = group->addDim(
                dimName,
                len
            );
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
}

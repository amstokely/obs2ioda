#ifndef NETCDF_ATTRIBUTE_H
#define NETCDF_ATTRIBUTE_H
#include <netcdf>
#include <netcdf_error.h>

#include "netcdf_file.h"

namespace Obs2Ioda {
    template<typename FileType, typename T, bool IsString = false>
    void put_att(const std::shared_ptr<FileType> &file, const std::string &attName, T values,
                      const std::string &varName, const std::string &groupName,
                      const netCDF::NcType &netcdfDataType, size_t len) {
        auto iodaVarName = varName.empty() ? "" : iodaSchema.getVariable(varName).getValidName();
        auto iodaGroupName = groupName.empty() ? "" :  iodaSchema.getGroup(groupName).getValidName();


        auto iodaAttName = iodaSchema.getAttribute(attName).getValidName();
        file->template put_att<T, IsString>(iodaAttName, values, iodaVarName, iodaGroupName, netcdfDataType, len);
    }


    extern "C" {
    /**
     * @brief Writes an attribute to a variable, group, or as a global attribute in a NetCDF file.
     *
     * If `varName` is provided, the attribute is assigned to the specified variable.
     * If `varName` is `NULL`, the attribute is assigned to the group.
     * If both `groupName` and `varName` are `NULL`, the attribute is written as a global attribute.
     *
     * @param netcdfID The identifier of the NetCDF file where the attribute will be written.
     * @param attName The name of the attribute to be written.
     * @param attValue A pointer to the integer value to be assigned to the attribute.
     * @param varName The name of the variable to which the attribute will be attached. If `NULL`, the attribute is assigned to the group.
     * @param groupName The name of the group containing the variable. If `NULL`, the root group is assumed.
     * @return int A status code indicating the outcome of the operation:
     *         - 0: Success.
     *         - Non-zero: Failure, with an error message logged.
     */
    int c_netcdfPutAttIntArray(int netcdfID, const char *attName,
                               const int *attValue, const int attLen,
                               const char *varName, const char *groupName) {
        try {
            if (varName == nullptr) {
                std::string msg = "Variable name cannot be null";
                throw netCDF::exceptions::NcBadName(msg.c_str(), __FILE__,
                                                    __LINE__);
            }
            if (groupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null group name to c_netcdfAddDim", __FILE__,
                    __LINE__);
            }
            const auto file = Obs2Ioda::FileMap::getInstance().
                    getFile(netcdfID);
            put_att(file, attName, attValue, varName, groupName,
                         netCDF::NcType(netCDF::ncInt), attLen);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }

    int c_netcdfPutAttRealArray(int netcdfID, const char *attName,
                                const float *attValue, const int attLen,
                                const char *varName, const char *groupName) {
        try {
            if (varName == nullptr) {
                std::string msg = "Variable name cannot be null";
                throw netCDF::exceptions::NcBadName(msg.c_str(), __FILE__,
                                                    __LINE__);
            }
            if (groupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null group name to c_netcdfAddDim", __FILE__,
                    __LINE__);
            }
            const auto file = Obs2Ioda::FileMap::getInstance().
                    getFile(netcdfID);
            put_att(file, attName, attValue, varName, groupName,
                         netCDF::NcType(netCDF::ncFloat), attLen);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }

    /**
     * @brief Writes an attribute to a variable, group, or as a global attribute in a NetCDF file.
     *
     * If `varName` is provided, the attribute is assigned to the specified variable.
     * If `varName` is `NULL`, the attribute is assigned to the group.
     * If both `groupName` and `varName` are `NULL`, the attribute is written as a global attribute.
     *
     * @param netcdfID The identifier of the NetCDF file where the attribute will be written.
     * @param attName The name of the attribute to be written.
     * @param attValue A pointer to the integer value to be assigned to the attribute.
     * @param varName The name of the variable to which the attribute will be attached. If `NULL`, the attribute is assigned to the group.
     * @param groupName The name of the group containing the variable. If `NULL`, the root group is assumed.
     * @return int A status code indicating the outcome of the operation:
     *         - 0: Success.
     *         - Non-zero: Failure, with an error message logged.
     */
    int c_netcdfPutAttInt(int netcdfID, const char *attName,
                          const int *attValue, const char *varName,
                          const char *groupName) {
        try {
            if (varName == nullptr) {
                std::string msg = "Variable name cannot be null";
                throw netCDF::exceptions::NcBadName(msg.c_str(), __FILE__,
                                                    __LINE__);
            }
            if (groupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null group name to c_netcdfAddDim", __FILE__,
                    __LINE__);
            }
            const auto file = Obs2Ioda::FileMap::getInstance().
                    getFile(netcdfID);
            put_att(file, attName, attValue, varName, groupName,
                         netCDF::NcType(netCDF::ncInt), 1);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }

    int c_netcdfPutAttString(const int netcdfID, const char *attName,
                             const char *attValue, const char *varName,
                             const char *groupName) {
        try {
            if (varName == nullptr) {
                std::string msg = "Variable name cannot be null";
                throw netCDF::exceptions::NcBadName(msg.c_str(), __FILE__,
                                                    __LINE__);
            }
            if (groupName == nullptr) {
                throw netCDF::exceptions::NcBadGroupId(
                    "Passed null group name to c_netcdfAddDim", __FILE__,
                    __LINE__);
            }
            const auto file = Obs2Ioda::FileMap::getInstance().
                    getFile(netcdfID);
            put_att<NetcdfFile, const char *, true>(file, attName, attValue,
                                             varName, groupName,
                                             netCDF::NcType(netCDF::ncString),
                                             strlen(attValue));
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }
    }
}

#endif //NETCDF_ATTRIBUTE_H

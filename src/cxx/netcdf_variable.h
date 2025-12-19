#ifndef NETCDF_VARIABLE_H
#define NETCDF_VARIABLE_H

#include <netcdf>
#include "netcdf_file.h"
#include "netcdf_utils.h"
#include "ioda_obs_schema.h"
#include "netcdf_error.h"

namespace Obs2Ioda {
    template<typename FileType>
    void netcdfAddVar(const std::shared_ptr<FileType> &file,
                      const char *groupName, const char *varName,
                      const int dataType, int numDims,
                      const char **dimNames) {
        std::vector<std::string> iodaDimNames;
        iodaDimNames.reserve(numDims);
        for (int i = 0; i < numDims; i++) {
            iodaDimNames.push_back(iodaSchema.getDimension(dimNames[i]).getValidName());
        }
        const auto iodaVarName = iodaSchema.getVariable(varName).getValidName();
        const auto iodaGroupName = iodaSchema.getGroup(groupName).getValidName();
        file->add_var(iodaGroupName, iodaVarName, dataType, iodaDimNames);

    }

    template<typename FileType, typename T, bool netcdfChar = false>
    void netcdfPutVar(const std::shared_ptr<FileType> &file,
                      const char *groupName, const char *varName,
                      const T *values) {
        const auto group = setNetcdfGroup(file, groupName);
        auto iodaVarName = iodaSchema.getVariable(varName).getValidName();
        const auto var = group->getVar(iodaVarName);
        // Validate the data type of the variable
        validateNetcdfDataType<T>(var.getType().getId(),
                                  "Invalid data type for NetCDF variable '" +
                                  std::string(varName) + "': expected " +
                                  std::string(typeid(T).name()) +
                                  ", got NetCDF type ID " + var.getType().
                                  getName());
        var.putVar(values);
    }

    template<typename FileType, typename T>
    void netcdfSetFill(const std::shared_ptr<FileType> &file,
                       const char *groupName, const char *varName, int fillMode,
                       T fillValue) {
        const auto group = setNetcdfGroup(file, groupName);
        auto iodaVarName = iodaSchema.getVariable(varName).getValidName();
        auto var = group->getVar(iodaVarName);
        // Validate the data type of the variable
        validateNetcdfDataType<T>(var.getType().getId(),
                                  "Invalid data type for NetCDF variable '" +
                                  std::string(varName) + "': expected " +
                                  std::string(typeid(T).name()) +
                                  ", got NetCDF type ID " + var.getType().
                                  getName());
        var.setFill(fillMode != 0, // true if fillMode is non-zero
                    fillValue);
    }
}

extern "C" {
/**
 * @brief Adds a variable to a NetCDF file.
 *
 * @param netcdfID The identifier of the NetCDF file where the variable will be added.
 * @param groupName The name of the group in which the variable should be created.
 *                  If NULL, the variable is added as a global variable.
 * @param varName The name of the variable to be created.
 * @param netcdfDataType The NetCDF data type of the variable (e.g., NC_INT, NC_FLOAT).
 * @param numDims The number of dimensions associated with the variable.
 * @param dimNames An array of dimension names specifying the shape of the variable.
 * @return int A status code indicating the outcome of the operation:
 *         - 0: Success.
 *         - Non-zero: Failure, with an error message logged.
 */
int c_netcdfAddVar(int netcdfID, const char *groupName, const char *varName,
                   nc_type netcdfDataType, int numDims, const char **dimNames) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfAddVar(file, groupName, varName, netcdfDataType,
                               numDims, dimNames);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

/**
* @brief Writes data to a variable in a NetCDF file.
*
* @param netcdfID The identifier of the NetCDF file where the data will be written.
* @param groupName The name of the group containing the variable. If NULL, the variable is assumed to be a global variable.
* @param varName The name of the variable to which data will be written.
* @param values A pointer to the data to be written to the variable.
* @return int A status code indicating the outcome of the operation:
*         - 0: Success.
*         - Non-zero: Failure, with an error message logged.
*/
int c_netcdfPutVarInt(int netcdfID, const char *groupName, const char *varName,
                      const int *values) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfPutVar(file, groupName, varName, values);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfPutVarInt64(int netcdfID, const char *groupName,
                        const char *varName, const long long *values) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfPutVar(file, groupName, varName, values);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfPutVarReal(int netcdfID, const char *groupName, const char *varName,
                       const float *values) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfPutVar(file, groupName, varName, values);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfPutVarDouble(int netcdfID, const char *groupName,
                         const char *varName, const double *values) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfPutVar(file, groupName, varName, values);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfPutVarString(int netcdfID, const char *groupName,
                         const char *varName, const char **values) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfPutVar(file, groupName, varName, values);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

/**
* @brief Sets the fill mode and fill value for a variable in a NetCDF file.
*
* @param netcdfID The identifier of the NetCDF file containing the variable.
* @param groupName The name of the group containing the variable. If NULL, the variable is assumed to be a global variable.
* @param varName The name of the variable for which the fill mode is set.
* @param fillMode The fill mode to be applied:
*         - 0: Disable fill mode (use uninitialized values).
*         - 1: Enable fill mode (use the specified fill value).
* @param fillValue The fill value to be applied when fill mode is enabled. Must match the data type of the variable.
* @return int A status code indicating the outcome of the operation:
*         - 0: Success.
*         - Non-zero: Failure, with an error message logged.
*/
int c_netcdfSetFillInt(int netcdfID, const char *groupName, const char *varName,
                       int fillMode, int fillValue) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfSetFill(file, groupName, varName, fillMode, fillValue);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfSetFillReal(int netcdfID, const char *groupName,
                        const char *varName, int fillMode, float fillValue) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfSetFill(file, groupName, varName, fillMode, fillValue);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfSetFillInt64(int netcdfID, const char *groupName,
                         const char *varName, int fillMode,
                         long long fillValue) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfSetFill(file, groupName, varName, fillMode, fillValue);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}

int c_netcdfSetFillString(int netcdfID, const char *groupName,
                          const char *varName, int fillMode,
                          const char *fillValue) {
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
        const auto file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        Obs2Ioda::netcdfSetFill(file, groupName, varName, fillMode, fillValue);
        return 0; // Success
    } catch (netCDF::exceptions::NcException &e) {
        return Obs2Ioda::netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}
}

#endif //NETCDF_VARIABLE_H

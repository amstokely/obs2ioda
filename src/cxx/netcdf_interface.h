#ifndef OBS2IODA_NETCDF_INTERFACE_H
#define OBS2IODA_NETCDF_INTERFACE_H
#include <netcdf>
#include "netcdf_utils.h"
#include <iostream>

class NetcdfFile : public netCDF::NcFile,
                   std::enable_shared_from_this<netCDF::NcGroup> {
public:
    NetcdfFile(const std::string &path, int mode)
        : NcFile(path, static_cast<FileMode>(mode)) {
    }

    void add_group(const std::string &parentGroupName,
                   const std::string &groupName) {
        std::shared_ptr<NcGroup> parentGroup;
        if (!parentGroupName.empty()) {
            parentGroup = std::make_shared<NcGroup>(
                this->getGroup(parentGroupName));
        } else {
            parentGroup = std::make_shared<NcGroup>(*this);
        }
        const auto group = parentGroup->addGroup(groupName);
    }

    void add_dim(const std::string &groupName, const std::string &dimName,
                 int len, int *dimID) {
        std::shared_ptr<NcGroup> group;
        if (!groupName.empty()) {
            group = std::make_shared<NcGroup>(this->getGroup(groupName));
        } else {
            group = std::make_shared<NcGroup>(*this);
        }
        const auto dim = group->addDim(dimName, len);
        *dimID = dim.getId();
    }

    template<typename T, bool IsString = false>
    void put_att(const std::string &attName, T values,
                 const std::string &varName, const std::string &groupName,
                 const netCDF::NcType &netcdfDataType, size_t len) {
        std::shared_ptr<NcGroup> group;
        if (!groupName.empty()) {
            group = std::make_shared<NcGroup>(this->getGroup(groupName));
        } else {
            group = std::make_shared<NcGroup>(*this);
        }
        if (!std::string(varName).empty()) {
            auto var = group->getVar(varName);
            if constexpr (std::is_same_v<const char *, T> && IsString) {
                var.putAtt(attName,
                           std::string(reinterpret_cast<const char *>(values)));
            } else {
                var.putAtt(attName, netcdfDataType, len, values);
            }
        } else {
            if constexpr (std::is_same_v<const char *, T> && IsString) {
                group->putAtt(attName,
                              std::string(
                                  reinterpret_cast<const char *>(values)));
            } else {
                group->putAtt(attName, netcdfDataType, len, values);
            }
        }
    }

    void add_var(const std::string &groupName, const std::string &varName,
                 const int dataType, const std::vector<std::string> &dimNames) {
        const auto num_dims = dimNames.size();
        std::shared_ptr<NcGroup> group;
        if (!groupName.empty()) {
            group = std::make_shared<NcGroup>(this->getGroup(groupName));
        } else {
            group = std::make_shared<NcGroup>(*this);
        }
        std::vector<netCDF::NcDim> dims;
        dims.reserve(num_dims);
        for (int i = 0; i < num_dims; i++) {
            dims.push_back(this->getDim(dimNames.at(i)));
        }
        auto var = group->addVar(varName, netCDF::NcType(dataType), dims);
    }
};

#endif //OBS2IODA_NETCDF_INTERFACE_H

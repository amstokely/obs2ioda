#ifndef IODASCHEMA_H
#define IODASCHEMA_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <utility>

#include "yaml-cpp/yaml.h"
#include "eckit/config/YAMLConfiguration.h"
#include "eckit/filesystem/PathName.h"

#include "FilePathConfig.h"

template<typename T>
concept YamlNode = requires(const T &t, const std::string &key) {
    { t.hasKey(key) } -> std::same_as<bool>;
    { t.isSequence(key) } -> std::same_as<bool>;
    { t.getStringList(key) } -> std::same_as<std::vector<std::string>>;
    { t.getSequence(key) } -> std::same_as<std::vector<T>>;
};

class YamlCppNode {
public:
    explicit YamlCppNode(YAML::Node node) : node_(std::move(node)) {}

    bool hasKey(const std::string &key) const {
        return node_[key].IsDefined();
    }

    bool isSequence(const std::string &key) const {
        return node_[key] && node_[key].IsSequence();
    }

    std::vector<std::string> getStringList(const std::string &key) const {
        return node_[key].as<std::vector<std::string>>();
    }

    std::vector<YamlCppNode> getSequence(const std::string &key) const {
        std::vector<YamlCppNode> result;
        for (const auto &child : node_[key]) {
            result.emplace_back(child);
        }
        return result;
    }

private:
    YAML::Node node_;
};

class YamlEckitNode {
public:
    explicit YamlEckitNode(const std::string &yamlPath)
        : node_(eckit::YAMLConfiguration(eckit::PathName(yamlPath))) {}

    bool hasCategory(const std::string &category) const {
        return node_.has(category);
    }

    bool hasKey(const eckit::LocalConfiguration &config,
                const std::string &key) const {
        return config.has(key);
    }

    bool isCategorySequence(const std::string &category) const {
        return node_.isList(category);
    }

    bool isKeySequence(const eckit::LocalConfiguration &config,
                       const std::string &key) const {
        return config.isList(key);
    }

    std::vector<std::string> getStringList(const eckit::LocalConfiguration &config,
                                           const std::string &key) const {
        return config.getStringVector(key);
    }

    std::vector<eckit::LocalConfiguration> getSequence(const std::string &key) const {
        std::vector<eckit::LocalConfiguration> keyConfigs;
        node_.get(key, keyConfigs);
        return keyConfigs;
    }

private:
    eckit::YAMLConfiguration node_;
};



class IodaObsSchemaComponent {
protected:
    std::string validName;
    std::vector<std::string> names;
    std::string componentType;

    template<YamlNode Node>
    void setNames(const Node &node, const std::string &key) {
        if (node.hasKey(key) && node.isSequence(key)) {
            this->names = node.getStringList(key);
            if (!names.empty()) {
                this->validName = names.at(0);
            }
        }
    }

    explicit IodaObsSchemaComponent(std::string componentType,
                                    std::string name = "")
        : componentType(std::move(componentType)) {
        if (!name.empty()) {
            this->names.push_back(std::move(name));
            this->validName = this->names.at(0);
        }
    }

public:
    const std::string &getValidName() const { return this->validName; }
    const std::vector<std::string> &getNames() const { return this->names; }

    template<YamlNode Node>
    void load(const Node &node) {
        setNames(node, this->componentType);
    }
};
class IodaObsAttribute : public IodaObsSchemaComponent {
public:
    explicit IodaObsAttribute(std::string name = "")
        : IodaObsSchemaComponent("Attribute", std::move(name)) {}
};

class IodaObsGroup : public IodaObsSchemaComponent {
public:
    explicit IodaObsGroup(std::string name = "")
        : IodaObsSchemaComponent("Group", std::move(name)) {}
};

class IodaObsDimension : public IodaObsSchemaComponent {
public:
    explicit IodaObsDimension(std::string name = "")
        : IodaObsSchemaComponent("Dimension", std::move(name)) {}
};

class IodaObsVariable : public IodaObsSchemaComponent {
public:
    explicit IodaObsVariable(std::string name = "")
        : IodaObsSchemaComponent("Variable", std::move(name)) {}

    template<YamlNode Node>
    void load(const Node &node) {
        static constexpr std::array<const char *, 2> keys = {"Variable", "Dimension"};
        for (const auto &key : keys) {
            if (node.hasKey(key) && node.isSequence(key)) {
                this->setNames(node, key);
                break;
            }
        }
    }
};
template<YamlNode Node>
class IodaObsSchema {
    std::unordered_map<std::string, std::shared_ptr<IodaObsVariable>> variables;
    std::unordered_map<std::string, std::shared_ptr<IodaObsDimension>> dimensions;
    std::unordered_map<std::string, std::shared_ptr<IodaObsGroup>> groups;
    std::unordered_map<std::string, std::shared_ptr<IodaObsAttribute>> attributes;

    template<typename T>
    void loadComponent(const Node &schema,
                       const std::string &category,
                       const std::string &key,
                       std::unordered_map<std::string, std::shared_ptr<T>> &componentMap) {
        if (schema.hasKey(category) && schema.isSequence(category)) {
            for (const auto &item : schema.getSequence(category)) {
                if (item.hasKey(key)) {
                    auto component = std::make_shared<T>();
                    component->load(item);
                    for (const auto &n : component->getNames()) {
                        componentMap.emplace(n, component);
                    }
                }
            }
        }
    }

    template<typename T>
    std::shared_ptr<const T> getComponent(const std::string &name,
                                          std::unordered_map<std::string, std::shared_ptr<T>> &componentMap) {
        auto it = componentMap.find(name);
        if (it != componentMap.end()) {
            return it->second;
        }
        auto component = std::make_shared<T>(name);
        componentMap[name] = component;
        return component;
    }

public:
    explicit IodaObsSchema(const Node &schema) {
        loadComponent<IodaObsAttribute>(schema, "Attributes", "Attribute", attributes);
        loadComponent<IodaObsGroup>(schema, "Groups", "Group", groups);
        loadComponent<IodaObsDimension>(schema, "Dimensions", "Dimension", dimensions);
        loadComponent<IodaObsVariable>(schema, "Variables", "Variable", variables);
        loadComponent<IodaObsVariable>(schema, "Dimensions", "Dimension", variables);
    }

    std::shared_ptr<const IodaObsAttribute> getAttribute(const std::string &name) {
        return getComponent(name, attributes);
    }
    std::shared_ptr<const IodaObsGroup> getGroup(const std::string &name) {
        return getComponent(name, groups);
    }
    std::shared_ptr<const IodaObsDimension> getDimension(const std::string &name) {
        return getComponent(name, dimensions);
    }
    std::shared_ptr<const IodaObsVariable> getVariable(const std::string &name) {
        return getComponent(name, variables);
    }
};



#endif  // IODASCHEMA_H

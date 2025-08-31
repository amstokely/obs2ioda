#ifndef IODA_OBS_SCHEMA_ECKIT_H
#define IODA_OBS_SCHEMA_ECKIT_H
#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "ioda_obs_schema_components.h"

#include "eckit/config/LocalConfiguration.h"
#include "eckit/config/YAMLConfiguration.h"
#include "eckit/filesystem/PathName.h"

/**
 * @brief Thin wrapper around eckit::YAMLConfiguration, providing
 *        schema-specific convenience methods.
 */
class YamlEckitNode {
public:
    /// @brief Construct from a YAML file path.
    explicit YamlEckitNode(const std::string &yamlPath)
        : node_(eckit::YAMLConfiguration(eckit::PathName(yamlPath))) {
    }

    /// @brief Check if a top-level category exists.
    [[nodiscard]] bool hasCategory(const std::string &key) const {
        return node_.has(key);
    }

    /// @brief Check if a category is a list/sequence.
    [[nodiscard]] bool
    isCategorySequence(const std::string &key) const {
        return node_.isList(key);
    }

    /// @brief Retrieve sequence of configurations for a category.
    [[nodiscard]] std::vector<eckit::LocalConfiguration>
    getSequence(const std::string &key) const {
        std::vector<eckit::LocalConfiguration> configs;
        node_.get(key, configs);
        return configs;
    }

    /// @brief Check if a key exists within a LocalConfiguration.
    static bool hasKey(
        const eckit::LocalConfiguration &cfg, const std::string &key
    ) {
        return cfg.has(key);
    }

    /// @brief Check if a key corresponds to a list in a LocalConfiguration.
    static bool isKeySequence(
        const eckit::LocalConfiguration &cfg, const std::string &key
    ) {
        return cfg.isList(key);
    }

    /// @brief Retrieve a list of strings from a LocalConfiguration.
    static std::vector<std::string> getStringList(
        const eckit::LocalConfiguration &cfg, const std::string &key
    ) {
        return cfg.getStringVector(key);
    }

private:
    eckit::YAMLConfiguration node_;
};

/**
 * @brief Backend implementation using eckit for loading YAML schema
 *        components into maps.
 */
class EckitBackend {
public:
    /**
     * @brief Load components of a given type from the YAML schema into a map.
     *
     * @tparam Component Schema component type (must derive from IodaObsSchemaComponent).
     * @param schema Pointer to YAML schema node.
     * @param category Category name in the YAML file (e.g. "Variables").
     * @param key Key name within each item in the category sequence (e.g. "Variable").
     * @param map Target map from name → component instance.
     */
    template<std::derived_from<IodaObsSchemaComponent> Component>
    static void loadComponent(
        const std::unique_ptr<YamlEckitNode> &schema,
        const std::string &category,
        const std::string &key,
        ComponentMap<const Component> &map
    ) {
        // Proceed only if the schema contains this category and it is a sequence (list)
        if (schema->hasCategory(category) && schema->isCategorySequence(category)) {

            // Iterate over each entry in the category sequence
            for (const auto &item : schema->getSequence(category)) {

                // Check that the entry has the requested key and that it is itself a sequence
                if (YamlEckitNode::hasKey(item, key) &&
                    YamlEckitNode::isKeySequence(item, key)) {

                    // Extract list of names (canonical + aliases) for this component
                    if (auto names = YamlEckitNode::getStringList(item, key); !names.empty()) {

                        // Create a shared component using the canonical name (front)
                        auto component = std::make_shared<Component>(names.front(), names);

                        // Insert into the map under all names (canonical + aliases),
                        // so they all point to the same component
                        for (const auto &n : names) {
                            map.emplace(n, component);
                        }
                    }
                }
            }
        }
    }
};
using EckitPolicy = SchemaPolicy<YamlEckitNode, EckitBackend>;

#endif //IODA_OBS_SCHEMA_ECKIT_H

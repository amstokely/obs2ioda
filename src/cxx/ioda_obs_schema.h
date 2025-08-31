#ifndef IODASCHEMA_H
#define IODASCHEMA_H

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "ioda_obs_schema_components.h"
#include "ioda_obs_schema_concepts.h"

template<SchemaPolicyLike Policy>
class IodaObsSchema {
    using YamlNode = typename Policy::YamlNode;
    using Backend  = typename Policy::Backend;
    std::unordered_map<std::string, std::shared_ptr<const IodaObsVariable> >
    variables;
    std::unordered_map<std::string, std::shared_ptr<const IodaObsDimension> >
    dimensions;
    std::unordered_map<std::string, std::shared_ptr<const IodaObsGroup> >
    groups;
    std::unordered_map<std::string, std::shared_ptr<const IodaObsAttribute> >
    attributes;

    template<std::derived_from<IodaObsSchemaComponent> Component> void
    setComponent(
        const std::unique_ptr<YamlNode> &schema,
        const std::string &category, const std::string &key,
        ComponentMap<const Component> &map
    ) {
        Backend::template loadComponent<Component>(
            schema, category, key, map
        );
    }

    template<std::derived_from<IodaObsSchemaComponent> Component> const
    Component &getComponent(
        const std::string &n, ComponentMap<const Component> &componentMap
    ) {
        auto [it, inserted] = componentMap.try_emplace(
            n, std::make_shared<Component>(n, std::vector{n})
        );
        return *(it->second);
    }

public:
    /**
     * @brief Construct schema by loading components from a YAML node.
     *
     * @param schema Pointer to YAML schema node.
     */
    explicit IodaObsSchema(const std::unique_ptr<YamlNode> &schema) {
        setComponent<IodaObsAttribute>(
            schema, "Attributes", "Attribute", attributes
        );
        setComponent<IodaObsGroup>(schema, "Groups", "Group", groups);
        setComponent<IodaObsDimension>(
            schema, "Dimensions", "Dimension", dimensions
        );
        setComponent<IodaObsVariable>(
            schema, "Variables", "Variable", variables
        );
        setComponent<IodaObsVariable>(
            schema, "Dimensions", "Dimension", variables
        );
    }

    /// @return Attribute by name (inserts default if not present).
    const IodaObsAttribute &getAttribute(const std::string &n) {
        return getComponent<IodaObsAttribute>(n, attributes);
    }

    /// @return Group by name (inserts default if not present).
    const IodaObsGroup &getGroup(const std::string &n) {
        return getComponent<IodaObsGroup>(n, groups);
    }

    /// @return Dimension by name (inserts default if not present).
    const IodaObsDimension &getDimension(const std::string &n) {
        return getComponent<IodaObsDimension>(n, dimensions);
    }

    /// @return Variable by name (inserts default if not present).
    const IodaObsVariable &getVariable(const std::string &n) {
        return getComponent<IodaObsVariable>(n, variables);
    }
};

#endif  // IODASCHEMA_H

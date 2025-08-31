#ifndef IODA_OBS_SCHEMA_COMPONENTS_H
#define IODA_OBS_SCHEMA_COMPONENTS_H
#include <concepts>
#include <memory>
#include <string>
/**
 * @brief Base class for schema components such as attributes, groups,
 *        dimensions, and variables.
 */
class IodaObsSchemaComponent {
protected:
    std::string validName; ///< Canonical name.
    std::vector<std::string> names{}; ///< Canonical name + aliases.
    std::string componentType; ///< Component type label.

public:
    /**
     * @brief Construct a new schema component.
     *
     * @param componentType The type of component (e.g. "Variable").
     * @param validName The canonical name of the component.
     * @param names Additional aliases (optional).
     */
    explicit IodaObsSchemaComponent(
        std::string componentType, std::string validName,
        std::vector<std::string> names = {}
    )
        : validName(std::move(validName)), names(std::move(names)),
          componentType(std::move(componentType)) {
        if (this->names.empty() && !this->validName.empty()) {
            this->names.push_back(this->validName);
        }
    }

    virtual ~IodaObsSchemaComponent() = default;

    /// @return Canonical name of the component.
    [[nodiscard]] const std::string &getValidName() const {
        return validName;
    }

    /// @return List of canonical + alias names for this component.
    [[nodiscard]] const std::vector<std::string> &getNames() const {
        return names;
    }
};

// --- Leaf schema components ---

/// @brief Schema component representing an attribute.
class IodaObsAttribute final : public IodaObsSchemaComponent {
public:
    explicit IodaObsAttribute(
        std::string validName, std::vector<std::string> names = {}
    )
        : IodaObsSchemaComponent(
            "Attribute", std::move(validName), std::move(names)
        ) {
    }
};

/// @brief Schema component representing a group.
class IodaObsGroup final : public IodaObsSchemaComponent {
public:
    explicit IodaObsGroup(
        std::string validName, std::vector<std::string> names = {}
    )
        : IodaObsSchemaComponent(
            "Group", std::move(validName), std::move(names)
        ) {
    }
};

/// @brief Schema component representing a dimension.
class IodaObsDimension final : public IodaObsSchemaComponent {
public:
    explicit IodaObsDimension(
        std::string validName, std::vector<std::string> names = {}
    )
        : IodaObsSchemaComponent(
            "Dimension", std::move(validName), std::move(names)
        ) {
    }
};

/// @brief Schema component representing a variable.
class IodaObsVariable final : public IodaObsSchemaComponent {
public:
    explicit IodaObsVariable(
        std::string validName, std::vector<std::string> names = {}
    )
        : IodaObsSchemaComponent(
            "Variable", std::move(validName), std::move(names)
        ) {
    }
};

template<std::derived_from<IodaObsSchemaComponent> Component> using
ComponentMap = std::unordered_map<std::string, std::shared_ptr<
    Component> >;

#endif //IODA_OBS_SCHEMA_COMPONENTS_H

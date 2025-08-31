#ifndef IODA_OBS_SCHEMA_CONCEPTS_H
#define IODA_OBS_SCHEMA_CONCEPTS_H
#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>
#include "ioda_obs_schema_components.h"
/**
 * @brief Concept to validate that a Backend provides the required
 *        static template function `loadComponent`.
 *
 * This checks the backend interface by attempting to instantiate
 * `loadComponent` with IodaObsVariable as the component type.
 *
 * @tparam T Backend type
 * @tparam Node YAML node type
 */
template<typename T, typename Node>
concept BackendLike = requires(const std::unique_ptr<Node> &schema,
                               const std::string &category,
                               const std::string &key,
                               std::unordered_map<std::string,
                               std::shared_ptr<const IodaObsVariable>> &map) {
    T::template loadComponent<IodaObsVariable>(schema, category, key, map);
                               };
template<typename YamlNodeT, typename BackendT>
struct SchemaPolicy {
    using YamlNode = YamlNodeT;
    using Backend  = BackendT;
};

template<typename P>
concept SchemaPolicyLike =
    requires {
    typename P::YamlNode;
    typename P::Backend;
    } &&
    BackendLike<typename P::Backend, typename P::YamlNode>;



#endif //IODA_OBS_SCHEMA_CONCEPTS_H

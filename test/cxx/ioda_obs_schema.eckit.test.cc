#include <netcdf_file.h>

#include "ioda_obs_schema.h"
#include "eckit/testing/Test.h"
#include "ioda_obs_schema_eckit.h"
#include "FilePathConfig.h"

using namespace eckit::testing;

namespace {
    auto makeSchema() {
        auto yamlNode = std::make_unique<YamlEckitNode>(
            Obs2Ioda::IODA_SCHEMA_YAML
        );
        return std::make_shared<IodaObsSchema<EckitPolicy>>(yamlNode);
    }
} // namespace

//--------------------------------------------------------------------
// Variable tests
//--------------------------------------------------------------------

CASE("IodaObsSchema - Variable aliases and canonical names") {
    auto iodaSchema = makeSchema();

    const auto station_id_Variable = iodaSchema->getVariable(
        "station_id"
    );
    const auto stationIdentification_Variable = iodaSchema->getVariable(
        "stationIdentification"
    );
    const auto sensorCentralFrequency_Variable = iodaSchema->
            getVariable("sensorCentralFrequency");
    const auto nlocs_Variable = iodaSchema->getVariable("nlocs");
    const auto Location_Variable = iodaSchema->getVariable("Location");
    const auto empty_Variable = iodaSchema->getVariable("");

    EXPECT(
        stationIdentification_Variable.getValidName() ==
        "stationIdentification"
    );
    EXPECT(stationIdentification_Variable.getNames().size() == 4);
    EXPECT(
        sensorCentralFrequency_Variable.getValidName() ==
        "sensorCentralFrequency"
    );
    EXPECT(sensorCentralFrequency_Variable.getNames().size() == 4);

    // Global dimension-as-variable logic
    EXPECT(Location_Variable.getValidName() == "Location");
    EXPECT(Location_Variable.getNames().size() == 2);

    // Empty name returns empty variable
    EXPECT(empty_Variable.getValidName().empty());
}

//--------------------------------------------------------------------
// Dimension tests
//--------------------------------------------------------------------

CASE("IodaObsSchema - Dimension aliases and canonical names") {
    auto iodaSchema = makeSchema();

    const auto nlocs_Dimension = iodaSchema->getDimension("nlocs");
    const auto Location_Dimension = iodaSchema->
            getDimension("Location");
    const auto nstring_Dimension = iodaSchema->getDimension("nstring");

    EXPECT(nlocs_Dimension.getValidName() == "Location");
    EXPECT(nlocs_Dimension.getNames().size() == 2);
    EXPECT(nstring_Dimension.getValidName() == "nstring");
    EXPECT(nstring_Dimension.getNames().size() == 1);
}

//--------------------------------------------------------------------
// Attribute tests
//--------------------------------------------------------------------

CASE("IodaObsSchema - Attribute aliases and canonical names") {
    auto iodaSchema = makeSchema();

    const auto _ioda_layout_Attribute = iodaSchema->getAttribute(
        "_ioda_layout"
    );
    const auto ioda_object_type_Attribute = iodaSchema->getAttribute(
        "ioda_object_type"
    );

    EXPECT(
        ioda_object_type_Attribute.getValidName() == "ioda_object_type"
    );
    EXPECT(ioda_object_type_Attribute.getNames().size() == 2);
}

//--------------------------------------------------------------------
// Group tests
//--------------------------------------------------------------------

CASE("IodaObsSchema - Group resolution and canonical names") {
    auto iodaSchema = makeSchema();

    const auto MetaData_Group = iodaSchema->getGroup("MetaData");
    const auto empty_Group = iodaSchema->getGroup("");
    EXPECT(MetaData_Group.getValidName() == "MetaData");
    EXPECT(MetaData_Group.getNames().size() == 1);

    // Empty name returns empty group
    EXPECT(empty_Group.getValidName().empty());
    EXPECT(empty_Group.getNames().size() == 1);
}

//--------------------------------------------------------------------
// Entry point
//--------------------------------------------------------------------

int main(int argc, char *argv[]) {
    return run_tests(argc, argv);
}

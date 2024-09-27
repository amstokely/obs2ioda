#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <vector>


int main() {
    std::string ioda3_yaml;
    std::string cmd;
    std::vector<std::string> input_files;
    std::string test_dir;
    std::string input_dir;
    std::string output_dir;
#ifdef OBS2IODA_V2_TEST_DIRECTORY
    test_dir = OBS2IODA_V2_TEST_DIRECTORY;
    input_dir =  test_dir + "/input/";
    output_dir = test_dir + "/output/";
    ioda3_yaml = test_dir + "/input/ObsSpace.yaml";
    input_files = {
        "gdas.satwnd.t00z.20180415.bufr",
        "gdas.gpsro.t00z.20180415.bufr",
        "gdas.1bamua.t00z.20180415.bufr"
    };
#endif
    system(("rm -rf " + output_dir).c_str());
    system(("mkdir -p " + output_dir).c_str());
    for (const auto& file : input_files) {
        cmd = "./obs2ioda-v2.x -i " + input_dir + " -o " + output_dir + " -yaml " + ioda3_yaml + " " + file;
        system(cmd.c_str());
        system(("rm -rf " + output_dir + "/*").c_str());
    }
    return 0;
}
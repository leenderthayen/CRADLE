#ifndef OPTIONCONTAINER
#define OPTIONCONTAINER

#include <fstream>
#include <string>
#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

class OptionContainer {
  public:
    static OptionContainer& GetInstance(std::string configName="config.txt") {
      static OptionContainer instance(configName);
      return instance;
    }
    template<typename T> T GetOption(std::string name) {
      return vm[name].as<T>();
    }
    inline static po::options_description GetConfigOptions() { return configOptions; };
    inline static po::options_description GetEnvOptions() { return envOptions; };

  private:
    static po::variables_map vm;
    static po::options_description configOptions;
    static po::options_description envOptions;
    OptionContainer(std::string);
    OptionContainer(OptionContainer const& copy);
    OptionContainer& operator=(OptionContainer const& copy);
};

#endif

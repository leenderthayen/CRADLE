#ifndef OPTIONCONTAINER
#define OPTIONCONTAINER

#include <fstream>
#include <string>
#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

/**
 * Macro to easily get the options from the OptionContainer object
 *
 * @param a variable type
 * @param b variable name
 */
#define GetOpt(a, b) OptionContainer::GetInstance().GetOption<a>(#b)
/**
 * Macro to check whether a certain option was present
 *
 * @param a variable name
 */
#define OptExists(a) OptionContainer::GetInstance().Exists(#a)

class OptionContainer {
  public:
    static OptionContainer& GetInstance(int argc = 0, char** argv = NULL) {
      static OptionContainer instance(argc, argv);
      return instance;
    }
    template<typename T> T GetOption(std::string name) {
      try {
        return vm[name].as<T>();
      } catch (boost::bad_any_cast& e) {
        std::cerr << "ERROR: Option \"" << name << "\" not defined. " << std::endl;
        throw e;
      }
    }
    bool Exists(std::string name) { return (bool)vm.count(name); }
    inline static po::options_description GetConfigOptions() { return configOptions; };
    inline static po::options_description GetEnvOptions() { return envOptions; };
  private:
    static po::variables_map vm;
    static po::options_description cmdOptions;
    static po::options_description configOptions;
    static po::options_description envOptions;
    OptionContainer(int, char**);
    OptionContainer(OptionContainer const& copy);
    OptionContainer& operator=(OptionContainer const& copy);
};

#endif

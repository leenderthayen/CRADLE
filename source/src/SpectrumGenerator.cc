#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "SpectrumGenerator.hh"
#include "Generator.h"
#include "Particle.hh"

namespace pt = boost::property_tree;

std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(Particle* initState, double Q) {
  std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
  std::vector<double> pair = {Q, 1.0};
  deltaDist->push_back(pair);

  return deltaDist;
}

std::vector<std::vector<double> >* BSG::GenerateSpectrum(Particle* initState, Particle* finalState, double Q) {
  char* filename = "test.ini";

  WriteINIFile(filename, initState, finalState, Q);

  int argc = 5;
  char * argv[] = {"bsg_exec", "-i", "filename", "-c", "bsgConfig.txt"};

  OptionContainer::GetInstance(argc, argv);

  Generator* gen = new Generator();

  std::vector<std::vector<double> > spectrum = gen->GenerateSpectrum();

  delete gen;

  return &spectrum;
}

void BSG::WriteINIFile(char* filename, Particle* initState, Particle* finalState, double Q) {
  // Create an empty property tree object.
  pt::ptree tree;

  //TODO Change Fermi/GT
  //Mae the most basic transition config file
  tree.put("Transition.Type", "Gamow-Teller");
  tree.put("Transition.QValue", Q);

  tree.put("Mother.Z", initState->charge);
  tree.put("Mother.A", initState->charge + initState->neutrons);
  tree.put("Daughter.Z", finalState->charge);
  tree.put("Daughter.A", finalState->charge + finalState->neutrons);
  //TODO Put actual spinparities in here
  tree.put("Mother.SpinParity", 2);
  tree.put("Daughter.SpinParity", 0);

  // Write property tree to INI file
  pt::write_ini(filename, tree);
}

SpectrumGenerator::SpectrumGenerator() { }

SpectrumGenerator::~SpectrumGenerator() { }

SpectrumGenerator::DeltaSpectrumGenerator() { }

SpectrumGenerator::BSG() { }

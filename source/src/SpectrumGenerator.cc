#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "SpectrumGenerator.hh"
#include "Particle.hh"
#include "BSGOptionContainer.h"
#include "Generator.h"

namespace pt = boost::property_tree;

std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(Particle* initState, Particle* finalState, double Q) {
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

  bsg::BSGOptionContainer::GetInstance(argc, argv);

  bsg::Generator* gen = new bsg::Generator();

  std::vector<std::vector<double> > spectrum = gen->CalculateSpectrum();

  delete gen;

  return &spectrum;
  //std::vector<std::vector<double> >* spectrum = new std::vector<std::vector<double> >();
  //return spectrum;
}

void BSG::WriteINIFile(const char* filename, Particle* initState, Particle* finalState, double Q) {
  // Create an empty property tree object.
  pt::ptree tree;

  //TODO Change Fermi/GT
  //Mae the most basic transition config file
  tree.put("Transition.Type", "Gamow-Teller");
  tree.put("Transition.QValue", Q);

  tree.put("Mother.Z", initState->GetCharge());
  tree.put("Mother.A", initState->GetCharge() + initState->GetNeutrons());
  tree.put("Daughter.Z", finalState->GetCharge());
  tree.put("Daughter.A", finalState->GetCharge() + finalState->GetNeutrons());
  //TODO Put actual spinparities in here
  tree.put("Mother.SpinParity", 2);
  tree.put("Daughter.SpinParity", 0);

  // Write property tree to INI file
  pt::write_ini(filename, tree);
}

SpectrumGenerator::SpectrumGenerator() { }

SpectrumGenerator::~SpectrumGenerator() { }

DeltaSpectrumGenerator::DeltaSpectrumGenerator() { }

BSG::BSG() { }

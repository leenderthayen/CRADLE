#ifndef CRADLE_WRITER_H
#define CRADLE_WRITER_H

#include "CRADLE/Event.h"

#include "PDS/Factory/ParticleFactory.h"

#include <iostream>
#include <string>

namespace CRADLE {

  void DisplayBannerInfo(std::ostream& out) {
    std::string author = "L. Hayen (lmhayen@ncsu.edu)";
    out << "-----------------------------------------------" << std::endl;
    //cout << "-  CRADLE++ version " << std::string(CRADLE_VERSION) << "      -" << endl;
    //cout << "-  Last update: " << std::string(CRADLE_LAST_UPDATE) << endl;
    out << "-  Author: " << author << std::endl;
    out << "-----------------------------------------------\n" << std::endl;
  }

  void WriteToFile(std::string& fullPath, const std::vector<Event>& events) {
    std::ofstream fstream(fullPath.c_str());

    if (fstream.is_open()) {
      DisplayBannerInfo(fstream);
      PDS::ParticleFactory::ListRegisteredParticles(fstream);

      fstream << "Definition of vertices" << std::endl;
      fstream << "----------------------" << std::endl;
      for (auto & s : Event::VertexFormat) {
        fstream << s << "\t";
      }
      fstream << std::endl;
      for (auto & e : events) {
        e.ListVertices(fstream);
      }
      fstream << "List of outgoing particles" << std::endl;
      fstream << "--------------------------" << std::endl;
      for (auto & s : Event::ParticleFormat) {
        fstream << s << "\t";
      }
      fstream << std::endl;
      for (auto & e : events) {
        e.ListParticlesOut(fstream);
      }
    } else {
      std::cerr << "Not able to open filename " << fullPath << std::endl;
    }
  }
}// End of CRADLE namespace


#endif

#include "CRADLE/Event.h"

namespace CRADLE {
  Event::Event() {

  }

  Event::~Event() {
    //TODO
  }

  void Event::AddVertex(std::vector<PDS::core::DynamicParticle> v, std::array<double, 4> p, PDS::core::ReactionModeName m) {
    Vertex vert = {v, p, m};

    vertices.push_back(vert);
  }
}//end of CRADLE namespace

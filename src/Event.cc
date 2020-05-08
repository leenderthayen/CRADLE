#include "CRADLE/Event.h"

namespace CRADLE {
  Event::Event() {
    ID = 0;
  }

  Event::~Event() {
    //TODO
  }

  void Event::AddVertex(std::vector<std::shared_ptr<PDS::core::DynamicParticle> > v, std::array<double, 4> p, PDS::core::ReactionModeName m) {
    // PDS::core::Vertex vert = {v, p, m};
    //
    // vertices.push_back(vert);
  }
}//end of CRADLE namespace

#ifndef CRADLE_EVENT_H
#define CRADLE_EVENT_H

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ParticleDefinition.h"

#include <vector>
#include <array>

namespace CRADLE {
  struct Vertex {
    std::vector<DynamicParticle&> particles;
    std::array<double, 4> position;
    ReactionModeName reactionModeName;
  };

  class Event {
  public:
    Event();
    ~Event();

    void Info();
    void AddVertex(std::vector<DynamicParticle&>, std::array<double, 4>, ReactionModeName);

    inline void AddVertex(Vertex& v) { vertices.push_back(v); }
  private:
    std::vector<Vertex> vertices;
  };
}

#endif

#ifndef CRADLE_EVENT_H
#define CRADLE_EVENT_H

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ParticleDefinition.h"

#include <vector>
#include <array>

namespace CRADLE {
  struct Vertex {
    std::vector<PDS::core::DynamicParticle> particles;
    std::array<double, 4> position;
    PDS::core::ReactionModeName reactionModeName;
  };

  class Event {
  public:
    Event();
    ~Event();

    void Info();
    void AddVertex(std::vector<PDS::core::DynamicParticle>, std::array<double, 4>, PDS::core::ReactionModeName);

    inline std::vector<Vertex> GetVertices() { return vertices; }

    inline void AddVertex(Vertex& v) { vertices.push_back(v); }
  private:
    std::vector<Vertex> vertices;
  };
}

#endif

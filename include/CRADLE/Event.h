#ifndef CRADLE_EVENT_H
#define CRADLE_EVENT_H

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ParticleDefinition.h"
#include

#include <vector>
#include <array>
#include <memory>
#include <iostream>

namespace CRADLE {
  class Event {
  public:
    Event();
    ~Event();

    void Info();
    void AddVertex(std::vector<std::shared_ptr<PDS::core::DynamicParticle> >, std::array<double, 4>, PDS::core::ReactionModeName);
    void GetInfo();

    friend std::ostream& operator<< (std::ostream &out, const Event &event);
    friend std::istream& operator>> (std::istream &in, Event &event);

    inline const std::vector<PDS::core::Vertex> GetVertices() const { return vertices; }
    inline void AddVertex(PDS::core::Vertex& v) { vertices.push_back(v); }
    inline const PDS::core::Vertex& GetLastVertex() const { return vertices.back(); }
  private:
    std::vector<PDS::core::Vertex> vertices;
    //TODO additional meta data
  };
}

#endif

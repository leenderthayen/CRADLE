#ifndef CRADLE_EVENT_H
#define CRADLE_EVENT_H

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ParticleDefinition.h"
#include "PDS/Core/Vertex.h"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

namespace CRADLE {
  class Event {
  public:
    Event(unsigned _ID) : ID(_ID) {};
    ~Event() {};

    void Info() {};
    void AddVertex(std::vector<std::shared_ptr<PDS::core::DynamicParticle> >, std::array<double, 4>, PDS::core::ReactionModeName);
    void GetInfo() {};
    void Print() {};

    //friend std::ostream& operator<< (std::ostream &out, const Event &event);
    //friend std::istream& operator>> (std::istream &in, Event &event);

    inline const std::vector<std::shared_ptr<PDS::core::Vertex> >& GetVertices() const { return vertices; }
    inline void AddVertex(std::shared_ptr<PDS::core::Vertex>& v) { vertices.push_back(v); }
    inline void AddVertices(std::vector<std::shared_ptr<PDS::core::Vertex> > v) { vertices.insert(vertices.end(), v.begin(), v.end()); }
    inline const PDS::core::Vertex& GetLastVertex() const { return *(vertices.back()).get(); }

    inline unsigned GetID() const { return ID; }
    inline void SetID(unsigned _ID) { ID = _ID; }

  private:
    std::vector<std::shared_ptr<PDS::core::Vertex> > vertices;
    unsigned ID;
    //TODO additional meta data
  };
}

#endif

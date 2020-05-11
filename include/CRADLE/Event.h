#ifndef CRADLE_EVENT_H
#define CRADLE_EVENT_H

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/Vertex.h"

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <array>

namespace CRADLE {
  class Event {
  public:
    Event(unsigned _ID) : ID(_ID) {};
    ~Event() {};

    std::string GetInfo() const;

    void ListVertices(std::ostream& out) const;
    void ListParticles(std::ostream& out, std::vector<std::shared_ptr<PDS::core::DynamicParticle> >) const;
    void ListParticlesOut(std::ostream& out) const;
    void ListParticlesIn(std::ostream& out) const;

    friend std::ostream& operator<< (std::ostream &out, const Event &event);
    friend std::istream& operator>> (std::istream &in, Event &event);

    inline const std::vector<std::shared_ptr<PDS::core::Vertex> >& GetVertices() const { return vertices; }
    inline void AddVertex(std::shared_ptr<PDS::core::Vertex>& v) { vertices.push_back(v); }
    inline void AddVertices(std::vector<std::shared_ptr<PDS::core::Vertex> > v) { vertices.insert(vertices.end(), v.begin(), v.end()); }
    inline const PDS::core::Vertex& GetLastVertex() const { return *(vertices.back()).get(); }

    inline unsigned GetNrVertices() const { return vertices.size(); }


    inline unsigned GetID() const { return ID; }
    inline void SetID(unsigned _ID) { ID = _ID; }

    static inline std::array<std::string, 6> VertexFormat = {"ID", "t[ns]", "delta t[ns]", "x[mm]", "y[mm]", "z[mm]"};
    static inline std::array<std::string, 11> ParticleFormat = {"PDGID\t", "Symbol", "Name", "Mass[MeV]", "ProdVertex",
    "DestrVertex", "Ekin[MeV]", "px", "py", "pz", "Eexc [MeV]"};

  private:
    std::vector<std::shared_ptr<PDS::core::Vertex> > vertices;
    unsigned ID;
  };
}

#endif

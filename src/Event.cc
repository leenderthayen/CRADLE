#include "CRADLE/Event.h"

#include "PDS/Units/GlobalSystemOfUnits.h"
#include "PDS/Units/GlobalPhysicalConstants.h"

namespace CRADLE {

  std::string Event::GetInfo() const {
    std::ostringstream oss;
    oss << "Event ID: " << ID << " Nr Vertices: " << GetNrVertices() << std::endl;
    return oss.str();
  }

  void Event::ListVertices(std::ostream& out) const {
    for (auto & v : vertices) {
      out << GetID() << "." << v->GetID() << "\t" << v->GetPosition()[0] / c_light / ns
      << "\t" << (v->GetParticlesIn()[0]->GetProductionVertex()->GetPosition()[0] - v->GetPosition()[0]) / c_light / ns
      << "\t" << v->GetPosition()[1] / mm << "\t" << v->GetPosition()[2] / mm << "\t"
      << v->GetPosition()[3] / mm << std::endl;
    }
  }

  void Event::ListParticles(std::ostream& out, std::vector<std::shared_ptr<PDS::core::DynamicParticle> > pv) const {
    for (auto & p : pv) {
      std::string destrVertex = "-";
      if (p->GetDestructionVertex()) {
        destrVertex = std::to_string(GetID()) + "." + std::to_string(p->GetDestructionVertex()->GetID());
      }
      out << p->GetParticle().GetID() << "\t" << p->GetParticle().GetSymbol() << "\t"
      << p->GetName() << "\t" << p->GetRestMass() / MeV << "\t"
      << GetID() << "." << p->GetProductionVertex()->GetID() << "\t"
      << destrVertex << "\t" << p->GetKinEnergy() / MeV << "\t"
      << p->GetFourMomentum()[1] / MeV << "\t"
      << p->GetFourMomentum()[2] / MeV << "\t"
      << p->GetFourMomentum()[3] / MeV << "\t"
      << p->GetParticle().GetExcitationEnergy() / MeV
      << std::endl;
    }
  }

  void Event::ListParticlesOut(std::ostream& out) const{
    for (auto & v : vertices) {
      ListParticles(out, v->GetParticlesOut());
    }
  }

  void Event::ListParticlesIn(std::ostream& out) const {
    for (auto & v : vertices) {
      ListParticles(out, v->GetParticlesIn());
    }
  }

  std::ostream& operator<< (std::ostream &out, const Event &event) {
    out << "# Definition of vertices" << std::endl;
    for (auto & s : event.VertexFormat) {
      out << s << "\t";
    }
    out << std::endl;
    event.ListVertices(out);

    out << "------------------------------------" << std::endl;
    out << "Definition of particles" << std::endl;
    for (auto & s : event.ParticleFormat) {
      out << s << "\t";
    }
    out << std::endl;
    event.ListParticlesOut(out);

    return out;
  }
}//end of CRADLE namespace

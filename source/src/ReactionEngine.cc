#include "ReactionEngine.hh"
#include "PDS/core/DynamicParticle.h"

ReactionEngine::ReactionEngine(std::vector<DynamicParticle *> _dynamicParticles) : dynamicParticles(_dynamicParticles){}

ReactionEngine::~ReactionEngine(){}

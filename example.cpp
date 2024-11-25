/*
 * A tiny LHEF reader.
 *
 * Minimal C++17 implementation handling only <event> elements.
 * Written by <lyazj@github.com>.
 */
#include <iostream>

#include "LHEF.h"

using namespace std;

int main()
{
  LHEF lhef("example.lhe");
  LHEEvent event;
  for(size_t ievent = 0; lhef.readEvent(event); ++ievent) {
    cout << "Event " << ievent << ": " << event.nparticle << " particles" << endl;
    for(size_t iparticle = 0; iparticle < event.nparticle; ++iparticle) {
      cout << " Particle " << iparticle << ": " << event.particles[iparticle].pid << " "
           << event.particles[iparticle].status << endl;
    }
  }
  return 0;
}

/*
 * A tiny LHEF reader.
 *
 * Minimal C++17 implementation handling only <event> elements.
 * Written by <lyazj@github.com>.
 */
#include <iomanip>
#include <iostream>

#include "LHEF.h"

using namespace std;

int main()
{
  LHEF lhef("example.lhe");
  LHEEvent event;
  cout << scientific << showpos << setprecision(3);
  for(size_t ievent = 0; lhef.readEvent(event); ++ievent) {
    cout << "Event " << setw(3) << ievent << ": " << setw(2) << event.nparticle << " particles" << endl;
    for(size_t iparticle = 0; iparticle < event.nparticle; ++iparticle) {
      const LHEParticle &particle = event.particles[iparticle];
      cout << " Particle " << setw(2) << iparticle << ": " << setw(4) << particle.pid << "  " << setw(2)
           << particle.status;
      cout << "  " << particle.px << "  " << particle.py << "  " << particle.pz << "  " << particle.e << "  "
           << particle.m << endl;
    }
  }
  return 0;
}

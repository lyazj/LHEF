/*
 * A tiny LHEF reader.
 *
 * Minimal C++17 implementation handling only <event> elements.
 * Written by <lyazj@github.com>.
 */
#include "LHEF.h"

#include <tinyxml2.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

using namespace std;
using namespace tinyxml2;

LHEF::LHEF(const char *lhepath)
{
  // Load LHE file as an XML document.
  document_ = new XMLDocument;
  if(document_->LoadFile(lhepath)) throw runtime_error("error loading LHE file: "s + lhepath);

  // Mark the beginning of the event loop.
  event_ = NULL;
}

LHEF::~LHEF() { delete document_; }

bool LHEF::readEvent(LHEEvent &event)
{
  // Locate the first/next event.
  if(event_ == NULL) {
    event_ = document_->RootElement()->FirstChildElement("event");
  } else {
    event_ = event_->NextSiblingElement("event");
  }
  if(event_ == NULL) return false;

  // Parse the event.
  event.fromXML(event_);
  return true;
}

class LHEEventStream : private istringstream {
  template<class T>
  friend LHEEventStream &operator>>(LHEEventStream &stream, T &t);

public:
  using istringstream::basic_istringstream;
  using istringstream::operator bool;
  using istringstream::operator!;
};

template<class T>
LHEEventStream &operator>>(LHEEventStream &stream, T &t)
{
  if constexpr(is_floating_point_v<T>) {
    // Supports conversion from nan.
    string s;
    stream >> s;
    t = stod(s);
  } else {
    ((istringstream &)stream) >> t;
  }
  return stream;
}

void LHEEvent::fromXML(const tinyxml2::XMLElement *element)
{
  // Parse the element content as text.
  LHEEventStream text(element->GetText());

  // Parse event information.
  text >> nparticle >> process_id >> weight >> scale >> alpha_qed >> alpha_qcd;
  if(!text) throw runtime_error("error parsing event information");

  // Parse particle information.
  for(size_t i = 0; i < nparticle; ++i) {
    particles.emplace_back();
    LHEParticle &particle = particles.back();
    text >> particle.pid >> particle.status;
    text >> particle.mother1 >> particle.mother2 >> particle.color1 >> particle.color2;
    text >> particle.px >> particle.py >> particle.pz >> particle.e >> particle.m;
    text >> particle.lifetime >> particle.spin;
  }
  if(!text) throw runtime_error("error parsing particle information");

  // End of content test.
  if(string s; text >> s) throw runtime_error("trailing event content: " + s);
}

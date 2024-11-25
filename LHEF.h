/*
 * A tiny LHEF reader.
 *
 * Minimal C++17 implementation handling only <event> elements.
 * Written by <lyazj@github.com>.
 */
#pragma once

#include <vector>

namespace tinyxml2 {

class XMLElement;
class XMLDocument;

}  // namespace tinyxml2

class LHEParticle {
public:
  long pid;
  long status;
  long mother1;
  long mother2;
  long color1;
  long color2;
  double px;
  double py;
  double pz;
  double e;
  double m;
  double lifetime;
  double spin;
};

class LHEEvent {
public:
  size_t nparticle;
  long process_id;
  double weight;
  double scale;
  double alpha_qed;
  double alpha_qcd;
  std::vector<LHEParticle> particles;

  void fromXML(const tinyxml2::XMLElement *);
};

class LHEF {
public:
  LHEF(const char *lhepath);
  ~LHEF();
  LHEF(const LHEF &) = delete;

  bool readEvent(LHEEvent &);

private:
  tinyxml2::XMLDocument *document_;
  tinyxml2::XMLElement *event_;
};

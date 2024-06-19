#ifndef FUZZ_H
#define FUZZ_H

#include <vector>
#include <string>
#include "effect.h"

namespace upc {
  class Fuzz: public upc::Effect {
    private:
      float gain, threshold, knee_width, mix, hf_damp, oversampling;
      float s1, x1, s2, x2;
      float fase, inc_fase;
    public:
      Fuzz(const std::string &param = "");
      void operator()(std::vector<float> &x);
      void command(unsigned int);
  };
}

#endif

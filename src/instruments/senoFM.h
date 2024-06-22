
#ifndef SENO_FM
#define SENO_FM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc
{
  class SenoFM : public upc::Instrument
  {
    EnvelopeADSR adsr;
    float index, N1, N2, setting;
    int N, decay_count, decay_count_I, note_int;
    float A, index_step, index_sen, Nnote,adsr_s,adsr_a,adsr_r,adsr_d, max_level;
    std::vector<float> tbl, x_tm;
    long double mod_phase, mod_phase_step;
    float fm, I1, I2;

  public:
    SenoFM(const std::string &param = "");
    void command(long cmd, long note, long velocity = 1);
    const std::vector<float> &synthesize();
    bool is_active() const { return bActive; }
  };
}

#endif

#ifndef SENO_FM
#define SENO_FM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc
{
  // Class SenoFM inherits from Instrument and implements FM synthesis with sine waves.
  class SenoFM : public upc::Instrument
  {
    EnvelopeADSR adsr; // ADSR envelope generator
    float index, N1, N2, setting; // Parameters for frequency modulation
    int N, decay_count, decay_count_I, note_int; // Various counters and settings
    float A, index_step, index_sen, Nnote, adsr_s, adsr_a, adsr_r, adsr_d, max_level; // Additional parameters
    std::vector<float> tbl, x_tm; // Tables for waveform and temporary storage
    long double mod_phase, mod_phase_step; // Phases for modulation
    float fm, I1, I2; // Frequency modulation parameters

  public:
    // Constructor that initializes parameters based on optional input 'param'.
    SenoFM(const std::string &param = "");

    // Process commands to start, sustain, or release a note.
    void command(long cmd, long note, long velocity = 1);

    // Synthesize and return a vector of float samples.
    const std::vector<float> &synthesize();

    // Check if the instrument is currently active.
    bool is_active() const { return bActive; }
  };
}

#endif

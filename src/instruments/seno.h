#ifndef INSTRUMENT_SENO
#define INSTRUMENT_SENO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc
{
    class InstrumentSeno : public Instrument
    {
        EnvelopeADSR adsr;
        unsigned int index;
        float A;
        std::vector<float> tbl;
        float f0, increment, phas;
        bool Interpolation;

    public:
        InstrumentSeno(const std::string &param = "");
        void command(long cmd, long note, long velocity = 10);
        const std::vector<float> &synthesize();
        bool is_active() const { return bActive; }

    private:
        // internal class function for interpolation between tbl[] values
        float getInterpolatedValue(const float phas);
    };
}

#endif

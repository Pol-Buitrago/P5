#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentSeno::InstrumentSeno(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  int N;
  if (!kv.to_int("N",N))
    N = 40; //default value
  
  // float f_fundamental; //valor de la frecuencia fundamental del instrumento seno
  // float f_discrete; //frecuencia normalizada (discreta)

  // if (!kv.to_float("f",f_fundamental)) //cambiar "f" en caso que fuese otro carácter el que deberia ir
  //     f_fundamental= 10; //default value (en herzios)

  // f_discrete = f_fundamental/SamplingRate;

  //   tbl.resize(N);
  //   index = 0;
  //   for (int i=0; i < N ; ++i) {
  //     tbl[i] = sin(2*M_PI *f_discrete*i); //generación de la onda
  //   }

  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}


void InstrumentSeno::command(long cmd, long note, long vel) {
  f0 = 440.0f * pow(2.0f, (note - 69.0f) / 12.0f);

  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    index = 0;
    phas = 0.0f;
    increment = ((f0 / SamplingRate) * tbl.size());
    A = vel / 127.;
    // A = std::clamp(static_cast<float>(vel) / 127.0f, 0.0f, 1.0f);
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentSeno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    phas += increment;
    x[i] = A * tbl[round(phas)];
    while(phas >= tbl.size()) phas = phas - tbl.size();
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}

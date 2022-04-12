#ifndef BLUR_FILTER_H_
#define BLUR_FILTER_H_

#include <cstdint>
#include <systemc>
using namespace sc_core;

#include <tlm_utils/simple_target_socket.h>

#include "utility.h"

#define EXE_LANTENCY 2
#define HLS_CLOCK_PERIOD 10

class BlurFilter : public sc_module {
public:
  tlm_utils::simple_target_socket<BlurFilter> t_skt;

  sc_fifo<pixel> i_p;
  sc_fifo<pixel> o_p;

  SC_HAS_PROCESS(BlurFilter);
  BlurFilter(sc_module_name n);
  ~BlurFilter() = default;
  
private:
  void  do_filter();

  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif

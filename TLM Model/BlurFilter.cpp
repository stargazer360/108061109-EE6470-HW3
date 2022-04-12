#include <cmath>
#include <iomanip>

#include "BlurFilter.h"

BlurFilter::BlurFilter(sc_module_name n) 
  : sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_filter);

  t_skt.register_b_transport(this, &BlurFilter::blocking_transport);
}

const double factor = 1.0 / 16.0;
const double bias = 0.0;

void BlurFilter::do_filter() {
  while (true) {
    double r = 0.0, g = 0.0, b = 0.0;
    pixel input, result;
    for (uint8_t y = 0; y < FILTER_H; ++y) {
      for (uint8_t x = 0; x < FILTER_W; ++x) {
        input = i_p.read();
        r += (double)input.r() * filter[y][x];
        g += (double)input.g() * filter[y][x];
        b += (double)input.b() * filter[y][x];
      }
    }
    result.set( static_cast<uint8_t>(std::min(std::max(int(factor * r + bias), 0), 255)),
                static_cast<uint8_t>(std::min(std::max(int(factor * g + bias), 0), 255)),
                static_cast<uint8_t>(std::min(std::max(int(factor * b + bias), 0), 255)));
    wait(EXE_LANTENCY * HLS_CLOCK_PERIOD, SC_NS);
    o_p.write(result);
  }
}

void BlurFilter::blocking_transport(tlm::tlm_generic_payload &payload,
                                    sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr -= base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  pixel p_buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case BLUR_FILTER_RESULT_ADDR:
      p_buffer = o_p.read();
      data_ptr[0] = p_buffer.r();
      data_ptr[1] = p_buffer.g();
      data_ptr[2] = p_buffer.b();
      break;
    case BLUR_FILTER_CHECK_ADDR:
      buffer.sint = o_p.num_available();
      data_ptr[0] = buffer.uchar[0];
      data_ptr[1] = buffer.uchar[1];
      data_ptr[2] = buffer.uchar[2];
      data_ptr[3] = buffer.uchar[3];
      break;
    default:
      std::cerr << "Error! BlurFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is nota valid" << std::endl; 
      break;
    }
    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case BLUR_FILTER_R_ADDR:
      if ((mask_ptr[0] == TLM_BYTE_ENABLED) && 
          (mask_ptr[1] == TLM_BYTE_ENABLED) && 
          (mask_ptr[2] == TLM_BYTE_ENABLED)) {
        p_buffer.set(data_ptr[0], data_ptr[1], data_ptr[2]);
       
        i_p.write(p_buffer);
      }
      break;
    default:
      std::cerr << "Error! BlurFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE);
}
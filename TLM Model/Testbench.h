#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <stdint.h>

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include "utility.h"
#include "Initiator.h"
#include "MemoryMap.h"

class Testbench : public sc_module {
public:
  Initiator initiator;
  
  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench() = default;

  int32_t read_bmp(string infile_name);
  int32_t write_bmp(string outfile_name);

  uint32_t get_width() { return width; }

  uint32_t get_height() { return height; }

  uint32_t get_width_bytes() { return width_bytes; }

  uint32_t get_bytes_per_pixel() { return bytes_per_pixel; }

  uint8_t *get_source_image() { return source_bitmap; }
  uint8_t *get_target_image() { return target_bitmap; }

private:
  uint32_t input_rgb_raw_data_offset;
  const uint32_t output_rgb_raw_data_offset;
  int32_t width;
  int32_t height;
  uint32_t width_bytes;
  uint8_t bits_per_pixel;
  uint16_t bytes_per_pixel;
  uint8_t *source_bitmap;
  uint8_t *target_bitmap;

  void do_blur();
};
#endif

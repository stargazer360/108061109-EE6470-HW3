#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "SobelFilter_pipe_unroll.h"

SobelFilter::SobelFilter( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);
	o_rgb.clk_rst(i_clk, i_rst);
#endif
}

SobelFilter::~SobelFilter() {}

// blur filter
const unsigned char filter[MASK_X * MASK_Y] = 
{
  0, 1, 0,
  1, 2, 1,
  0, 1, 0,
};

void SobelFilter::do_filter() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_rgb.reset();
#endif
		wait();
	}
	while (true) {
		HLS_PIPELINE_LOOP(HARD_STALL, 2, "main_loop_pipeline");
		sc_dt::sc_uint<12> r = 0, g = 0, b = 0;
		for (unsigned int i = 0; i < MASK_Y * MASK_X; ++i) {
			sc_dt::sc_uint<24> _i_rgb;
#ifndef NATIVE_SYSTEMC
			{
					HLS_DEFINE_PROTOCOL("input");
					_i_rgb = i_rgb.get();
					wait();
			}
#else
			_i_rgb = i_rgb.read();
#endif
			{
				HLS_CONSTRAIN_LATENCY(0, 1, "lat01");
				r += (_i_rgb.range(7,0) << filter[i]);
				g += (_i_rgb.range(15,8) << filter[i]);
				b += (_i_rgb.range(23, 16) << filter[i]);
			}
		}
		sc_dt::sc_uint<24> _o_rgb;
		{
			HLS_CONSTRAIN_LATENCY(0, 1, "lat02");
 			_o_rgb.range(7, 0) = (sc_dt::sc_uint<8>)(r >> 4);
        	_o_rgb.range(15, 8) = (sc_dt::sc_uint<8>)(g >> 4);
        	_o_rgb.range(23, 16) = (sc_dt::sc_uint<8>)(b >> 4);
		}
#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_rgb.put(_o_rgb);
			wait();
		}
#else
		o_rgb.write(_o_rgb);
#endif
	}
}

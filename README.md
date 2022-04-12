# Homework 3
Synthesizable Gaussian blur filter.
## Compile & Run
For Stratus HLS
- `$ cd Stratus`
- `$ cd stratus`
- `$ make sim_V_B # for behavioral simulation`
- `$ make sim_V_BASIC or make sim_V_DPA for synthesis and verilog simulation`
You can choose different version of the filter by including the header file of the version (like SobelFilter_pipe) in system.h and remove the previous included one.

For TLM model
 - `$ cd TLM Model`
 - `$ mkdir build`
 - `$ cd build`
 - `$ cmake ..`
 - `$ make`
 - `$ make run`

Alter the EXE_LANTENCY in `$ BlurFilter.h` corresponding to lantency per pixel in previous simulation to get simulation time in TLME model with time annotation.


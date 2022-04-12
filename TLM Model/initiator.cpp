#include"Initiator.h"

Initiator::Initiator(sc_module_name n) : sc_module(n), i_skt("i_skt") {}

// SetRead data from base address into rdata with specify data length
int Initiator::read_from_socket(unsigned long int addr, unsigned char mask[],
                                unsigned char rdata[], int dataLen) 
{
  // Set up generic payload attribute
  trans.set_read();
  trans.set_address((sc_dt::uint64)addr);
  trans.set_data_length((unsigned int)dataLen);
  trans.set_data_ptr((unsigned char* )rdata);
  // Set up byte enable mask(only 0x00 and 0xff shall be used in each value)
  trans.set_byte_enable_length((unsigned int) dataLen);
  trans.set_byte_enable_ptr((unsigned char* )mask);
  // Transport data
  do_trans(trans);
  // return non-zero value on error 
  return trans.is_response_ok() ? 0 : -1; 
}

// Read data from wdata into base address with specify data length
int Initiator::write_to_socket(unsigned long int addr, unsigned char mask[],
                                unsigned char wdata[], int dataLen)
{
  // Set up generic payload attribute
  trans.set_write();
  trans.set_address((sc_dt::uint64)addr);
  trans.set_data_length((unsigned int)dataLen);
  trans.set_data_ptr((unsigned char* )wdata);
  // Set up byte enable mask(only 0x00 and 0xff shall be used in each value)
  trans.set_byte_enable_length((unsigned int)dataLen);
  trans.set_byte_enable_ptr((unsigned char* )mask);

  // Transport data
  do_trans(trans);

  // return non-zero value on error 
  return trans.is_response_ok() ? 0 : -1; 
}

// call blocking transport and wait for no time
void Initiator::do_trans(tlm::tlm_generic_payload &trans)
{
  sc_time delay = SC_ZERO_TIME;

  i_skt->b_transport(trans, delay);
  wait(delay);
}
#include "utility.h"

std::ostream& operator<<(std::ostream& os,  const pixel& p)
{
  os << "red: " << (uint32_t)p.r() << ", green: " << (uint32_t)p.g() << ", blue: " << (uint32_t)p.b();
  return os;
} // end
// trace function, only required if actually used
void sc_trace(sc_trace_file*& tf, const pixel& p, string nm)
{
  sc_trace(tf, p.r(), nm + ".red");
  sc_trace(tf, p.g(), nm + ".green");
  sc_trace(tf, p.b(), nm + ".blue");
}
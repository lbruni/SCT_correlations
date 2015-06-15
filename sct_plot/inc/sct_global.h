#ifndef sct_global_h__
#define sct_global_h__
#include "sct_event_buffer.h"

namespace sct_corr{
  class Buffer_accessor{
  public:
    Buffer_accessor(sct_event_buffer * buffer);
    ~Buffer_accessor();

    static sct_event_buffer *getGlobalPlotCollection();

  };
}
#endif // sct_global_h__

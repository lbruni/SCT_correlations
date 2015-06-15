#include "sct_event_buffer.h"
#include "sct_global.h"
#include <mutex>


namespace sct_corr{
  std::mutex gplotBuffer_mutex;
  sct_event_buffer *gPlotCollection = nullptr;



  Buffer_accessor::Buffer_accessor(sct_event_buffer * buffer)
  {
    gplotBuffer_mutex.lock();
    gPlotCollection = buffer;
  }

  Buffer_accessor::~Buffer_accessor()
  {
    gPlotCollection = nullptr;
    gplotBuffer_mutex.unlock();
  }

  sct_event_buffer * Buffer_accessor::getGlobalPlotCollection()
  {
    return gPlotCollection;
  }
}
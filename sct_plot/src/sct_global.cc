#include "sct_event_buffer.h"

 
sct_event_buffer *gPlotCollection;

sct_event_buffer *getGlobalPlotCollection(){
  return gPlotCollection;
}

void setGlobalPlotCollection(sct_event_buffer* buffer){
  gPlotCollection = buffer;
}

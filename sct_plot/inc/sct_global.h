#ifndef sct_global_h__
#define sct_global_h__
#include "sct_event_buffer.h"

sct_event_buffer *getGlobalPlotCollection();

void setGlobalPlotCollection(sct_event_buffer* buffer);

#endif // sct_global_h__

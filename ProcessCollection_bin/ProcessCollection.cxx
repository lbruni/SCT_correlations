
#include "../sct_plot/inc/s_process_files.h"
#include "../sct_plot/inc/sct_runs.h"
#include "TError.h"





int main(int argc, char **argv) {


  s_process_files p;

 
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 

  sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_350V(p,"D:/data/DEVICE_1_ASIC_on_Position_7_Jim_350V/","D:/data/DEVICE_1_ASIC_on_Position_7_Jim_350V/" );
  //sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_150V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_150V/","D:/data/DEVICE_1_ASIC_on_Position_7_Jim_150V/");
  //sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_250V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_250V/", "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_250V/");

 // sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_400V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_400V/", "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_400V/");

  p.process();




}

 
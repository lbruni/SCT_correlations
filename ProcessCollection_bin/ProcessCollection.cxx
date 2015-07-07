
#include "../sct_plot/inc/s_process_files.h"
#include "../sct_plot/inc/sct_runs.h"
#include "TError.h"



void run_DEVICE_1_ASIC_on_Position_5_150V(){
  s_process_files p;

  sct_runs::DEVICE_1_ASIC_on_Position_5_150V(p, "D:/data/DEVICE_1_ASIC_on_Position_5_150V/", "D:/data/DEVICE_1_ASIC_on_Position_5_150V/");
  p.process();

}


void DEVICE_1_ASIC_on_Position_5_250V(){

  s_process_files p;
    sct_runs::DEVICE_1_ASIC_on_Position_5_250V(p, "D:/data/DEVICE_1_ASIC_on_Position_5_250V/", "D:/data/DEVICE_1_ASIC_on_Position_5_250V/");
  
  p.process();
}

void DEVICE_1_ASIC_on_Position_5_350V(){


  s_process_files p;
  sct_runs::DEVICE_1_ASIC_on_Position_5_350V(p, "D:/data/DEVICE_1_ASIC_on_Position_5_350V/", "D:/data/DEVICE_1_ASIC_on_Position_5_350V/");
  p.process();
}


void DEVICE_1_ASIC_on_Position_5_400V(){
  s_process_files p;
  sct_runs::DEVICE_1_ASIC_on_Position_5_400V(p, "D:/data/DEVICE_1_ASIC_on_Position_5_400V/", "D:/data/DEVICE_1_ASIC_on_Position_5_400V/");
  p.process();
}


void DEVICE_2_ASIC_on_Position_5_150V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_5_150V(p, "D:/data/DEVICE_2_ASIC_on_Position_5_150V/", "D:/data/DEVICE_2_ASIC_on_Position_5_150V/");
  p.process();
}

void DEVICE_2_ASIC_on_Position_5_250V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_5_250V(p, "D:/data/DEVICE_2_ASIC_on_Position_5_250V/", "D:/data/DEVICE_2_ASIC_on_Position_5_250V/");
  p.process();


}

void DEVICE_2_ASIC_on_Position_5_350V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_5_350V(p, "D:/data/DEVICE_2_ASIC_on_Position_5_350V/", "D:/data/DEVICE_2_ASIC_on_Position_5_350V/");
  p.process();
}

void DEVICE_2_ASIC_on_Position_7_150V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_7_150V(p, "D:/data/DEVICE_2_ASIC_on_Position_7_150V/", "D:/data/DEVICE_2_ASIC_on_Position_7_150V/");
  p.process();
}

void DEVICE_2_ASIC_on_Position_7_250V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_7_250V(p, "D:/data/DEVICE_2_ASIC_on_Position_7_250V/", "D:/data/DEVICE_2_ASIC_on_Position_7_250V/");
  p.process();
}


void DEVICE_2_ASIC_on_Position_7_350V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_7_350V(p, "D:/data/DEVICE_2_ASIC_on_Position_7_350V/", "D:/data/DEVICE_2_ASIC_on_Position_7_350V/");
  p.process();

}

void DEVICE_2_ASIC_on_Position_7_400V(){
  s_process_files p;
  sct_runs::DEVICE_2_ASIC_on_Position_7_400V(p, "D:/data/DEVICE_2_ASIC_on_Position_7_400V/", "D:/data/DEVICE_2_ASIC_on_Position_7_400V/");

  p.process();

}


void DEVICE_1_ASIC_on_Position_7_Jim_150V(){
  s_process_files p;
  
  sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_150V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_150V/", "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_150V/");
  p.process();

}
void DEVICE_1_ASIC_on_Position_7_Jim_250V(){
  s_process_files p;

  sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_250V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_250V/", "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_250V/");
  p.process();

}
void DEVICE_1_ASIC_on_Position_7_Jim_350V(){
  s_process_files p;

  sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_350V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_350V/", "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_350V/");
  p.process();
}

void DEVICE_1_ASIC_on_Position_7_Jim_400V(){
  s_process_files p;

  sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_400V(p, "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_400V/", "D:/data/DEVICE_1_ASIC_on_Position_7_Jim_400V/");
  p.process();
}
int main(int argc, char **argv) {



 
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 



 run_DEVICE_1_ASIC_on_Position_5_150V();


 DEVICE_1_ASIC_on_Position_5_250V();

 DEVICE_1_ASIC_on_Position_5_350V();


 DEVICE_1_ASIC_on_Position_5_400V();


 DEVICE_2_ASIC_on_Position_5_150V();

 DEVICE_2_ASIC_on_Position_5_250V();

 DEVICE_2_ASIC_on_Position_5_350V();

 DEVICE_2_ASIC_on_Position_7_150V();

 DEVICE_2_ASIC_on_Position_7_250V();

 DEVICE_2_ASIC_on_Position_7_350V();

 DEVICE_2_ASIC_on_Position_7_400V();

 DEVICE_1_ASIC_on_Position_7_Jim_150V();
 DEVICE_1_ASIC_on_Position_7_Jim_250V();
 DEVICE_1_ASIC_on_Position_7_Jim_350V();

 DEVICE_1_ASIC_on_Position_7_Jim_400V();

}

 
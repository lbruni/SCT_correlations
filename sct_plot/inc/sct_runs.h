#ifndef sct_runs_h__
#define sct_runs_h__
#include "internal/platform.hh"
#include <string>
class s_process_files;
class DllExport sct_runs{
public:
  static void DEVICE_1_ASIC_on_Position_7_Jim_150V(s_process_files& p, std::string path__,std::string outputPath=".");
  static void DEVICE_1_ASIC_on_Position_7_Jim_350V(s_process_files& p, std::string path__,std::string outputPath=".");
  static void DEVICE_1_ASIC_on_Position_7_Jim_250V(s_process_files& p, std::string path__,std::string outputPath=".");
  static void DEVICE_1_ASIC_on_Position_7_Jim_400V(s_process_files& p, std::string path__,std::string outputPath=".");


  static void DEVICE_1_ASIC_on_Position_5_150V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_1_ASIC_on_Position_5_250V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_1_ASIC_on_Position_5_350V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_1_ASIC_on_Position_5_400V(s_process_files& p, std::string path__, std::string outputPath = ".");


  static void DEVICE_2_ASIC_on_Position_5_150V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_2_ASIC_on_Position_5_250V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_2_ASIC_on_Position_5_350V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_2_ASIC_on_Position_5_400V(s_process_files& p, std::string path__, std::string outputPath = ".");



  static void DEVICE_2_ASIC_on_Position_7_150V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_2_ASIC_on_Position_7_250V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_2_ASIC_on_Position_7_350V(s_process_files& p, std::string path__, std::string outputPath = ".");
  static void DEVICE_2_ASIC_on_Position_7_400V(s_process_files& p, std::string path__, std::string outputPath = ".");


};

#ifdef __CINT__

#pragma link C++ class  sct_runs;

#endif

#endif // sct_runs_h__

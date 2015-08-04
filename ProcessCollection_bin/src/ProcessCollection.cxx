
#include "s_process_files.h"
#include "TError.h"


#include <iostream>
#include <sstream>


#include "tclap/CmdLine.h"
#include "xml_helpers/xml_fileList.hh"
#include <thread>


using namespace xml_util;

using namespace rapidxml;


int ADDRun(s_process_files& p, std::string xmlInputFileName, std::string path__, std::string outputPath = "."){
  path__ += "/";
  xmlImputFiles::XML_imput_file xml_imput(xmlInputFileName.c_str());

  if (xml_imput.fileList().empty())
  {
    return -1;
  }
  auto collname = xml_imput.globalConfig().CollectionName();
  outputPath += "/" + collname + ".root";

  p.setOutputName(outputPath.c_str());

  p.SetNumberOfBins(xml_imput.globalConfig().NumberOfBins());
  p.AddCut(xml_imput.globalConfig().cut());
  p.setActiveArea(xml_imput.globalConfig().AvtiveStrips().getMin(), xml_imput.globalConfig().AvtiveStrips().getMax());
  p.SetRotation(xml_imput.globalConfig().Rotation());
  p.SetPosition(xml_imput.globalConfig().Position_value(), 0);
  p.setResidualCut(xml_imput.globalConfig().residual_cut());

  for (auto& e : xml_imput.fileList()){
    p.push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());
  }

  return 0;
}

using namespace std;
using namespace TCLAP;

void remove_root_printouts() {
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 
}

struct  inParam {
  int argc;
  char **argv;
};


int asyncMain(void *arg) {

  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;

  try {

    CmdLine cmd("ProcessCollection", ' ', "0.1");
    ValueArg<std::string> FileNameArg("i", "inFile", "xml filename", true, "", "string");
    cmd.add(FileNameArg);
    ValueArg<std::string>  inPath("p", "inPath", "path to the root files", true, "", "string");
    cmd.add(inPath);
    ValueArg<std::string>  outpath("o", "outPath", "output path", false, ".", "string");
    cmd.add(outpath);
#ifdef _DEBUG
    cmd.setExceptionHandling(false);
#endif // _DEBUG

    cmd.parse(argc, argv);
    s_process_files p;
    ADDRun(p, FileNameArg.getValue(), inPath.getValue(), outpath.getValue());
    p.process();
  } catch (ArgException &e)  // catch any exceptions
  {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return -1;
  }
  exit(0);
  return 0;
}
int main(int argc, char **argv) {

  remove_root_printouts();
  
  




  inParam para;
  para.argc = argc;
  para.argv = argv;

  std::cout << "press q to quit the program" << std::endl;
  std::thread thr(asyncMain, &para);
  std::string i;
  
  while (i != "q") {
    std::cin >> i;

  }


  return 0;

}


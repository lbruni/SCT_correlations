
#include "../sct_plot/inc/s_process_files.h"
#include "../sct_plot/inc/sct_runs.h"
#include "TError.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <iostream>
#include <sstream>
#include "rapidxml.hpp"
#include "../sct_plot/inc/xml_helpers/xml_util.hh"
#include "tclap/CmdLine.h"

using namespace xml_util;

using namespace rapidxml;

namespace xmlImputFiles{



  using xml_n = xml_node < char > ;
  template<typename T>
  class MinMaxRange{
  public:
    MinMaxRange(T min_, T max_) :m_min(min_), m_max(max_){}
    T getMin() const{
      return m_min;
    }
    T getMax() const{
      return m_max;
    }
  private:
    T m_min, m_max;
  };
  using intRange = MinMaxRange < int > ;
  class xml_globalConfig{
  public:
    xml_globalConfig(xml_n* xIn) :m_node(xIn){
      // std::cout << xIn->name << std::endl;
    }
    xml_globalConfig(){}
    std::string CollectionName() const {
      return getAtribute(m_node->first_node("CollectionName"), "value", "notSet");
    }
    int NumberOfBins() const{
      return getAtribute(m_node->first_node("NumberOfBins"), "value", -1);
    }
    S_Cut cut() const{
      auto min_ = getAtribute(m_node->first_node("YCut"), "min", -1);
      auto max_ = getAtribute(m_node->first_node("YCut"), "max", -1);
      return S_YCut(min_, max_);
    }
    int Device() const{
      return getAtribute(m_node->first_node("Device"), "value", -1);
    }

    intRange AvtiveStrips() const{
      auto min_ = getAtribute(m_node->first_node("AvtiveStrips"), "min", -1);
      auto max_ = getAtribute(m_node->first_node("AvtiveStrips"), "max", -1);
      return intRange(min_, max_);
    }
    double Rotation()const {
      return getAtribute(m_node->first_node("Rotation"), "value", 0.0);
    }
    std::string Position_name()const{
      return getAtribute(m_node->first_node("Position"), "name", "notSet");
    }
    double Position_value()const {
      return getAtribute(m_node->first_node("Position"), "value", -1.0);
    }
  private:
    xml_n* m_node = nullptr;
  };
  class xml_file{
  public:
    xml_file(xml_n* xIn) :
      m_node(xIn){

    }
    static const char* NodeName() {
      return "file";
    }
    xml_file(){}
    std::string name() const {
      return getAtribute(m_node->first_node("name"), "value", "notSet");
    }
    double threshold() const {
      return getAtribute(m_node->first_node("threshold"), "value", -1.0);
    }
    double HV() const{
      return getAtribute(m_node->first_node("HV"), "value", -1.0);
    }
    int runNumber() const {
      return getAtribute(m_node->first_node("runNumber"), "value", -1);
    }
  private:
    xml_n* m_node = nullptr;
  };
  class XML_imput_file{
  public:
    XML_imput_file(const char* name) :m_file(name) {
      m_doc.parse<0>(m_file.data());    // 0 means default parse flags
      //      auto p = m_doc.next_sibling("");
      m_globalConfig = xml_globalConfig(m_doc.first_node("RunCollection")->first_node("globalConfig"));

      m_files = getVectorOfT<xml_file>(m_doc.first_node("RunCollection")->first_node("fileList"));

    }

    const xml_globalConfig& globalConfig() const {
      return m_globalConfig;
    }
    const std::vector<xml_file>& fileList() const{
      return m_files;

    }


  private:
    xml_document<> m_doc;
    rapidxml::file<> m_file;
    xml_globalConfig m_globalConfig;
    std::vector<xml_file> m_files;
  };
}

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
  //p.push_files((path__ + std::string("run000823-fitter.root")).c_str(), 20, 823, 150);

  for (auto& e : xml_imput.fileList()){
    p.push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());
  }

  return 0;
}

using namespace std;
using namespace TCLAP;

int main(int argc, char **argv) {


  
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 

  try {

    CmdLine cmd("ProcessCollection", ' ', "0.1");
    ValueArg<std::string> FileNameArg("i", "inFile", "xml filename", true, "", "string");
    cmd.add(FileNameArg);
    ValueArg<std::string>  inPath("p", "inPath", "path to the root files", true, "", "string");
    cmd.add(inPath);
    ValueArg<std::string>  outpath("o", "outPath", "output path", false, ".", "string");
    cmd.add(outpath);
    cmd.parse(argc, argv);
    s_process_files p;
    ADDRun(p, FileNameArg.getValue(), inPath.getValue(), outpath.getValue());
    p.process();
  }
  catch (ArgException &e)  // catch any exceptions
  {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return -1;
  }



}


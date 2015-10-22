#ifndef xml_fileList_h__
#define xml_fileList_h__


#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
#include "xml_helpers/xml_util.hh"

namespace xmlImputFiles {



  using xml_n = rapidxml::xml_node < char > ;
  template<typename T>
  class MinMaxRange {
  public:
    MinMaxRange(T min_, T max_) :m_min(min_), m_max(max_),m_stepSize(1) {}
    MinMaxRange(T min_, T step_, T max_) :m_min(min_), m_max(max_), m_stepSize(step_) {}

    T getMin() const {
      return m_min;
    }
    T getMax() const {
      return m_max;
    }
    T getStep() const {
      return m_stepSize;
    }
  private:
    T m_min, m_max,m_stepSize;

  };
  using intRange = MinMaxRange < int > ;
  class xml_globalConfig {
  public:
    xml_globalConfig(xml_n* xIn) :m_node(xIn) {
      // std::cout << xIn->name << std::endl;
    }
    xml_globalConfig() {}
    std::string CollectionName() const {
      return xml_util::getAtribute(m_node->first_node("CollectionName"), "value", "notSet");
    }
    int NumberOfBins() const {
      return xml_util::getAtribute(m_node->first_node("NumberOfBins"), "value", -1);
    }
    int NumberOfStrips() const {
      return xml_util::getAtribute(m_node->first_node("NumberOfStrips"), "value", -1);
    }
    const S_Cut& cut() const {
      auto min_ = xml_util::getAtribute(m_node->first_node("YCut"), "min", -1);
      auto max_ = xml_util::getAtribute(m_node->first_node("YCut"), "max", -1);
      m_cut = S_YCut(min_, max_);
      return  m_cut;
    }
    int Device() const {
      return xml_util::getAtribute(m_node->first_node("Device"), "value", -1);
    }

    intRange AvtiveStrips() const {
      auto min_ = xml_util::getAtribute(m_node->first_node("AvtiveStrips"), "min", -1);
      auto max_ = xml_util::getAtribute(m_node->first_node("AvtiveStrips"), "max", -1);
      return intRange(min_, max_);
    }
    double Rotation()const {
      return xml_util::getAtribute(m_node->first_node("Rotation"), "value", 0.0);
    }
    std::string Position_name()const {
      return xml_util::getAtribute(m_node->first_node("Position"), "name", "notSet");
    }
    double Position_value()const {
      return xml_util::getAtribute(m_node->first_node("Position"), "value", -1.0);
    }
    double residual_cut() const {
      return xml_util::getAtribute(m_node->first_node("residual_cut"), "value", 1000);
    }
    std::string gearFile() const {
      return xml_util::getAtribute(m_node->first_node("gearFile"), "name", "notSet");
    }
  private:
    xml_n* m_node = nullptr;
    mutable  S_YCut m_cut = S_YCut(-10000000, 100000000000);
  };
  class xml_file {
  public:
    xml_file(xml_n* xIn) :
      m_node(xIn) {

    }
    static const char* NodeName() {
      return "file";
    }
    xml_file() {}
    std::string name() const {
      return xml_util::getAtribute(m_node->first_node("name"), "value", "notSet");
    }
    double threshold() const {
      return xml_util::getAtribute(m_node->first_node("threshold"), "value", -1.0);
    }
    double HV() const {
      return xml_util::getAtribute(m_node->first_node("HV"), "value", -1.0);
    }
    int runNumber() const {
      return xml_util::getAtribute(m_node->first_node("runNumber"), "value", -1);
    }
  private:
    xml_n* m_node = nullptr;
  };
  class XML_imput_file {
  public:
    XML_imput_file(const char* name) :m_file(name) {
      m_doc.parse<0>(m_file.data());    // 0 means default parse flags
      //      auto p = m_doc.next_sibling("");
      m_globalConfig = xml_globalConfig(m_doc.first_node("RunCollection")->first_node("globalConfig"));

      m_files = xml_util::getVectorOfT<xml_file>(m_doc.first_node("RunCollection")->first_node("fileList"));

    }

    const xml_globalConfig& globalConfig() const {
      return m_globalConfig;
    }
    const std::vector<xml_file>& fileList() const {
      return m_files;

    }


  private:
    rapidxml::xml_document<> m_doc;
    rapidxml::file<> m_file;
    xml_globalConfig m_globalConfig;
    std::vector<xml_file> m_files;
  };
}
#endif // xml_fileList_h__
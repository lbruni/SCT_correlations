#ifdef USE_LCIO
#include "internal/plotsBase.hh"
#include <iostream>



#include "lcio.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/TrackerRawDataImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "EUTELESCOPE.h"
#include "EUTelRunHeaderImpl.h"


#include "EUTelSetupDescription.h"
#include "EUTelTrackerDataInterfacerImpl.h"
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/TrackerDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "IMPL/LCGenericObjectImpl.h"
#  include "UTIL/CellIDEncoder.h"
#  include "lcio.h"
using namespace IMPL;
using namespace UTIL;
using lcio::TrackerDataImpl;
using eutelescope::EUTELESCOPE;

using eutelescope::EUTelTrackerDataInterfacerImpl;
using eutelescope::EUTelGenericSparsePixel;


class convert_to_LCIO :public plot{
public:
  convert_to_LCIO(const char* name, const char *file_name,unsigned runNum);
  virtual const char* getType() const;
  virtual bool isReady() ;
  virtual void pushAxis(axis_ref* axis);
  virtual void pushPlane(S_plane* plane_) ;
  virtual void fill() ;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") ;
  virtual Long64_t Draw(const S_DrawOption&) ;

  std::unique_ptr<lcio::LCRunHeader>  GetLCRunHeader();
  void GetLCIORunHeader(lcio::LCRunHeader & header, S_plane* plane_) const;

  std::unique_ptr<lcio::LCEvent > ConvertToLCIO();
  void ConvertLCIOSubEvent(lcio::LCEvent & dest, S_plane* source);
  void ConvertPlaneToLCIOGenericPixel(S_plane* source, lcio::TrackerDataImpl& zsFrame);
  virtual const char* getOutputName()  const;

  std::string m_filename;
  std::vector<S_plane*> m_planes;
  lcio::LCWriter *m_lcwriter =nullptr; /// The lcio writer
  bool m_fileopened =false; /// We have to keep track whether a file is open ourselves
  unsigned m_runNum = 0, m_event_nr=0;

  bool Collection_createIfNotExist(lcio::LCCollectionVec** zsDataCollection, const lcio::LCEvent & lcioEvent, const char * name);
};

convert_to_LCIO::convert_to_LCIO(const char* name, const char *file_name, unsigned runNum) :
plot(name, true), 
m_filename(file_name),
m_runNum(runNum)
{
  m_lcwriter=lcio::LCFactory::getInstance()->createLCWriter();
}

const char* convert_to_LCIO::getType() const
{
  return "conveter_to_LCIO";
}

bool convert_to_LCIO::isReady()
{
  m_event_nr = 0;
  if (m_fileopened) {
    m_lcwriter->close();
    m_fileopened = false;
  }

  // open a new file
  try {
    m_lcwriter->open(m_filename.c_str(),lcio::LCIO::WRITE_NEW);
      m_fileopened = true;
  }
  catch (const lcio::IOException & e) {
    std::cout << e.what() << std::endl;
    ///FIXME Error message to run control and logger
    return m_fileopened;
  }


  auto lcRunHeader = GetLCRunHeader();
  m_lcwriter->writeRunHeader(lcRunHeader.get());

  return m_fileopened;
}

void convert_to_LCIO::pushAxis(axis_ref* axis)
{
  std::cout << "[convert_to_LCIO] axis are not supported \n";
}

void convert_to_LCIO::pushPlane(S_plane* plane_)
{
  m_planes.push_back(plane_);
}

void convert_to_LCIO::fill()
{
  //  std::cout << "EUDAQ_DEBUG: FileWriterLCIO::WriteEvent() processing event "
  //    <<  devent.GetRunNumber () <<"." << devent.GetEventNumber () << std::endl;

  auto lcevent =ConvertToLCIO();

  // only write non-empty events
  //std::cout << lcevent->getDetectorName() << std::endl;
  if (!lcevent->getCollectionNames()->empty()) {
    //  std::cout << " FileWriterLCIO::WriteEvent() : doing the actual writing : " <<std::flush;
    m_lcwriter->writeEvent(lcevent.get());
    //  std::cout << " done" <<std::endl;
  }
}


Long64_t convert_to_LCIO::Draw(const S_DrawOption&)
{
  std::cout << "[convert_to_LCIO] drawing not supported \n";
  return -1;
}

Long64_t convert_to_LCIO::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  return Draw(S_DrawOption(options,cuts,axis));
}

std::unique_ptr<lcio::LCRunHeader> convert_to_LCIO::GetLCRunHeader()
{
  auto ret = std::unique_ptr<lcio::LCRunHeader>(new IMPL::LCRunHeaderImpl());
  auto lcHeader = dynamic_cast<IMPL::LCRunHeaderImpl*>(ret.get());
  lcHeader->setRunNumber(m_runNum);
  lcHeader->setDetectorName("EUTelescope");
  eutelescope::EUTelRunHeaderImpl runHeader(lcHeader);
  runHeader.setDateTime();
  runHeader.setDataType(EUTELESCOPE::DAQDATA);
  runHeader.setDAQSWName(EUTELESCOPE::EUDAQ);


  runHeader.setGeoID( 0);

  for (auto& e:m_planes) {
    
   GetLCIORunHeader(*lcHeader, e);
  }
  return ret;
}

void convert_to_LCIO::GetLCIORunHeader(lcio::LCRunHeader & header, S_plane* plane_) const
{

}

std::unique_ptr<lcio::LCEvent >  convert_to_LCIO::ConvertToLCIO()
{
  auto ret= std::unique_ptr<lcio::LCEvent > ( new lcio::LCEventImpl());
  auto lcEv = dynamic_cast<lcio::LCEventImpl*>(ret.get());
  lcEv->setEventNumber(m_event_nr++);
  lcEv->setRunNumber(m_runNum);
  lcEv->setTimeStamp(0);

  for (auto& e: m_planes) {
    ConvertLCIOSubEvent(*lcEv, e);
  }

  return ret;
}

void convert_to_LCIO::ConvertLCIOSubEvent(lcio::LCEvent & lcEv, S_plane* source)
{

  // If we get here it must be a data event

  
  lcEv.parameters().setValue(eutelescope::EUTELESCOPE::EVENTTYPE, eutelescope::kDE);


  IMPL::LCCollectionVec* zsDataCollection = nullptr;
  auto zsDataCollectionExists = Collection_createIfNotExist(&zsDataCollection, lcEv, source->getName());



  // set the proper cell encoder
  auto  zsDataEncoder = CellIDEncoder<TrackerDataImpl>(eutelescope::EUTELESCOPE::ZSDATADEFAULTENCODING, zsDataCollection);
  zsDataEncoder["sensorID"] = static_cast<int>(source->getID());
  zsDataEncoder["sparsePixelType"] = eutelescope::kEUTelGenericSparsePixel;


  // prepare a new TrackerData for the ZS data
  auto zsFrame = std::unique_ptr<lcio::TrackerDataImpl>(new lcio::TrackerDataImpl());
  zsDataEncoder.setCellID(zsFrame.get());


  ConvertPlaneToLCIOGenericPixel(source, *zsFrame);

  // perfect! Now add the TrackerData to the collection
  zsDataCollection->push_back(zsFrame.release());

  std::cout << zsDataCollection->size() << std::endl;
  if (!zsDataCollectionExists){
    if (zsDataCollection->size() != 0)
      lcEv.addCollection(zsDataCollection, source->getName());
    else
      delete zsDataCollection; // clean up if not storing the collection here
  }


}

void convert_to_LCIO::ConvertPlaneToLCIOGenericPixel(S_plane* source, lcio::TrackerDataImpl& zsFrame)
{
  // helper object to fill the TrakerDater object 
  auto sparseFrame = eutelescope::EUTelTrackerDataInterfacerImpl<eutelescope::EUTelGenericSparsePixel>(&zsFrame);

  while (source->next()){

    auto hit = source->get();
    eutelescope::EUTelGenericSparsePixel thisHit1(hit.x*100, hit.y*100, 1, 0);
    sparseFrame.addSparsePixel(&thisHit1);
  }

}

const char* convert_to_LCIO::getOutputName() const
{
  return m_filename.c_str();
}

bool convert_to_LCIO::Collection_createIfNotExist(lcio::LCCollectionVec** zsDataCollection, const lcio::LCEvent & lcioEvent, const char * name)
{
  bool zsDataCollectionExists = false;
  try
  {
    *zsDataCollection = static_cast<lcio::LCCollectionVec*> (lcioEvent.getCollection(name));
    zsDataCollectionExists = true;
  }
  catch (lcio::DataNotAvailableException& e)
  {
    *zsDataCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERDATA);
  }

  return zsDataCollectionExists;
}


S_plot sct_plot::s_save2LCIO(const char* name, const char* filename, unsigned runnum)
{
  return S_plot(new convert_to_LCIO(name, filename, runnum));

}
#else

S_plot sct_plot::s_save2LCIO(const char* name, const char* filename, unsigned runnum)
{
  return S_plot();
}

#endif
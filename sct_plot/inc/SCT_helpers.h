#ifndef SCT_helpers_h__
#define SCT_helpers_h__
#include "internal/platform.hh"
#include "Rtypes.h"
#include "S_Axis.h"
#include "TF1.h"
#include "plane_def.h"
#include "TProfile.h"
#include "sct_types.h"
class TH2D;
class TH2;
class TH1;
class TTree;
class S_Cut;
class S_DrawOption;

class DllExport SCT_helpers {
public:
  static void CutTH2(TH2* h2, const S_Cut& cut_);
  static void CutTH1(TH1* h1, const S_Cut& cut_);
  static TH1* HistogrammSilhouette(TH2* h2, axis_def ax);
  static Long64_t DrawTTree(TTree * tree, const S_DrawOption& opt);
  //static TF1 LinearFit_Of_Profile(TH2* h2, Double_t cut_prozent = 0);
  static TF1 LinearFit_Of_Profile(TH2* h2, const sct_type::procent_t& cut_prozent);
  static TH1* calc_efficiency(TH1* trueHits, TH1* dutHits);
  static void saveTH1_as_txt(const TH1& h1, const char* nameTXT);

  static TObject* Draw(const sct_corr::plane_def& plane_, const S_DrawOption& opt);
  template <typename returnType>
  static returnType* Draw(const sct_corr::plane_def& plane_, const S_DrawOption& opt) {
    return dynamic_cast<returnType*>(Draw(plane_, opt));
  }
  static void saveTH1_as_txt(const TProfile& h1, const char* nameTXT);
  
};



#endif // SCT_helpers_h__

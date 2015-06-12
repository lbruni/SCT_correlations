#ifdef __CINT__
#pragma link C++ enum axis_def;
#pragma link C++ class sct;
#pragma link C++ class S_plot_collection;
#pragma link C++ class S_DrawOption;
#pragma link C++ class S_Axis;

#pragma link C++ class S_plot;
#pragma link C++ class S_plane;
#pragma link C++ class S_treeCollection;
#pragma link C++ class S_plot_def;
#pragma link C++ class sct_plot;
#pragma link C++ class sct_coll;
#pragma link C++ class s_plane_collection;
#pragma link C++ class S_Cut;
#pragma link C++ class S_XCut;
#pragma link C++ class S_YCut;
#pragma link C++ class S_Cut_min_max;
#pragma link C++ class SCT_helpers;
#pragma link C++ class S_plane_def;
#pragma link C++ class S_Cut_BinContent;
#pragma link C++ class S_CutCoollection;


#pragma link C++ function  operator+(s_plane_collection , const s_plane_collection& );
#pragma link C++ function  operator+(s_plane_collection , const S_plane_def& );
#pragma link C++ function  operator+(const S_plane_def& , const S_plane_def& );

#pragma link C++ function  operator+(const S_Cut&, const S_Cut&);





#endif
#include "sct_processors.h"
#include "sct_plots.h"
#include "TMath.h"
#include "geometry/setup_description.hh"



S_plane_def sct_processor::cut_x_y(
  const S_plane_def& planeA,
  const S_Cut& cut_,
  const s_plot_prob& plot_prob/*= ""*/
  ) {
  auto pl = planeA.get_plot();

  S_plane_def ret = pl->addPlot(sct_plot::cut_x_y(cut_, plot_prob), planeA)();
  ret.set_s_plot_collection(pl);
  return ret;
}

S_plane_def sct_processor::cluster_strip(
  const S_plane_def& hits,
  axis_def search_axis,
  double cluster_distance,
  const s_plot_prob& plot_prob
  ) {
  auto pl = hits.get_plot();
  S_plane_def ret = pl->addPlot(sct_plot::clustering_strip(search_axis, cluster_distance, plot_prob), hits)();

  ret.set_s_plot_collection(pl);
  return ret;

}



S_plane_def sct_processor::coordinate_transform(
  const S_plane_def& planeA,
  Double_t x_slope, Double_t x_offset,
  Double_t y_slope, Double_t y_offset,
  const s_plot_prob& plot_prob
  ) {
  auto pl = planeA.get_plot();
  S_plane_def ret = pl->addPlot(
    sct_plot::coordinate_transform(
    x_slope, x_offset,
    y_slope, y_offset,
    plot_prob), planeA
    )();

  ret.set_s_plot_collection(pl);
  return ret;
}

S_plane_def sct_processor::coordinate_transform_move(
  const S_plane_def& planeA,
  Double_t x_offset, Double_t y_offset,
  const s_plot_prob& plot_prob__
  ) {
  auto pl = planeA.get_plot();
  S_plane_def ret = pl->addPlot(
    sct_plot::coordinate_transform_move(
    x_offset, y_offset,
    plot_prob__), planeA
    )();

  ret.set_s_plot_collection(pl);
  return ret;
}

s_plane_collection_find_closest sct_processor::find_nearest(
  const S_plane_def& planeA,
  const S_plane_def& planeB,
  Double_t x_cutoff,
  Double_t y_cutoff,
  const s_plot_prob& plot_prob__
  ) {
  auto plA = planeA.get_plot();
  auto plB = planeB.get_plot();

  if (plA.get() != plB.get()) {
    std::cout << "[s_plane_collection_find_closest sct_processor::find_nearest] referencing to different plot collection\n";
    return s_plane_collection_find_closest();
  }

  auto collection_ = plA->addPlot(sct_plot::find_nearest(x_cutoff, y_cutoff, plot_prob__), planeA, planeB);

  s_plane_collection_find_closest ret;
  ret.setResidual(collection_.getByType("nearest_distance")());
  ret.setHitOnPlaneA(collection_.getByType("nearest_plane1")());
  ret.setHitOnPlaneB(collection_.getByType("nearest_plane2")());
  ret.set_s_plot_collection(plA);
  return ret;
}

s_plane_collection_find_closest sct_processor::find_nearest_strip(
  const S_plane_def& planeA,
  const S_plane_def& planeB,
  axis_def search_axis,
  Double_t cutOfff,
  const s_plot_prob& plot_prob_ /*= "" */
  ) {
  auto plA = planeA.get_plot();
  auto plB = planeB.get_plot();

  if (plA.get() != plB.get()) {
    std::cout << "[s_plane_collection_find_closest sct_processor::find_nearest_strip] referencing to different plot collection\n";
    return s_plane_collection_find_closest();
  }

  auto collection_ = plA->addPlot(
    sct_plot::find_nearest_strip(
    search_axis,
    cutOfff,
    plot_prob_),
    planeA,
    planeB
    );

  s_plane_collection_find_closest ret;
  ret.setResidual(collection_.getByType("nearest_strip_distance")());
  ret.setHitOnPlaneA(collection_.getByType("nearest_strip_plane1")());
  ret.setHitOnPlaneB(collection_.getByType("nearest_strip_plane2")());
  ret.set_s_plot_collection(plA);
  return ret;
}

s_plane_collection_find_closest sct_processor::modulo_find_nearest_strip(
  const S_plane_def& planeA,
  const S_plane_def& planeB,
  axis_def search_axis,
  Double_t cutOfff,
  Double_t modulo_parameter,
  const s_plot_prob& plot_prob_ /*= "" */
  ) {

  auto plA = planeA.get_plot();
  auto plB = planeB.get_plot();

  if (plA.get() != plB.get()) {
    std::cout << "[sct_processor::modulo_find_nearest_strip] referencing to different plot collection\n";
    return s_plane_collection_find_closest();
  }

  auto collection_ = plA->addPlot(
    sct_plot::modulo_find_nearest_strip(search_axis, modulo_parameter, cutOfff, plot_prob_),
    planeA,
    planeB
    );

  s_plane_collection_find_closest ret;
  ret.setResidual(collection_.getByType("nearest_strip_distance")());
  ret.setHitOnPlaneA(collection_.getByType("nearest_strip_plane1")());
  ret.setHitOnPlaneB(collection_.getByType("nearest_strip_plane2")());
  ret.set_s_plot_collection(plA);
  return ret;
}


S_plane_def sct_processor::correlation(
  const S_Axis& axisA,
  const S_Axis& axisB,
  const s_plot_prob& plot_prob__/*= "" */
  ) {
  auto plA = axisA.get_plot();
  auto plB = axisB.get_plot();

  if (plA.get() != plB.get()) {
    std::cout << "[sct_processor::modulo_find_nearest_strip] referencing to different plot collection\n";
    return S_plane_def("error", 0);
  }

  auto ret = plA->addPlot(sct_plot::correlation(plot_prob__), axisA, axisB)();
  ret.set_s_plot_collection(plA);
  return ret;
}


S_plane_def sct_processor::correlation(
  const S_plane_def& planeA,
  const s_plot_prob& plot_prob__/*= "" */
  ) {
  auto plA = planeA.get_plot();
  auto ret = plA->addPlot(sct_plot::correlation(plot_prob__), planeA)();
  ret.set_s_plot_collection(plA);
  return ret;
}

S_plane_def sct_processor::rotate(
  const S_plane_def& planeA,
  Double_t angle,
  const s_plot_prob& plot_prob__/*= "" */
  ) {

  auto plA = planeA.get_plot();
  auto ret = plA->addPlot(sct_plot::rotated(angle, plot_prob__), planeA)();

  ret.set_s_plot_collection(plA);
  return ret;
}

S_plane_def sct_processor::linear_trans(
  const S_plane_def& planeA,
  Double_t a11, Double_t a21,
  Double_t a12, Double_t a22,
  const s_plot_prob& plot_prob__
  ) {
  auto plA = planeA.get_plot();
  auto ret = plA->addPlot(sct_plot::linear_trans(
    a11, a21,
    a12, a22,
    plot_prob__), planeA
    )();

  ret.set_s_plot_collection(plA);
  return ret;
}

S_plane_def sct_processor::hitmap(
  const S_plane_def& hits,
  const s_plot_prob& plot_prob__
  ) {
  auto plA = hits.get_plot();
  auto ret = plA->addPlot(sct_plot::hitmap(plot_prob__), hits)();
  ret.set_s_plot_collection(plA);
  return ret;

}

S_plane_def sct_processor::hitmap(
  const S_Axis& axisA,
  const S_Axis& axisB,
  const s_plot_prob& plot_prob__
  ) {
  auto plA = axisA.get_plot();
  auto plB = axisB.get_plot();

  if (plA.get() != plB.get()) {
    std::cout << "[sct_processor::hitmap] referencing to different plot collection\n";
    return S_plane_def("error", 0);
  }

  auto ret = plA->addPlot(sct_plot::hitmap(plot_prob__), axisA, axisB)();
  ret.set_s_plot_collection(plA);
  return ret;
}

S_plane_def sct_processor::moduloHitMap(
  const S_Axis& axisA,
  const S_Axis& axisB,
  double mod_x, double mod_y,
  const s_plot_prob& plot_prob
  ) {
  auto plA = axisA.get_plot();
  auto plB = axisB.get_plot();
  if (plA.get() != plB.get()) {
    std::cout << "[sct_processor::moduloHitMap] referencing to different plot collection\n";
    return S_plane_def("error", 0);
  }
  auto ret = plA->addPlot(sct_plot::moduloHitMap(mod_x, mod_y, plot_prob), axisA, axisB)();
  ret.set_s_plot_collection(plA);
  return ret;
}

S_plane_def sct_processor::moduloHitMap(
  const S_plane_def& hits,
  double mod_x,
  double mod_y,
  const s_plot_prob& plot_prob
  ) {
  auto plA = hits.get_plot();

  auto ret = plA->addPlot(sct_plot::moduloHitMap(mod_x, mod_y, plot_prob), hits)();
  ret.set_s_plot_collection(plA);
  return ret;
}

S_plane_def sct_processor::residual(
  const S_Axis& axisA,
  const S_Axis& axisB,
  const s_plot_prob& plot_prob
  ) {
  auto plA = axisA.get_plot();
  auto ret = plA->addPlot(sct_plot::residual(plot_prob), axisA, axisB)();
  ret.set_s_plot_collection(plA);
  return ret;
}


S_plane_def sct_processor::convert_local_to_global(
  const S_plane_def& local_hits, 
  const sct_corr::Xlayer& layer, 
  const s_plot_prob& plot_prob_
  ) {

  auto rot = rotate(
    local_hits,
    layer.ladder.rotationXY / 360 * TMath::Pi() * 2,
    s_plot_prob().doNotSaveToDisk()
    );
  auto a = linear_trans(
    rot,
    layer.sensitive.rotation1,
    layer.sensitive.rotation2,
    layer.sensitive.rotation3,
    layer.sensitive.rotation4,
    s_plot_prob().doNotSaveToDisk()
    );


  auto b = coordinate_transform_move(
    a,
    layer.ladder.positionX,
    layer.ladder.positionY,
    plot_prob_
    );

  return b;
}


S_plane_def sct_processor::convert_global_to_local(
  const S_plane_def& global_hits, 
  const sct_corr::Xlayer& layer, 
  const s_plot_prob& plot_prob_/*= ""*/
  ) {


  auto b = coordinate_transform_move(
    global_hits,
    -layer.ladder.positionX,
    -layer.ladder.positionY,
    s_plot_prob().doNotSaveToDisk()
    );

  auto det_A = layer.sensitive.rotation1*layer.sensitive.rotation4 
    - layer.sensitive.rotation2*layer.sensitive.rotation3;

  if (det_A == 0) {
    std::cout << "[sct_plot::convert_global_to_local] : Det_A ==0 " << std::endl;
    return S_plane_def("error", 0);
  }


  auto a = linear_trans(
    b,
    layer.sensitive.rotation4 / det_A,
    -layer.sensitive.rotation2 / det_A,
    -layer.sensitive.rotation3 / det_A,
    layer.sensitive.rotation1 / det_A,
    s_plot_prob().doNotSaveToDisk()
    );

  auto local_hits = rotate(
    a,
    -layer.ladder.rotationXY / 360 * TMath::Pi() * 2,
    plot_prob_
    );

  return local_hits;
}

S_plane_def sct_processor::convert_hits_to_zs_data_GBL(
  const S_plane_def& hits, 
  const sct_corr::Xlayer& layer, 
  const s_plot_prob& plot_prob_ /*= ""*/
  ) {

  return coordinate_transform(
    hits,
    1 / layer.sensitive.pitchX,                                                         // x_slope
    -(layer.sensitive.positionX - layer.sensitive.sizeX / 2) / layer.sensitive.pitchX,  //x_offset
    1 / layer.sensitive.pitchY,                                                         //y_slope
    -(layer.sensitive.positionY - layer.sensitive.sizeY / 2) / layer.sensitive.pitchY,  //y_offset
    plot_prob_
    );
}

S_plane_def sct_processor::convert_zs_data_to_hits_GBL(const S_plane_def& sz_data, const sct_corr::Xlayer& layer, const s_plot_prob& plot_prob_/*= ""*/) {
  return coordinate_transform(
    sz_data,
    layer.sensitive.pitchX,                                     // x_slope
    layer.sensitive.positionX - layer.sensitive.sizeX / 2,      //x_offset
    layer.sensitive.pitchY,                                     //y_slope
    layer.sensitive.positionY - layer.sensitive.sizeY / 2,      //y_offset
    plot_prob_
    );
}

S_plane_def sct_processor::convert_zs_data_to_hits_DAF(
  const S_plane_def& sz_data, 
  const sct_corr::Xlayer& layer, 
  const s_plot_prob& plot_prob_/*= ""*/
  ) {
  return coordinate_transform(
    sz_data,
    layer.sensitive.pitchX,          // x_slope
    -layer.sensitive.sizeX / 2,      //x_offset
    layer.sensitive.pitchY,          //y_slope
    -layer.sensitive.sizeY / 2,      //y_offset
    plot_prob_
    );
}

S_plane_def sct_processor::convert_hits_to_zs_data_DAF(
  const S_plane_def& hits, 
  const sct_corr::Xlayer& layer, 
  const s_plot_prob& plot_prob_
  ) {
  return coordinate_transform(
    hits,
    1 / layer.sensitive.pitchX,                            // x_slope
    (layer.sensitive.sizeX / 2) / layer.sensitive.pitchX,  //x_offset
    1 / layer.sensitive.pitchY,                             //y_slope
    (layer.sensitive.sizeY / 2) / layer.sensitive.pitchY,  //y_offset
    plot_prob_
    );
}

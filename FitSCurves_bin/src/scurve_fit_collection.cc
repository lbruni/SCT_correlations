#include "scurve_fit_collection.hh"
#include "SCT_helpers.h"
#include "s_DrawOption.h"
#include <algorithm>
#include "Math/Interpolator.h"

double sum(const std::vector<double> & vec) {
  double N_event = 0;
  for (const auto&e : vec) {
    N_event += e;
  }
  return N_event;
}

double interpolate(double xa, double xb, double ya, double yb, double x) {
  
  return ya + (yb - ya) / (xb - xa)*(x - xa);

}

scurve_fit_collection::scurve_fit_collection(const char* outputFileName) : m_out(outputFileName) {
  setStartValues();
}

void scurve_fit_collection::setTree(TTree* tree) {
  m_tree = tree;
  m_tree_sc_curve = std::make_shared<getScurvesFromTTree>(tree);
}

void scurve_fit_collection::setBeamRuns(TTree* noise) {
  m_use_total_efficiency_fit_as_start = true;
  m_start_amp = 1;
  if (!m_use_total_efficiency_fit_as_start) {
    m_start_mean = 110;
  }
  m_start_gaus_sigma = 52;
  m_start_landau_sigma = 10;
  m_gaus_sigma_lower_boundary = 20;
  m_mpv_start_low_cut = 280;
  m_threshold_Cut = "Threshold>=25";
  setThreshold_linear_trans(1, 0);
  setTree(noise);
  setStartValues();
}

void scurve_fit_collection::setNoiseRun(TTree* hits) {
  m_start_amp = 1;
  m_start_mean = 8;
  m_start_gaus_sigma = 12;
  m_start_landau_sigma = 1.2;
  m_gaus_sigma_lower_boundary = 1;
  m_use_total_efficiency_fit_as_start = false;
  m_mpv_start_low_cut = 10;
  m_threshold_Cut = "Threshold>=0";
  setThreshold_linear_trans(2.772, 12.39);
  setTree(hits);
  setStartValues();
}

void scurve_fit_collection::setStartValues() {
  f.setFitRange(0, 600);
  f.setLimits_Amplitude(0.7, 1.0);
  f.setStartAmplitude(m_start_amp);
  f.setStartGaussSigma(m_start_gaus_sigma);
  f.setStartLandauMean(m_start_mean);
  f.setStartLandauSigma(m_start_landau_sigma);
  f.setLimits_GaussSigma(m_gaus_sigma_lower_boundary, 1000);
}

void scurve_fit_collection::check() {
  if (f.getLandauSigma() == m_gaus_sigma_lower_boundary) {
    std::cout << "fit hit boundary \n";
  }
}

void scurve_fit_collection::push_to_file() {
  m_out << m_x_pos << ";  " << f.getAmplitude() << ";  " << f.getLandauMostProbable() << ";   " << f.getLandauSigma() << ";   " << f.getGaussSigma() << ";  " << f.getChiSqare() << std::endl;
}

void scurve_fit_collection::push_to_file_emtpyEvent() {
  m_out << m_x_pos << ";  " << 0 << ";  " << 0 << ";   " << 0 << ";   " << 0 << ";   " << 0 << std::endl;
}

void scurve_fit_collection::setStartMPV(double mpv) {
  m_start_mean = mpv;
  m_use_total_efficiency_fit_as_start = false;
}

void scurve_fit_collection::processTotal(const char * total_name) {
  TCanvas *canv = new TCanvas("canv", "", 500, 500);
  setStartValues();
  SCT_helpers::DrawTTree(
    m_tree,
    S_DrawOption()
    .output_object(&g)
    .draw_axis(total_name)
    .opt_bar()
    .cut(m_threshold_Cut.c_str())
    );

  f(&g);
  f.DrawfitFunction();
  push_to_file();
  if (m_use_total_efficiency_fit_as_start) {
    m_start_amp = f.getAmplitude();

    m_start_mean = f.getLandauMostProbable();
  }
  m_start_gaus_sigma = f.getGaussSigma();
  m_start_landau_sigma = f.getLandauSigma();
  canv->Close();
  delete canv;
}

double scurve_fit_collection::get_startPoint(const std::vector<double> & thr, const std::vector<double> & occ) {
  const double occ_begin = occ[0];
  for (size_t i = 0; i < occ.size(); ++i) {
    if (occ.at(i) < 0.5 * occ_begin) {
      return interpolate(occ.at(i - 1), occ.at(i), thr.at(i - 1), thr.at(i), 0.5 * occ_begin);
      
    }

  }
  return -1;
}
double scurve_fit_collection::get_startSigma(const std::vector<double> & thr, const std::vector<double> & occ)const {

  const double occ_begin = occ[0];
  double max_thr = 0, min_thr = 0;
  for (size_t i = 0; i < occ.size(); ++i) {
    if (occ.at(i) <(1 - 0.158650) * occ_begin) {
      min_thr = interpolate(occ.at(i - 1), occ.at(i), thr.at(i - 1), thr.at(i), (1 - 0.158650) * occ_begin);
      break;
    }

  }
  for (size_t i = 0; i < occ.size(); ++i) {
    if (occ.at(i) < 0.158650 * occ_begin) {
      
      max_thr = interpolate(occ.at(i - 1), occ.at(i), thr.at(i - 1), thr.at(i), 0.158650 * occ_begin);
      
      break;
    }

  }
  return max_thr - min_thr;
  
  

}
void scurve_fit_collection::processStrip(const char* stripName, axis_def axis__, double min__, double max__) {
 
  m_tree_sc_curve->set_pos(m_tree_sc_curve->size() - 1);
  const double max_x = m_tree_sc_curve->getX();
  double dec = 0;
  const auto max_index = m_tree_sc_curve->size();
  for (size_t i = 0; i < max_index; ++i) {
    m_out_data = output_data();
    m_tree_sc_curve->set_pos(i);
    if (m_tree_sc_curve->getX() > dec) {
      std::cout << "process channel: " << m_tree_sc_curve->getX() << " of " << max_x << std::endl;
      dec += 10;
    }
    auto thr = m_tree_sc_curve->get_Threshold();
    auto occ = m_tree_sc_curve->get_Ocuupancy();
    m_x_pos = m_tree_sc_curve->getX();
    double N_event = sum(*m_tree_sc_curve->get_total_hits());
    //   std::cout << N_event << std::endl;
    if (N_event < 1000) {
      push_to_file_emtpyEvent();
      continue;
    }
    auto d = std::max_element(occ->begin(), occ->end());
    if (*d < 0.5) {
      push_to_file_emtpyEvent();
      continue;
    }
    if (occ->empty()) {
      push_to_file_emtpyEvent();
      continue;
    }

    TGraph g(thr->size(), thr->data(), occ->data());


    const auto mpv_start = get_startPoint(*thr, *occ);
    if (mpv_start < m_mpv_start_low_cut) {
      push_to_file_emtpyEvent();
      continue;
    }
    f.setStartLandauMean(mpv_start);
    auto sigma=get_startSigma(*thr, *occ);
    f.setStartGaussSigma(sigma);

    f(&g);


    m_out_data.mpv_fit = f.getLandauMostProbable();
    m_out_data.max_efficiency = f.getErrorOfAmplitude();
    m_out_data.gauss_sigma = f.getGaussSigma();
    m_out_data.landau_sigma = f.getErrorOfLandauSigma();
    m_out_data.mpv_estimated = mpv_start;
    m_out_data.sigma_estimated = sigma;
    m_out_data.chiSquare = f.getChiSqare();
    m_out_data.x_pos = m_x_pos;
    

    //  f.printResults();
      f.DrawfitFunction();
      std::string cname = std::string("scurve") + std::to_string(i) + std::string(".png");
      m_c.SaveAs(cname.c_str());
      m_out_data.print(m_out);

  }
}

//#include "../include/ShapeLens.h"
//#include "../include/DEIMOSElliptical.h"
//#include "../include/DEIMOSCircular.h"
//#include "../include/DEIMOSForward.h"
//#include "../include/KSB.h"
//#include "../include/HOLICS.h"
//#include "../include/LensHelper.h"
//FCS MODIFIED 
#include <shapelens/ShapeLens.h>
#include <shapelens/DEIMOSElliptical.h>
#include <shapelens/DEIMOSCircular.h>
#include <shapelens/DEIMOSForward.h>
#include <shapelens/KSB.h>
#include <shapelens/HOLICS.h>
#include <shapelens/LensHelper.h>
#include <tclap/CmdLine.h>

using namespace shapelens;

data_t scale_list[] = {10, 8, 7, 6, 5, 4, 3.5, 3, 2.5, 2.5, 2.0};
//data_t scale_list[] = {4, 3.5, 3, 2.5, 2.5, 2.0};
//int L = 48; //FCS COMMENTED
//int N = 100;

int main(int argc, char* argv[]) {
  int L,N;//FCS ADDED
  TCLAP::CmdLine cmd("Measure ellipticity estimators for GREAT10 images", ' ', "0.4");
  TCLAP::SwitchArg circular("c","circular","Use DEIMOS with circular weight function", cmd, false);
  TCLAP::SwitchArg elliptical("e","elliptical","Use DEIMOS with elliptical weight function", cmd, false);
  TCLAP::SwitchArg forward("f","forward","Use DEIMOS with forward modelling", cmd, false);
  TCLAP::SwitchArg ksb("k","ksb","Use regular KSB", cmd, false);
  TCLAP::SwitchArg trace("t","trace","Use KSB with trace trick", cmd, false);
  TCLAP::SwitchArg ksb1("K","ksb1","Use first-order KSB", cmd, false);
  TCLAP::SwitchArg verbmode("V","verbose","Use verbose mode", cmd, false);
  TCLAP::SwitchArg trace1("T","trace1","Use first-order KSB with trace trick", cmd, false);
  TCLAP::ValueArg<int> patchsize("s","size_stamp","Length for the square postage stamp", false, 96,"int" ,cmd);//FCS ADDED
  TCLAP::ValueArg<int> gridsize("g","grid_size","Number of stamps in each of the 2 dimension", false, 100,"int" ,cmd);//FCS ADDED
  TCLAP::ValueArg<std::string> psffile("p","psf_file", "PSF FITS file", true, "", "string", cmd);
  TCLAP::UnlabeledValueArg<std::string> filename("filename","Object FITS file",true,"","string", cmd);
  cmd.parse(argc,argv);
  L=patchsize.getValue();//FCS ADDED
  N=gridsize.getValue();//FCS ADDED
  bool verbose=verbmode.getValue();

  if(verbose) std::cout<<"PATCH SIZE="<<L<<std::endl;
  if(verbose) std::cout<<"PATCH NB="<<N<<std::endl;
  // set up scales
  std::set<data_t> scales;
  int scale_len = sizeof(scale_list) / sizeof(data_t);
  for(int i = 0; i < scale_len; i++)
    scales.insert(scale_list[i]);
  
  // set up galaxy and star containers
  Object obj, psf;
  obj.resize(L*L);
  psf.resize(L*L);
  obj.grid.setSize(0,0,L,L);
  psf.grid.setSize(0,0,L,L);
  obj.noise_rms = 1;
  psf.noise_rms = 1;
  
  // open GREAT10 images
  if(verbose) std::cout<<"GET IMAGE "<<filename.getValue()<<std::endl;
  Image<float> im(filename.getValue());
  if(verbose) std::cout<<"GET STAR IMAGE "<<psffile.getValue()<<std::endl;
  Image<float> star_im(psffile.getValue());

  // iterate through all objects in both images
  int id = 1;
  Point<int> P1,P2;
  complex<data_t> gamma;
  for (int n2 = 0; n2 < N; n2++) {
    for (int n1 = 0; n1 < N; n1++) {
      obj.id = id;
      // get obj from its box in image
      P1(0) = n1*L;
      P1(1) = n2*L;
      P2(0) = (n1+1)*L;
      P2(1) = (n2+1)*L;
      im.slice(obj,P1,P2);
      star_im.slice(psf,P1,P2);
      
      // initialize centroid in center of image
      obj.centroid(0) = P2(0) - L/2;
      obj.centroid(1) = P2(1) - L/2;
      psf.centroid = obj.centroid;

      std::cout << id << "\t" << obj.centroid(0) << "\t" << obj.centroid(1);
      

      // DEIMOS estimators, can use the same PSF measurements
      if (circular.isSet() || elliptical.isSet() || forward.isSet()) {
	// measure PSF moments at all requested scales
	int C = 4; // weighting correction order
	DEIMOS::PSFMultiScale psfs;
	for (std::set<data_t>::iterator iter = scales.begin(); iter != scales.end(); iter++) {
	  DEIMOSElliptical p(psf, 2, C, *iter);
	  data_t flux = p.mo(0,0);
	  p.mo /= flux;
	  psfs.insert(*iter, p.mo);
	}
      
	if (circular.isSet()) {
	  DEIMOSCircular d(obj, psfs, 2, C);
	  gamma = d.epsilon();
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << d.size() << "\t" << d.scale;
	  std::cout << "\t" << d.SN[d.scale];
	}
	
	if (elliptical.isSet()) {
	  DEIMOSElliptical d(obj, psfs, 2, C);
	  gamma = d.epsilon();
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << d.size() << "\t" << d.matching_scale;
	  std::cout << "\t" << d.SN[d.matching_scale];
	}
	
	if (forward.isSet()) {
	  //std::vector<DEIMOS::PSFMultiScale> mepsf (1, psfs);
	  MultiExposureObject meo(1, obj);
	  //FCS COMMENT: DEIMOSForward no longer accepts this constructor
	  //DEIMOSForward d(meo, mepsf, 2, C);
	  MultiExposureObject mepsf(1, psf);//FCS ADDED
	  DEIMOSForward d(meo, mepsf, 2, C,scales);//FCS ADDED
	  gamma = d.epsilon();
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << d.size();
	  //std::cout << "\t" << d.SN;
	  std::cout << "\t" << d.SN[d.matching_scale];//FCS ADDED
	}
      } 

      if (ksb.isSet() || trace.isSet() || ksb1.isSet() || trace1.isSet()) {
	if(verbose) std::cout<<"Get parameters for galaxy"<<std::endl;
        KSB k(obj, scales);
	// take the scale of the final measurement for PSF moments
        if(verbose) std::cout<<"k.scale="<<k.scale<<std::endl;
	if(verbose) std::cout<<"Get parameters for psf"<<std::endl;
	KSB p(psf, k.scale);
	if(verbose) std::cout<<"Compute ellipticity"<<std::endl;
	
	if (ksb.isSet()) {
	  gamma = k.gamma(p);
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << k.scale << "\t" << k.SN;
	}
	if (trace.isSet()) {
	  gamma = k.gammaTr(p);
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << k.scale << "\t" << k.SN;
	}
	if (ksb1.isSet()) {
	  gamma = k.gamma1(p);
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << k.scale << "\t" << k.SN;
	}
	if (trace1.isSet()) {
	  gamma = k.gammaTr1(p);
	  std::cout << "\t" << real(gamma) << "\t" << imag(gamma);
	  std::cout << "\t" << k.scale << "\t" << k.SN;
	}
      }

      std::cout << std::endl;
      id++;
    }
  }
  return 0;
}


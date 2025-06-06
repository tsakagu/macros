// add up to slew calibrations
#include <mbd/MbdCalib.h>
#include <mbd/MbdGeomV1.h>
#include <mbd/MbdDefs.h>


#if defined(__CLING__)
R__LOAD_LIBRARY(libmbd.so)
#endif


const int NPOINTS = 16000; // number of points in correction LUT
const int MINADC = 0;       // subtracted adc
const int MAXADC = 15999;

void addslew(const char *calfile1 = "results/0/mbd_slewcorr_pass1.calib",
    const char *calfile2 = "results/0/mbd_slewcorr_pass2.calib")
{
  MbdGeom *mbdgeom = new MbdGeomV1();

  MbdCalib *mcal1 = new MbdCalib();
  mcal1->Download_SlewCorr( calfile1 );

  MbdCalib *mcal2 = new MbdCalib();
  mcal2->Download_SlewCorr( calfile2 );

  //cout << mcal1->get_scorr(0,100) << endl;
  //cout << mcal2->get_scorr(0,0) << endl;

  // Write out slew curves to temp calib file
  TString scorr_fname = "add_mbd_slewcorr.calib";
  cout << scorr_fname << endl;
  ofstream scorr_file( scorr_fname );
  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue;  // skip q-channels
    int pmtch = mbdgeom->get_pmt(ifeech);

    scorr_file << ifeech << "\t" << NPOINTS << "\t" << MINADC << "\t" << MAXADC << endl;
    int step = (MAXADC-MINADC)/(NPOINTS-1);
    //cout << "STEP " << step << endl;
    for (int iadc=MINADC; iadc<=MAXADC; iadc+=step)
    {
      float dt1 = mcal1->get_scorr(ifeech,iadc);
      float dt2 = mcal2->get_scorr(ifeech,iadc);
      float slewcorr = dt1 + dt2;
      scorr_file << slewcorr << " ";
      if ( iadc%10 == 9 ) scorr_file << endl;
    }
  }
  scorr_file.close();

}


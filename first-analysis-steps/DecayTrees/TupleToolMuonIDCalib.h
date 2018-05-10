#ifndef TUPLETOOLMUONIDCALIB_H
#define TUPLETOOLMUONIDCALIB_H 1

// Include files
#include "Kernel/IParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"

#include "Event/MuonCoord.h"
#include "Kernel/LHCbID.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/Particle2MCLinker.h"

/** @class TupleToolMuonIDCalib TupleToolMuonIDCalib.h
 *
 *  @author Fatima Soomro
 *  @date   2011-09-26
 */

using namespace LHCb;
using namespace Gaudi::Units;
using namespace Gaudi;

class DVAlgorithm;
struct ITrackExtrapolator;

class TupleToolMuonIDCalib : public TupleToolBase, virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolMuonIDCalib( const std::string& type,
                        const std::string& name,
                        const IInterface* parent );

  /// Loop over differnt conesizes and fill the variables into the tuple
  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  StatusCode fillVars( const LHCb::Particle *part, std::string, Tuples::Tuple& );
  void LoadMuonGeometry();
  StatusCode fillCoordVectors();
  double foiX(const int &station, const int &region, const double &p, const double &dx);
  double foiY(const int &station, const int &region, const double &p, const double &dy);
  StatusCode FindFieldPolarity();
  bool linFit(int &FromM1);
  bool calculatePt(int &FromM1);
  bool estrapola();

private:

  int             m_NFoi;         // look for hits winhin m_NFoi*Foi */
  std::string     m_particlesPath;

  /*   bool            m_DoHits;   // true: run the Hits code */
  /*   bool            m_DoMcHits;   // true: run the Hits code */
  /*   bool            m_DoSelectH;// true: store only closest hit */

  /*   double          m_MomentumCut;  // Minimum momentum cut */
  /*   double          m_IpsCut;       // Minimum momentum cut */
  /*   bool            m_DoAccCut;     // Do track within muon detector acc cut */
  /*   bool            m_DoHitCut;     // Do min hits in Nfoi/Nstation cut */
  /*   bool            m_DoMipCut;     // Do Mip cut          */

  /*   int             m_NHitsCut;     // minimum # of Hits within m_NFoi*FOI/station */
  /*                                   // if=-1 do the cut in NFoi as IsMuon  */
  /*   int             m_NStationsCut; // number of stations to require m_NHitsCut */
  /*                                   // if =-N require maximum N hits in M2 & in M3 */
  /*                                   // as well as IsMuon in nFoi */
  /*   double            m_EecalMax, m_EecalMin;  // Asymmetric Ecal Mip enregy window */
  /*   double            m_EhcalMax, m_EhcalMin;  // Asymmetric Hcal Mip enregy window */

  // FOIs parameters:
  // function that defines the field of interest size
  // formula is p(1) + p(2)*momentum+p(3)*exp(-p(4)*momentum)
  std::vector< double >     m_xfoiParam1;
  std::vector< double >     m_xfoiParam2;
  std::vector< double >     m_xfoiParam3;
  std::vector< double >     m_yfoiParam1;
  std::vector< double >     m_yfoiParam2;
  std::vector< double >     m_yfoiParam3;

  //--------------------------
  // Muon Detector variables:
  //--------------------------

  int m_NStation;       /// Number of stations
  int m_NRegion;        ///Number of regions
  //Names of the station
  std::vector<std::string> m_stationNames;
  // fill local arrays of pad sizes and region sizes
  DeMuonDetector*  m_mudet;

  // track state @ M1
  const LHCb::State *m_stateP;

  // These are indexed [station]
  std::vector<double> m_stationZ; // station position
  double m_trackX[5];   // position of track in x(mm) in each station
  double m_trackY[5];   // position of track in y(mm) in each station

  std::string m_extrapolatorName;
  ITrackExtrapolator* m_extrapolator; ///< extrapolator
  double m_TextraX[5];   // x(mm) position of extrapolated track in each station
  double m_TextraY[5];   // y(mm) position of extrapolated track in each station
  double m_ms2X[5];      // ms error on x(mm) position in each station
  double m_ms2Y[5];      // ms error on y(mm) position in each station
  double m_TextraReg[5]; // region crossed by the extrapolated track

  // closest hit in FOI[station] ==> input for linFit, the linear fit
  double m_smalldist_X[5];
  double m_smalldist_Y[5];
  double m_smalldist_Z[5];
  double m_smalldist_dX[5];
  double m_smalldist_dY[5];
  double m_smalldist_dZ[5];

  // linear fit results
  double m_sx, m_sy;
  double m_bx, m_by;
  double m_errbx, m_errsx, m_covbsx;
  double m_errby, m_errsy, m_covbsy;
  double m_chi2x, m_chi2y;

  // pt calculation
  std::vector<double> m_ParabolicCorrection;
  std::vector<double> m_resParams;
  double m_Constant;
  IBIntegrator* m_bIntegrator; // magnetic field tool
  double m_FieldPolarity;

  double m_4qOverP;
  double m_4sigmaQOverP2;
  double m_4pZM1;
  double m_4pXPvtx;
  double m_4pYPvtx;
  double m_4pZPvtx;

  double m_5qOverP;
  double m_5sigmaQOverP2;
  double m_5pZM1;
  double m_5pXPvtx;
  double m_5pYPvtx;
  double m_5pZPvtx;

  std::vector<float> m_hitInFOIx;
  std::vector<float> m_hitInFOIdx;
  std::vector<float> m_hitInFOIy;
  std::vector<float> m_hitInFOIdy;
  std::vector<float> m_hitInFOIz;
  std::vector<float> m_hitInFOIdz;
  std::vector<float> m_hitInFOIuncrossed;
  std::vector<float> m_hitInFOIID;
  std::vector<float> m_hitInFOITDC1;
  std::vector<float> m_hitInFOITDC2;
  std::vector<float> m_allMuonHitsX;
  std::vector<float> m_allMuonHitsDX;
  std::vector<float> m_allMuonHitsY;
  std::vector<float> m_allMuonHitsDY;
  std::vector<float> m_allMuonHitsZ;
  std::vector<float> m_allMuonHitsDZ;
  std::vector<float> m_allMuonHitsUncrossed;
  std::vector<float> m_allMuonHitsTDC1;
  std::vector<float> m_allMuonHitsTDC2;
  std::vector<float> m_allMuonHitsID;
  bool m_doVerbose;
  bool m_estimate;

  // local array of region sizes
  std::vector<double> m_regionInnerX; // inner edge in abs(x)
  std::vector<double> m_regionOuterX; // outer edge in abs(x)
  std::vector<double> m_regionInnerY; // inner edge in abs(y)
  std::vector<double> m_regionOuterY; // outer edge in abs(y)

  // Store x,dx,y,dy of each coord
  class coordExtent_{
  public:
    coordExtent_(double x, double dx, double y, double dy,  double z, double dz, bool uncrossed, int digitTDC1, int digitTDC2,
                 LHCb::MuonCoord *pCoord) :
      m_x(x), m_dx(dx), m_y(y), m_dy(dy), m_z(z), m_dz(dz), m_uncrossed(uncrossed), m_digitTDC1(digitTDC1), m_digitTDC2(digitTDC2), m_pCoord(pCoord)  {};
    double m_x;
    double m_dx;
    double m_y;
    double m_dy;
    double m_z;
    double m_dz;
    bool m_uncrossed;
    int m_digitTDC1;
    int m_digitTDC2;
    LHCb::MuonCoord *m_pCoord;
  };
  // vector of positions of coords (innner vector coords,
  // outer is [station* m_NRegion + region ]
  std::vector<std::vector<coordExtent_> > m_coordPos;


};

#endif // TUPLETOOLMUONIDCALIB_H

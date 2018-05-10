#include <Kernel/IDVAlgorithm.h>
#include <Kernel/GetIDVAlgorithm.h>
// local
#include "TupleToolVeloTrackMatch.h"

using namespace LHCb ;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolVeloTrackMatch
//
// 2013-11-21 : Andrea Contu
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolVeloTrackMatch )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
    TupleToolVeloTrackMatch::TupleToolVeloTrackMatch( const std::string& type,
    const std::string& name,
    const IInterface* parent )
  : TupleToolBase ( type, name, parent )
    , m_dva(0)
    , m_slopediff(0)
    , m_momentumcone(false)
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("ConeAngle", m_slopediff = 0.05);
  declareProperty("ConeAroundMomentum", m_momentumcone = false);
  declareProperty("VerboseInfo", m_verbose = false);
  declareProperty("MaxIPwrtBestPV", m_maxVbestPVdist=0.5);
}

//=============================================================================
// Initialize
//=============================================================================
StatusCode TupleToolVeloTrackMatch::initialize(){
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  if (!m_dva) return Error("Couldn't get parent DVAlgorithm",
  StatusCode::FAILURE);

  return sc;
}



//=============================================================================
// Fill
//=============================================================================
StatusCode TupleToolVeloTrackMatch::fill( const LHCb::Particle * /* top */,
                                          const LHCb::Particle *part,
                                          const std::string &   head,
                                          Tuples::Tuple &   tuple   ){

  const std::string prefix=fullName(head);
  StatusCode sc=StatusCode::SUCCESS;

  const unsigned int maxVeloTracks = 2000 ;

  std::vector<Gaudi::XYZPoint> vfirstpoints;
  std::vector<Gaudi::XYZPoint> vslopes;
  std::vector<double> vslopesx,vslopesy;
  std::vector<double> vslopesxxerr,vslopesyyerr,vslopesxyerr;
  std::vector<double> vslopesxratio,vslopesyratio,vR;
  std::vector<double> vfpx,vfpy,vfpz;
  std::vector<double> vfpxxerr,vfpyyerr,vfpxyerr;
  std::vector<double> vconedist;
  std::vector<double> vbestip;

  double ipcut=m_maxVbestPVdist;

  double bestdt=9999.;
  double bestip=9999.;
  double nvelotracks=-1.;
  double bestsloperatiox=9999.;
  double bestsloperatioy=9999.;
  double bestconedist=9999.;
  unsigned int besttrackindex=-1;
  Gaudi::XYZPoint bestfposition;
  Gaudi::SymMatrix3x3 bestfpositionerr;
  Gaudi::XYZPoint bestslopes;
  Gaudi::SymMatrix3x3 bestslopeserr;

  if(!part->isBasicParticle() && part->endVertex()){

    Gaudi::LorentzVector lorvectmother=part->momentum();

    bool conemom=m_momentumcone;
    const VertexBase* aPV = NULL;

    if(!m_momentumcone) {
      aPV = m_dva->bestVertex (part);
      if(!aPV){
        conemom=true;
        ipcut=9999.;
        Warning("Could not find best PV, falling back to momentum cone!");
      }
    }
    else{
      conemom=true;
    }

    double mpx=9999.;
    double mpy=9999.;
    double mpz=9999.;

    if(!conemom){
      mpx=aPV->position().x()-part->endVertex()->position().x();
      mpy=aPV->position().y()-part->endVertex()->position().y();
      mpz=aPV->position().z()-part->endVertex()->position().z();
    }

    else{
      mpx=lorvectmother.Px();
      mpy=lorvectmother.Py();
      mpz=lorvectmother.Pz();
    }
    double msx=mpx/mpz;
    double msy=mpy/mpz;

    LHCb::Track::Range vtrax=get<LHCb::Track::Range>(LHCb::TrackLocation::Velo);

    if(vtrax.size()==0){
      return Warning("Could not find any VELO track, you may need to re-run the VELO tracking",sc);
    }

    for(LHCb::Track::Range::const_iterator tri=vtrax.begin(); tri!= vtrax.end(); ++tri){
      const Track *tmptrack=(*tri);
      if(tmptrack->type()==LHCb::Track::Types::Velo){
        Gaudi::XYZPoint tmpfp;
        Gaudi::SymMatrix3x3 fperror;
        tmptrack->position(tmpfp,fperror);

        Gaudi::XYZVector tmpslope;
        Gaudi::SymMatrix3x3 slopeerror;
        tmptrack->slopes(tmpslope,slopeerror);
        double dtx = msx - tmpslope.X() ;
        double dty = msy - tmpslope.Y() ;
        double dt  = std::sqrt(dtx*dtx+dty*dty) ;
        double tmpR3d=sqrt(pow((1-tmpslope.X()/msx),2)+pow((1-tmpslope.Y()/msy),2));

        double ipbestpv=-1.;
        double tmpvect2=0.;
        double tmpscalar2=0.;

        if(!conemom){
          tmpvect2= pow((tmpfp.X()-aPV->position().x()),2) + pow((tmpfp.Y()-aPV->position().y()),2) + pow((tmpfp.Z()-aPV->position().z()),2);
          tmpscalar2=pow( ((tmpfp.X()-aPV->position().x())*tmpslope.X() + (tmpfp.Y()-aPV->position().y())*tmpslope.Y() + (tmpfp.Z()-aPV->position().z())),2)/(tmpslope.X()*tmpslope.X()+tmpslope.Y()*tmpslope.Y()+tmpslope.Z()*tmpslope.Z());
          ipbestpv=sqrt(tmpvect2-tmpscalar2);
        }

        if(dt<m_slopediff && ipbestpv<ipcut){
          if(tmpR3d<bestdt ) {
            bestdt = tmpR3d ;
            bestip = ipbestpv ;
            besttrackindex=nvelotracks;
            bestfposition=tmpfp;
            bestfpositionerr=fperror;
            bestslopes=tmpslope;
            bestslopeserr=slopeerror;
            bestsloperatiox=tmpslope.X()/msx;
            bestsloperatioy=tmpslope.Y()/msy;
            bestconedist=dt;
          }
          ++nvelotracks;
          vfpx.push_back(tmpfp.X());
          vfpy.push_back(tmpfp.Y());
          vfpz.push_back(tmpfp.Z());
          vslopesx.push_back(tmpslope.X());
          vslopesy.push_back(tmpslope.Y());
          vslopesxxerr.push_back(slopeerror(0,0));
          vslopesyyerr.push_back(slopeerror(1,1));
          vslopesxyerr.push_back(slopeerror(0,1));
          vfpxxerr.push_back(fperror(0,0));
          vfpyyerr.push_back(fperror(1,1));
          vfpxyerr.push_back(fperror(0,1));
          vslopesxratio.push_back(tmpslope.X()/msx);
          vslopesyratio.push_back(tmpslope.Y()/msy);
          vR.push_back(tmpR3d);
          vbestip.push_back(ipbestpv);
          vconedist.push_back(dt);
        }
      }
    }

    tuple->column(prefix+"_VM_cand_slope_X",msx);
    tuple->column(prefix+"_VM_cand_slope_Y",msy);
    tuple->column(prefix+"_VM_nvelotracks",nvelotracks);
    tuple->column(prefix+"_VM_R",bestdt);
    tuple->column(prefix+"_VM_IP",bestip);
    tuple->column(prefix+"_VM_CD",bestconedist);
    tuple->column(prefix+"_VM_index",besttrackindex);
    tuple->column(prefix+"_VM_fpos_X",bestfposition.X());
    tuple->column(prefix+"_VM_fpos_Y",bestfposition.Y());
    tuple->column(prefix+"_VM_fpos_Z",bestfposition.Z());
    tuple->column(prefix+"_VM_fpos_err_XX",bestfpositionerr(0,0));
    tuple->column(prefix+"_VM_fpos_err_YY",bestfpositionerr(1,1));
    tuple->column(prefix+"_VM_fpos_err_XY",bestfpositionerr(0,1));
    tuple->column(prefix+"_VM_sloperatio_X",bestsloperatiox);
    tuple->column(prefix+"_VM_sloperatio_Y",bestsloperatioy);
    tuple->column(prefix+"_VM_slope_X",bestslopes.X());
    tuple->column(prefix+"_VM_slope_Y",bestslopes.Y());
    tuple->column(prefix+"_VM_slope_err_XX",bestslopeserr(0,0));
    tuple->column(prefix+"_VM_slope_err_YY",bestslopeserr(1,1));
    tuple->column(prefix+"_VM_slope_err_XY",bestslopeserr(0,1));

    if(m_verbose){
      tuple->farray( prefix+"_VM_array_fpos_X", vfpx ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_fpos_Y", vfpy ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_fpos_Z", vfpz ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_fpos_err_XX", vfpxxerr ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_fpos_err_YY", vfpyyerr ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_fpos_err_XY", vfpxyerr ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_slope_X", vslopesx ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_slope_Y", vslopesy ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_slope_err_XX", vslopesxxerr ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_slope_err_YY", vslopesyyerr ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_slope_err_XY", vslopesxyerr ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_sloperatio_X", vslopesxratio ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_sloperatio_Y", vslopesyratio ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_R", vR ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_IP", vbestip ,prefix+"_VM_nTP",maxVeloTracks );
      tuple->farray( prefix+"_VM_array_CD", vconedist ,prefix+"_VM_nTP",maxVeloTracks );
    }
  }
  else{
    return Warning("Particle "+prefix+" is not composite or does not have an endvertex!",sc);
  }
  return sc;
}
//=============================================================================
// Destructor
//=============================================================================
TupleToolVeloTrackMatch::~TupleToolVeloTrackMatch() {}

//=============================================================================

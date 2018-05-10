// Include files

// local
#include "TupleToolL0Calo.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Kernel/IParticleTupleTool.h"
#include "Event/Particle.h"
#include "CaloInterfaces/IPart2Calo.h"
#include "Event/L0CaloCandidate.h"
#include "Event/L0DUBase.h"
#include "Event/L0CaloAdc.h"
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h"

#include "TMath.h"
#include "boost/foreach.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : L0CaloTupleTool
//
// 2012-10-11 : Alexandra Martin Sanchez
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// parameters
namespace
{
  // HCAL constants
  const double HCAL_CellSize_Inner = 131.3;
  //const double HCAL_CellSize_Outer = 262.6;
  const double HCAL_xMax_Inner = 2101;
  const double HCAL_yMax_Inner = 1838;
  const double HCAL_xMax_Outer = 4202;
  const double HCAL_yMax_Outer = 3414;

  // ECAL constants
  const double ECAL_CellSize_Inner = 40.4;
  //const double ECAL_CellSize_Middle = 60.6;
  //const double ECAL_CellSize_Outer = 121.2;
  const double ECAL_xMax_Inner = 970;
  const double ECAL_yMax_Inner = 725;
  const double ECAL_xMax_Middle = 1940;
  const double ECAL_yMax_Middle = 1210;
  const double ECAL_xMax_Outer = 3880;
  const double ECAL_yMax_Outer = 3150;
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolL0Calo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolL0Calo::TupleToolL0Calo( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
: TupleToolBase ( type, name , parent ) , m_fillTriggerEt( false ) ,
  m_caloDe( 0 ) , m_adcsHcal(NULL)

{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("WhichCalo", m_calo = "HCAL");
  declareProperty("TriggerClusterLocation" , m_location = "" ) ;
}

//=============================================================================

StatusCode TupleToolL0Calo::initialize( )
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  m_part2calo = tool<IPart2Calo>("Part2Calo","Part2Calo",this);

  // Check selected calo is valid
  if (m_calo != "HCAL" && m_calo != "ECAL") {
    err() << "TupleToolL0Calo -- Invalid calo: " << m_calo << "." << endmsg ;
    return StatusCode::FAILURE;
  }

  if ( m_calo == "HCAL" ) {
    m_caloDe = getDet< DeCalorimeter >( DeCalorimeterLocation::Hcal ) ;
    m_adcsHcal = tool<ICaloTriggerAdcsFromRaw>( "CaloTriggerAdcsFromRaw",
                                                "HcalTriggerAdcTool" );
  } else
    m_caloDe = getDet< DeCalorimeter >( DeCalorimeterLocation::Ecal ) ;

  // Fill trigger info ?
  if ( "" != m_location ) m_fillTriggerEt = true ;

  return sc;
}

//=============================================================================
StatusCode TupleToolL0Calo::fill( const LHCb::Particle* /* mother */,
                                  const LHCb::Particle* P,
                                  const std::string& head,
                                  Tuples::Tuple& tuple )
{

  bool test = true;
  if( !(P->isBasicParticle()) ) return StatusCode(test);

  const std::string prefix = fullName(head);

  double trackET, xProjection, yProjection;

  // Project the track on the selected calo
  if (m_calo == "HCAL"){
    m_part2calo->match( P, "/dd/Structure/LHCb/DownstreamRegion/Hcal" ); }
  else if (m_calo == "ECAL") {
    m_part2calo->match( P, "/dd/Structure/LHCb/DownstreamRegion/Ecal" ); }

  // Calculate a few observables
  trackET = TMath::Sqrt(TMath::Power(m_part2calo->caloState().x(),2)+
                        TMath::Power(m_part2calo->caloState().y(),2))
    /TMath::Sqrt(TMath::Power(m_part2calo->caloState().x(),2)+
                 TMath::Power(m_part2calo->caloState().y(),2)+
                 TMath::Power(m_part2calo->caloState().z(),2))*
    TMath::Sqrt(TMath::Power(m_part2calo->caloState().p(),2)+
                TMath::Power(P->measuredMass(),2));

  xProjection = m_part2calo->caloState().x();
  yProjection = m_part2calo->caloState().y();

  double triggerET( -1. ) , triggerHCALET( -1. ) , xtrigger( 0. ) , ytrigger( 0. ) ;

  if ( m_fillTriggerEt ) {
    triggerET = getAssociatedCluster( triggerHCALET , xtrigger , ytrigger ) ;
  }

  // Fill the tuple
  if (m_calo == "HCAL") {
    test &= tuple->column( prefix+"_L0Calo_HCAL_realET", trackET );
    test &= tuple->column( prefix+"_L0Calo_HCAL_xProjection", xProjection );
    test &= tuple->column( prefix+"_L0Calo_HCAL_yProjection", yProjection );
    test &= tuple->column( prefix+"_L0Calo_HCAL_region",
                           isinside_HCAL( xProjection , yProjection ) );
    test &= tuple->column( prefix+"_L0Calo_HCAL_TriggerET", triggerET ) ;
    test &= tuple->column( prefix+"_L0Calo_HCAL_TriggerHCALET", triggerHCALET ) ;
    test &= tuple->column( prefix+"_L0Calo_HCAL_xTrigger", xtrigger ) ;
    test &= tuple->column( prefix+"_L0Calo_HCAL_yTrigger", ytrigger ) ;
  }
  else if (m_calo == "ECAL") {
    test &= tuple->column( prefix+"_L0Calo_ECAL_realET", trackET );
    test &= tuple->column( prefix+"_L0Calo_ECAL_xProjection", xProjection );
    test &= tuple->column( prefix+"_L0Calo_ECAL_yProjection", yProjection );
    test &= tuple->column( prefix+"_L0Calo_ECAL_region",
                           isinside_ECAL( xProjection , yProjection ) );
    test &= tuple->column( prefix+"_L0Calo_ECAL_TriggerET", triggerET ) ;
    test &= tuple->column( prefix+"_L0Calo_ECAL_xTrigger", xtrigger ) ;
    test &= tuple->column( prefix+"_L0Calo_ECAL_yTrigger", ytrigger ) ;
  }


  if( msgLevel( MSG::VERBOSE ) ) verbose() << "Returns " << test << endmsg;
  return StatusCode(test);

}


int TupleToolL0Calo::isinside_HCAL(double x,
                                   double y)
{
  bool inside, inner, outer;
  inside = true;
  inner = false;
  outer = false;

  // projection inside calo
  if (TMath::Abs(x) < HCAL_xMax_Outer && TMath::Abs(y) < HCAL_yMax_Outer) {
    // projection inside inner calo (else is outer calo)
    if (TMath::Abs(x) < HCAL_xMax_Inner && TMath::Abs(y) < HCAL_yMax_Inner) {
      // projections outside the beampipe (in x)
      if (TMath::Abs(x) > 2*HCAL_CellSize_Inner)
        inner = true;
      // projections outside the beampipe (not in x, so in y)
      else if (TMath::Abs(y) > 2*HCAL_CellSize_Inner)
        inner = true;
      else
        inside = false;
    }
    else
      outer = true;
  }
  else
    inside = false;

  if (!inside)
    return -1;
  else if (inner)
    return 1;
  else if (outer)
    return 0;
  else
    return -999;

}

int TupleToolL0Calo::isinside_ECAL(double x,
                                   double y)
{
  bool inside, inner, middle, outer;
  inside = true;
  inner = false;
  middle = false;
  outer = false;

  // projection inside calo
  if (TMath::Abs(x) < ECAL_xMax_Outer && TMath::Abs(y) < ECAL_yMax_Outer) {
    // projection inside middle calo (else is outer calo)
    if (TMath::Abs(x) < ECAL_xMax_Middle && TMath::Abs(y) < ECAL_yMax_Middle) {
      // projection inside inner calo
      if (TMath::Abs(x) < ECAL_xMax_Inner && TMath::Abs(y) < ECAL_yMax_Inner) {
        // projections outside the beampipe (in x)
        if (TMath::Abs(x) > 2*ECAL_CellSize_Inner)
          inner = true;
        // projections outside the beampipe (not in x, so in y)
        else if (TMath::Abs(y) > 2*ECAL_CellSize_Inner)
          inner = true;
        else
          inside = false;
      }
      else
        middle = true;
    }
    else
      outer = true;
  }
  else
    inside = false;

  if (!inside)
    return -1;
  else if (inner)
    return 2;
  else if (middle)
    return 1;
  else if (outer)
    return 0;
  else
    return -999;
}

//=============================================================================
// Get associated L0 or LLT cluster
//=============================================================================
double TupleToolL0Calo::getAssociatedCluster( double & hcal_energy , double & xTrigger ,
                                              double & yTrigger ) {
  // First get the CALO cells in the 3x3 cluster around the track projection
  std::vector< LHCb::CaloCellID > cells3x3;

  if( m_part2calo -> isValid() ) {
    const LHCb::CaloCellID centerCell = m_part2calo -> caloCellID();
    cells3x3.push_back( centerCell ) ;
    BOOST_FOREACH( LHCb::CaloCellID cell, m_caloDe -> neighborCells( centerCell ) )
    {
      cells3x3.push_back( cell );
    } ;
  }
  std::sort( cells3x3.begin() , cells3x3.end() );

  // loop over the L0 candidates
  LHCb::L0CaloCandidates * candidates =
    getIfExists<LHCb::L0CaloCandidates> ( m_location );

  int typeToCheck = L0DUBase::CaloType::Electron ;
  if ( m_calo == "HCAL" ) typeToCheck = L0DUBase::CaloType::Hadron ;

  LHCb::L0CaloCandidates::iterator cand ;
  double result = -1. ;

  for ( cand = candidates -> begin() ; candidates -> end() != cand ;
        ++cand ) {
    LHCb::L0CaloCandidate * theCand = (*cand) ;
    if ( theCand -> type() == typeToCheck ) {
      LHCb::CaloCellID cell1 , cell2 , cell3 , cell4 ;
      cell1 = theCand -> id() ;
      cell2 = LHCb::CaloCellID( cell1.calo() , cell1.area() ,
                                cell1.row()+1 , cell1.col() ) ;
      cell3 = LHCb::CaloCellID( cell1.calo() , cell1.area() ,
                                cell1.row()  , cell1.col()+1 ) ;
      cell4 = LHCb::CaloCellID( cell1.calo() , cell1.area() ,
                                cell1.row()+1  , cell1.col()+1 ) ;
      if ( std::binary_search( cells3x3.begin() , cells3x3.end() , cell1 ) ||
           std::binary_search( cells3x3.begin() , cells3x3.end() , cell2 ) ||
           std::binary_search( cells3x3.begin() , cells3x3.end() , cell3 ) ||
           std::binary_search( cells3x3.begin() , cells3x3.end() , cell4 ) )
      {
        if ( theCand -> et() > result ) {
          xTrigger = m_caloDe -> cellX( cell1 ) ;
          yTrigger = m_caloDe -> cellY( cell1 ) ;
          result = theCand -> et() ;
          // Compute the HCAL energy of this cluster
          if ( m_calo == "HCAL" ) {
            hcal_energy = 0. ;
            const std::vector< LHCb::L0CaloAdc >& adcs = m_adcsHcal -> adcs() ;
            for ( std::vector< LHCb::L0CaloAdc >::const_iterator itAdc = adcs.begin() ;
                  adcs.end() != itAdc ; ++itAdc )
            {
              LHCb::CaloCellID id = (*itAdc).cellID() ;
              if ( ( id == cell1 ) || (id == cell2 ) || ( id == cell3 ) || ( id == cell4 ) )
                hcal_energy += (*itAdc).adc() ;
            }
            if ( hcal_energy > 255 ) hcal_energy = 255 ;
            hcal_energy = hcal_energy * ( m_caloDe -> L0EtGain() ) ;
          }
        }
      }
    }
  }

  return result ;
}

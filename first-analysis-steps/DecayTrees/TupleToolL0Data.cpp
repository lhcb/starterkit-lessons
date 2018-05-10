#include "TupleToolL0Data.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
//#include "Event/L0ProcessorData.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolL0Data
//
//  - produce tuple branches for L0DU input data extracted from the L0DU bank as defined in EDMS-845277
//   (see $L0EVENTROOT/EVENT/L0DUBase.h for data naming)
//  - produce the emulated L0 decision for the TCK list given in option
//
//  - Properties :
//       * 'TCKList'  : list of registered TCK's for which the emulated decision is requested (vector<string>)
//       * 'DataList' : list of L0data to be tupled (vector<string>)
//                    -> wildcard are allowed
//                    -> e.g.   ["*Et*","*Pt*"] to tuple the L0 data with name containing 'Et' or 'Pt'
//                    -> e.g.   ["ALL","!*Status*"] to tuple ALL L0 data except those with name containing 'Status'
//
//                    -> Default : ALL L0 data used for the standard decision making
//                               -> i.e. 'technical' data related to candidate address, status or intermediate value are rejected
//
//
//       * 'ScaledData' : scale the candidate pT/eT data in MeV  (boolean - default = false, i.e. no scaling)
//                     -> warning : the scaling factor relies on condDB for L0Calo and is partially hardcoded in L0DU s/w for L0Muon.
//                     -> the scaling has changed in July 2015.
//
//
// 2011-06-08 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolL0Data )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolL0Data::TupleToolL0Data( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent ):
    TupleToolBase ( type, name , parent )
{
  declareInterface<IEventTupleTool>(this);

  declareProperty( "ScaledData",m_scale=false);
  declareProperty( "TCKList" , m_tcks );
  // Accepted/rejected data
  m_list.push_back("ALL");      // add all data but ...
  m_list.push_back("!*Status*");
  m_list.push_back("!*More*");
  m_list.push_back("!*CU*");
  m_list.push_back("!*M0*");
  m_list.push_back("!*M1*");
  m_list.push_back("!*M2*");
  m_list.push_back("!*M3*");
  m_list.push_back("!*M4*");
  m_list.push_back("!*M5*");
  m_list.push_back("!*M6*");
  m_list.push_back("!*M7*");
  m_list.push_back("!*Add*");
  declareProperty( "DataList", m_list );
}

StatusCode TupleToolL0Data::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  m_l0    = tool<IL0DUFromRawTool>("L0DUFromRawTool","L0DUFromRaw" ,this );
  m_l0emu = tool<IL0DUEmulatorTool>("L0DUEmulatorTool","L0DUEmulator",this);
  m_l0conf= tool<IL0DUConfigProvider>( "L0DUMultiConfigProvider","L0DUConfig");
  return sc;
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolL0Data::~TupleToolL0Data() {}
//=============================================================================

StatusCode TupleToolL0Data::fill( Tuples::Tuple& tuple)
{
  const std::string prefix=fullName();
  if(!m_l0->decodeBank())Warning("Unable to decode L0DU rawBank", StatusCode::SUCCESS).ignore();
  const std::map<std::string, std::pair<unsigned int,double> >&  datamap = m_l0->datas();
  for(std::map<std::string, std::pair<unsigned int,double> >::const_iterator it = datamap.begin();
      datamap.end() != it ; ++it)
  {
    const std::string& name = it->first;
    const int& val      = (it->second).first;
    const double& scale = (it->second).second;
    if ( LHCb::CaloAlgUtils::StringMatcher(m_list,name) )
    {
      if(m_scale)tuple->column( prefix+"L0Data_"+rename(name), double(val)*scale );
      else tuple->column( prefix+"L0Data_"+rename(name), val );
      if ( msgLevel(MSG::DEBUG) )
        debug() << "++ ACCEPT L0Data : '" << prefix<<"L0Data_"<<rename(name) <<endmsg;
    }
    else
    {
      if ( msgLevel(MSG::DEBUG) )
        debug() << "-- REJECT L0Data : '" << prefix<<"L0Data_"<<rename(name) <<endmsg;
    }
  }

  // L0DU emulated Decision (incl. downscaling !!) Useful on MC data

  if( m_tcks.empty() )return StatusCode::SUCCESS;
  LHCb::L0ProcessorDatas* proDatas= m_l0->L0ProcessorDatas();
  for( std::vector<std::string>::iterator it=m_tcks.begin();m_tcks.end()!=it;++it )
  {
    const std::string& stck =  *it;
    std::istringstream istck( stck.c_str() );
    int itck(0);
    istck >> std::hex >> itck;
    int dec = 0;
    LHCb::L0DUConfig* config = m_l0conf->config( itck );
    if ( config )
    {
      const StatusCode sc = m_l0emu->process(config, proDatas );
      if( sc.isSuccess() && config->emulatedDecision() )dec=1;
    }
    tuple->column( prefix+"L0Emu_TCK"+stck, dec);
    if ( msgLevel(MSG::DEBUG) )
      debug() << " >> emulated (pre)decision for TCK : " << stck
              << " ("<<itck <<") : " << dec << endmsg;
  }

  return StatusCode::SUCCESS;
}

std::string TupleToolL0Data::rename(const std::string& name)
{
  std::string rname=name;
  if( std::string::npos != rname.find("(") )
  {
    const int index = rname.find("(");
    rname.erase(index,1);
    rname.insert(index,"_");
  }
  if( std::string::npos != rname.find(")") )
  {
    const int index = rname.find(")");
    rname.erase(index,1);
  }
  return rname;
}

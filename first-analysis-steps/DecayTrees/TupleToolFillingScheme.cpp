// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// local
#include "TupleToolFillingScheme.h"
#include "Event/ODIN.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolFillingScheme
//
// 2015-08-15 : Albert Bursche
// 2018-04-23 : Bartek Rachwal
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolFillingScheme::TupleToolFillingScheme( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
  : TupleToolBase ( type, name, parent )
  ,m_lhc("Conditions/Online/LHC")
  ,m_lhcFS("Conditions/Online/LHCb/LHCFillingScheme")
{
  declareInterface<IEventTupleTool>(this);
  declareProperty( "BXOffset"  , m_BXOffset = 0 );
 }

//=============================================================================
StatusCode TupleToolFillingScheme::initialize() {
  StatusCode sc = TupleToolBase::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm
  m_condFilling = new Condition();
  if (this->existDet<Condition>(m_lhcFS))
    registerCondition(m_lhcFS, m_condFilling, &TupleToolFillingScheme::i_cacheFillingData);
  else
    warning() << "Could not find Filling Scheme in DB!" << endmsg;

  // Check if the LHC information is available in the conditions database.
  m_condLHC = new Condition();
  if (this->existDet<Condition>(m_lhc)) {
    registerCondition(m_lhc, m_condLHC, &TupleToolFillingScheme::cacheFill);
    // First update.
    sc = updMgrSvc()->update(this);
    if (sc.isFailure()) return Error("Cannot retrieve fill number.", sc);

  } else {
    warning() << "Cannot find " << m_lhc << " in database" << endmsg;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolFillingScheme::~TupleToolFillingScheme() {}

//=============================================================================
StatusCode TupleToolFillingScheme::fill( Tuples::Tuple& tup )
{
  const std::string prefix = fullName();
  bool test = true;
  //Get bunch crossing ID from ODIN
  LHCb::ODIN* odin = getIfExists<LHCb::ODIN>(LHCb::ODINLocation::Default);

  // should remain flagged as an error and stop the run
  if(!odin)
    return Error("ODIN cannot be loaded", StatusCode::FAILURE);

  long bxid = odin->bunchId();
  int m_type = odin->bunchCrossingType();

  if ( msgLevel(MSG::DEBUG) )
    debug() << "ODIN BCID: " << bxid << endmsg;

  test &= tup->column( prefix+"fill", m_fillNumber);

  //logic from  FilterFillingScheme::processDB
  const int max_buckets = m_B1FillingScheme.size();
  assert(static_cast<size_t>(max_buckets) == m_B2FillingScheme.size());
  if ( msgLevel(MSG::DEBUG) )
    debug() << "max_buckets: " << max_buckets << endmsg;
  if (max_buckets){

      // ODIN starts counting at 1 - so we have to subtract 1 always to index into the string
      unsigned int index = (m_BXOffset + bxid + max_buckets - 1) % max_buckets;
      unsigned int indexPrev1 = (index - 1 ) % max_buckets;
      unsigned int indexPrev2 = (index - 2 ) % max_buckets;
      unsigned int indexNext1 = (index + 1 ) % max_buckets;
      unsigned int indexNext2 = (index + 2 ) % max_buckets;

      int firstBB, firstEE, firstBE, firstEB;
      int lastBB, lastEE, lastBE, lastEB;

      lastBB = lastBE = lastEB = lastEE = firstEE = firstBB = firstBE = firstEB = -1;
      bool isBeamBeam =  (m_B1FillingScheme[index] == '1') && (m_B2FillingScheme[index] == '1');
      for(int i=1; ( lastBB==-1||lastBE==-1||lastEB==-1||firstBB==-1||firstEE==-1||lastEE==-1||firstBE==-1||firstEB==-1 ) && i<max_buckets;++i)
      {
        if (m_B1FillingScheme[(index-i) % max_buckets] == '0' &&  m_B2FillingScheme[(index-i) % max_buckets] == '0')
          if (lastEE==-1) lastEE=i;

        if (m_B1FillingScheme[(index+i) % max_buckets] == '0' &&  m_B2FillingScheme[(index+i) % max_buckets] == '0')
          if (firstEE==-1) firstEE=i;

        if (m_B1FillingScheme[(index+i) % max_buckets] == '1' &&  m_B2FillingScheme[(index+i) % max_buckets] == '1')
          if (firstBB==-1) firstBB=i;

        if (m_B1FillingScheme[(index-i) % max_buckets] == '1' &&  m_B2FillingScheme[(index-i) % max_buckets] == '1')
          if (lastBB==-1) lastBB=i;

        if (m_B1FillingScheme[(index-i) % max_buckets] == '0' &&  m_B2FillingScheme[(index-i) % max_buckets] == '1')
          if (lastEB==-1) lastEB=i;

        if (m_B1FillingScheme[(index-i) % max_buckets] == '1' &&  m_B2FillingScheme[(index-i) % max_buckets] == '0')
          if (lastBE==-1) lastBE=i;

        if (m_B1FillingScheme[(index+i) % max_buckets] == '1' &&  m_B2FillingScheme[(index+i) % max_buckets] == '0')
          if (firstBE==-1) firstBE=i;

        if (m_B1FillingScheme[(index+i) % max_buckets] == '0' &&  m_B2FillingScheme[(index+i) % max_buckets] == '1')
          if (firstBE==-1) firstEB=i;
      }

      ///////////////////////////////////////////////////////////////////////////////////////
      int tmptypePrev1 = -1;
      if(  (m_B1FillingScheme[indexPrev1] == '0') && (m_B2FillingScheme[indexPrev1] == '0') ) tmptypePrev1=0;
      if(  (m_B1FillingScheme[indexPrev1] == '1') && (m_B2FillingScheme[indexPrev1] == '0') ) tmptypePrev1=1;
      if(  (m_B1FillingScheme[indexPrev1] == '0') && (m_B2FillingScheme[indexPrev1] == '1') ) tmptypePrev1=2;
      if(  (m_B1FillingScheme[indexPrev1] == '1') && (m_B2FillingScheme[indexPrev1] == '1') ) tmptypePrev1=3;
      ///////////////////////////////////////////////////////////////////////////////////////
      int tmptypePrev2 = -1;
      if(  (m_B1FillingScheme[indexPrev2] == '0') && (m_B2FillingScheme[indexPrev2] == '0') ) tmptypePrev2=0;
      if(  (m_B1FillingScheme[indexPrev2] == '1') && (m_B2FillingScheme[indexPrev2] == '0') ) tmptypePrev2=1;
      if(  (m_B1FillingScheme[indexPrev2] == '0') && (m_B2FillingScheme[indexPrev2] == '1') ) tmptypePrev2=2;
      if(  (m_B1FillingScheme[indexPrev2] == '1') && (m_B2FillingScheme[indexPrev2] == '1') ) tmptypePrev2=3;
      ///////////////////////////////////////////////////////////////////////////////////////
      int tmptypeNext1 = -1;
      if(  (m_B1FillingScheme[indexNext1] == '0') && (m_B2FillingScheme[indexNext1] == '0') ) tmptypeNext1=0;
      if(  (m_B1FillingScheme[indexNext1] == '1') && (m_B2FillingScheme[indexNext1] == '0') ) tmptypeNext1=1;
      if(  (m_B1FillingScheme[indexNext1] == '0') && (m_B2FillingScheme[indexNext1] == '1') ) tmptypeNext1=2;
      if(  (m_B1FillingScheme[indexNext1] == '1') && (m_B2FillingScheme[indexNext1] == '1') ) tmptypeNext1=3;
      ///////////////////////////////////////////////////////////////////////////////////////
      int tmptypeNext2 = -1;
      if(  (m_B1FillingScheme[indexNext2] == '0') && (m_B2FillingScheme[indexNext2] == '0') ) tmptypeNext2=0;
      if(  (m_B1FillingScheme[indexNext2] == '1') && (m_B2FillingScheme[indexNext2] == '0') ) tmptypeNext2=1;
      if(  (m_B1FillingScheme[indexNext2] == '0') && (m_B2FillingScheme[indexNext2] == '1') ) tmptypeNext2=2;
      if(  (m_B1FillingScheme[indexNext2] == '1') && (m_B2FillingScheme[indexNext2] == '1') ) tmptypeNext2=3;
      ///////////////////////////////////////////////////////////////////////////////////////

      test &= tup->column( prefix+"CrossingsAfterToEE"  ,(int)firstEE );
      test &= tup->column( prefix+"CrossingsAfterToBB"  ,(int)firstBB );
      test &= tup->column( prefix+"CrossingsAfterToBE"  ,(int)firstBE );
      test &= tup->column( prefix+"CrossingsAfterToEB"  ,(int)firstEB );
      test &= tup->column( prefix+"CrossingsBeforeToEE" ,(int)lastEE );
      test &= tup->column( prefix+"CrossingsBeforeToBB" ,(int)lastBB );
      test &= tup->column( prefix+"CrossingsBeforeToBE",(int)lastBE );
      test &= tup->column( prefix+"CrossingsBeforeToEB",(int)lastEB );
      test &= tup->column( prefix+"IsBeamBeam", isBeamBeam);
      test &= tup->column( prefix+"Prev1" , tmptypePrev1 );
      test &= tup->column( prefix+"Prev2" , tmptypePrev2 );
      test &= tup->column( prefix+"Next1" , tmptypeNext1 );
      test &= tup->column( prefix+"Next2" , tmptypeNext2 );
      int isSpillOver = 0;
      int isEmpty     = 0;
      int isPreSpill  = 0;
      if( m_type == 0 && tmptypePrev1 == 3 && tmptypeNext1 == 0 && tmptypeNext2 == 0 ) isSpillOver = 1;
      if( m_type == 0 && tmptypePrev2 == 0 && tmptypePrev1 == 0 && tmptypeNext1 == 0 && tmptypeNext2 == 0 ) isEmpty = 1;
      if( m_type == 0 && tmptypePrev2 == 0 && tmptypePrev1 == 0 && tmptypeNext1 == 3 ) isPreSpill = 1;
      test &= tup->column( prefix+"isSpillOver",isSpillOver);
      test &= tup->column( prefix+"isEmpty",isEmpty);
      test &= tup->column( prefix+"isPreSpill",isPreSpill);
    }
    else
      {
        test &= tup->column( prefix+"CrossingsAfterToEE"  ,(int) -1);
        test &= tup->column( prefix+"CrossingsAfterToEB"  ,(int) -1);
        test &= tup->column( prefix+"CrossingsAfterToBE"  ,(int) -1);
        test &= tup->column( prefix+"CrossingsAfterToBB"  ,(int) -1);
        test &= tup->column( prefix+"CrossingsBeforeToEE" ,(int) -1);
        test &= tup->column( prefix+"CrossingsBeforeToBB" ,(int) -1);
        test &= tup->column( prefix+"CrossingsBeforeToBE" ,(int) -1);
        test &= tup->column( prefix+"CrossingsBeforeToEB" ,(int) -1);
        test &= tup->column( prefix+"IsBeamBeam", false);
        test &= tup->column( prefix+"Prev1" , -1. );
        test &= tup->column( prefix+"Prev2" , -1. );
        test &= tup->column( prefix+"Next1" , -1. );
        test &= tup->column( prefix+"Next2" , -1. );
        test &= tup->column( prefix+"isSpillOver", -1.);
        test &= tup->column( prefix+"isEmpty", -1.);
        test &= tup->column( prefix+"isPreSpill", -1.);
      }


  return StatusCode(test);
}

//=========================================================================
// Extract data from Filling scheme
//=========================================================================
StatusCode TupleToolFillingScheme::i_cacheFillingData() {
  // data from /Conditions/Online/LHCb/LHCFillingScheme
  //if ( msgLevel(MSG::DEBUG) ) debug() << "callback Filling:" << endmsg;
  m_B1FillingScheme = (std::string) m_condFilling->param<std::string>("B1FillingScheme");
  m_B2FillingScheme = (std::string) m_condFilling->param<std::string>("B2FillingScheme");
  if ( msgLevel(MSG::DEBUG) )
    debug() <<   "B1FillingScheme: " << m_B1FillingScheme
    << "\nB2FillingScheme: " << m_B2FillingScheme  << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Call-back function for retrieving fill number from conditions database.
//=============================================================================
StatusCode TupleToolFillingScheme::cacheFill(){
  m_fillNumber = m_condLHC->param<int>("FillNumber");
  return StatusCode::SUCCESS;
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolFillingScheme )

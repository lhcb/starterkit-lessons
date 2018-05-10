// Include files

// from Gaudi
#include "Event/ODIN.h"
#include "Event/HCDigit.h"
// from LHCb
#include "DetDesc/Condition.h"

// local
#include "TupleToolHerschel.h"

DECLARE_COMPONENT( TupleToolHerschel )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolHerschel::TupleToolHerschel( const std::string& type,
                                              const std::string& name,
                                              const IInterface* parent )
  : TupleToolBase ( type, name, parent ),
    m_cond(nullptr)
{
  declareInterface<IEventTupleTool>(this);
}

StatusCode TupleToolHerschel::initialize() {
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  // Set up the mapping
  m_channels.resize(5);
  m_masked.resize(5);
  // Check if the mapping is available in the conditions database.
  const std::string location = "Conditions/ReadoutConf/HC/Mapping";
  if (existDet<Condition>(location)) {
    registerCondition(location, m_cond, &TupleToolHerschel::cacheMapping);
    // First update.
    sc = updMgrSvc()->update(this);
    if (sc.isFailure()) {
      return Error("Cannot update mapping.", StatusCode::FAILURE);
    }
  } else {
    warning() << "Cannot find " << location << " in database" << endmsg;
    mapChannels(m_channelsB0, m_sparesB0, m_masksB0, 0, true);
    mapChannels(m_channelsB1, m_sparesB1, m_masksB1, 1, true);
    mapChannels(m_channelsB2, m_sparesB2, m_masksB2, 2, true);
    mapChannels(m_channelsF1, m_sparesF1, m_masksF1, 1, false);
    mapChannels(m_channelsF2, m_sparesF2, m_masksF2, 2, false);
  }
  printMapping();

  // Check if the FOM parameters are available in the conditions database.
  const std::string location_fom = "Conditions/Calibration/HC/FigureOfMerit_SimpleChi2";
  if (existDet<Condition>(location_fom)) {
    registerCondition(location_fom, m_condFOM, &TupleToolHerschel::cacheFOMParameters);
    // First update.
    sc = updMgrSvc()->update(this);
    if (sc.isFailure()) {
      return Error("Cannot update FOM constants.");
    }
  } else {
    warning() << "Cannot find " << location_fom << " in database." << endmsg;
    _makeFOM = false ;
    warning() << "Cannot set FOM constants, will not create Herschel FOM in NTuple. Please fix FOM constants location in CondDB." << endmsg;
    //}
  }

  return sc ;
}

//=============================================================================
// Update the channel map using the conditions database.
//=============================================================================
StatusCode TupleToolHerschel::cacheMapping() {

  m_crateB = m_cond->param<int>("CrateB");
  m_crateF = m_cond->param<int>("CrateF");

  m_channelsB0 = m_cond->paramVect<int>("ChannelsB0");
  m_channelsB1 = m_cond->paramVect<int>("ChannelsB1");
  m_channelsB2 = m_cond->paramVect<int>("ChannelsB2");
  m_channelsF1 = m_cond->paramVect<int>("ChannelsF1");
  m_channelsF2 = m_cond->paramVect<int>("ChannelsF2");

  m_sparesB0 = m_cond->paramVect<int>("ReferenceChannelsB0");
  m_sparesB1 = m_cond->paramVect<int>("ReferenceChannelsB1");
  m_sparesB2 = m_cond->paramVect<int>("ReferenceChannelsB2");
  m_sparesF1 = m_cond->paramVect<int>("ReferenceChannelsF1");
  m_sparesF2 = m_cond->paramVect<int>("ReferenceChannelsF2");

  m_masksB0 = m_cond->paramVect<int>("MasksB0");
  m_masksB1 = m_cond->paramVect<int>("MasksB1");
  m_masksB2 = m_cond->paramVect<int>("MasksB2");
  m_masksF1 = m_cond->paramVect<int>("MasksF1");
  m_masksF2 = m_cond->paramVect<int>("MasksF2");

  mapChannels(m_channelsB0, m_sparesB0, m_masksB0, 0, true);
  mapChannels(m_channelsB1, m_sparesB1, m_masksB1, 1, true);
  mapChannels(m_channelsB2, m_sparesB2, m_masksB2, 2, true);
  mapChannels(m_channelsF1, m_sparesF1, m_masksF1, 1, false);
  mapChannels(m_channelsF2, m_sparesF2, m_masksF2, 2, false);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Setup the channel map for a given station.
//=============================================================================
bool TupleToolHerschel::mapChannels(const std::vector<int>& channels,
                                    const std::vector<int>& refs,
                                    const std::vector<int>& masks,
                                    const unsigned int station,
                                    const bool bwd) {

  const unsigned int offset = bwd ? 0 : 2;
  // Check if the input is valid.
  if (channels.size() != 4 || refs.size() != 4 || masks.size() != 4) {
    std::string s = bwd ? "B" : "F";
    s += std::to_string(station);
    warning() << "Invalid channel map for station " << s
              << ". Masking all quadrants." << endmsg;
    m_channels[station + offset].assign(4, 0);
    m_masked[station + offset].assign(4, true);
    return false;
  }

  const unsigned int crate = bwd ? m_crateB : m_crateF;
  m_channels[station + offset].resize(4);
  m_masked[station + offset].resize(4);
  for (unsigned int i = 0; i < 4; ++i) {
    if (channels[i] < 0) {
      std::string s = bwd ? "B" : "F";
      s += std::to_string(station);
      warning() << "Invalid channel number " << channels[i]
                << ". Masking quadrant " << s << i << endmsg;
      m_masked[station + offset][i] = true;
      continue;
    }
    m_channels[station + offset][i] = (crate << 6) | channels[i];
    m_masked[station + offset][i] = masks[i] != 0;
  }
  return true;
}

//=============================================================================
// Print the channel map for information
//=============================================================================
void TupleToolHerschel::printMapping() const {

  info() << " Quadrant   ID   Ref. ID    Comment" << endmsg;
  const std::vector<std::string> stations = {"B0", "B1", "B2", "F1", "F2"};
  const unsigned int nStations = stations.size();
  for (unsigned int i = 0; i < nStations; ++i) {
    for (unsigned int j = 0; j < 4; ++j) {
      const std::string comment = m_masked[i][j] ? "Masked" : "";
      info() << format("   %2s/%1d", stations[i].c_str(), j)
             << format("   %4d", m_channels[i][j])
             << format(" %-10s", comment.c_str()) << endmsg;
    }
  }

}

//=============================================================================
// Retrieve the FOM parameters from the conditions database.
//=============================================================================
StatusCode TupleToolHerschel::cacheFOMParameters()
{
  info() << "Updating HRC figure of merit parameters." << endmsg;
  m_hrcFomVnum = m_condFOM->param<int>("FOMvNum");
  info() << "HRC FOM version number " << m_hrcFomVnum << endmsg;

  m_meanConfig.clear();
  m_rmsConfig.clear();
  const std::vector<std::string> stations = {"B0", "B1", "B2", "F1", "F2"};
  for (const auto& st : stations) {
    const auto meanEven = m_condFOM->paramVect<double>("Mean" + st + "Even");
    const auto meanOdd  = m_condFOM->paramVect<double>("Mean" + st + "Odd");
    const auto rmsEven  = m_condFOM->paramVect<double>("RMS" + st + "Even");
    const auto rmsOdd   = m_condFOM->paramVect<double>("RMS" + st + "Odd");
    for (unsigned int i = 0; i < 4; ++i) {
      m_meanConfig.push_back(meanEven[i]);
      m_meanConfig.push_back(meanOdd[i]);
      m_rmsConfig.push_back(rmsEven[i]);
      m_rmsConfig.push_back(rmsOdd[i]);
    }
  }
  return setFOMParameters();
}

//=============================================================================
// Update the calibration constants.
//=============================================================================
StatusCode TupleToolHerschel::setFOMParameters() {

  const unsigned int nStations = 5;
  const unsigned int nQuadrants = 4;
  // Check if the lists of calibration constants have the right size.
  const unsigned int nExpectedSize = nStations * 4 * 2;
  if (m_meanConfig.size() != nExpectedSize ||
      m_rmsConfig.size()  != nExpectedSize ) {
    return Error("List of FOM calibration constants has incorrect size.");
  }
  m_mean.resize(nStations);
  m_rms.resize(nStations);
  unsigned int index = 0;
  for (unsigned int i = 0; i < nStations; ++i) {
    m_mean[i].resize(nQuadrants);
    m_rms[i].resize(nQuadrants);
    for (unsigned int j = 0; j < nQuadrants; ++j) {
      m_mean[i][j].resize(2);
      m_rms[i][j].resize(2);
      for (unsigned int k = 0; k < 2; ++k) {
        m_mean[i][j][k] = m_meanConfig[index];
        m_rms[i][j][k]  = m_rmsConfig[index];
        ++index;
      }
    }
  }
  return StatusCode::SUCCESS;

}

//=============================================================================
StatusCode TupleToolHerschel::fill( Tuples::Tuple& tup )
{
  const std::string prefix = fullName();

  // Get ODIN
  const LHCb::ODIN* odin = getIfExists<LHCb::ODIN>(evtSvc(),LHCb::ODINLocation::Default);
  if ( !odin ) { odin = getIfExists<LHCb::ODIN>(evtSvc(),LHCb::ODINLocation::Default,false); }
  if ( !odin ) {
    // should always be available ...
    return Error( "Cannot load the ODIN data object", StatusCode::SUCCESS );
  }
  const unsigned int bxid = odin->bunchId();
  const unsigned int parity = bxid % 2;

  // Get Herschel digits
  LHCb::HCDigits* digits = getIfExists<LHCb::HCDigits>(m_DigitsLocation);
  if ( !digits ) {
    // should always be available ...
    return Error( "Cannot load the HCDigits data object", StatusCode::SUCCESS );
  }


  // Fill the Herschel information
  double log_hrc_fom   = -999 ;
  double log_hrc_fom_B = -999 ;
  double log_hrc_fom_F = -999 ;
  double hrc_fom   = 0. ;
  double hrc_fom_B = 0. ;
  double hrc_fom_F = 0. ;

  // Herschel FOM : log( sum( chi2 ))
  bool test = true;
  const std::vector<std::string> stations = {"B0", "B1", "B2", "F1", "F2"};
  const unsigned int nStations = 5;
  for (unsigned int i = 0; i < nStations; ++i) 
  {
    for (unsigned int j = 0; j < 4; ++j) 
    {
      // Build channel name
      const std::string ch = stations[i] + std::to_string(j);
      LHCb::HCCellID id(m_channels[i][j]);
      const LHCb::HCDigit* digit = digits->object(id);

      // Create a value of ADC to write out every event, irrespective of whether a HCdigit is found
      int adc = -999 ;

      // If a digit is found *and* the channel is not masked
      if (digit && !m_masked[i][j] ) 
      {
        adc = digit->adc();

        // Make Herschel FOM
        if (_makeFOM) {
          // A counter is to be ignored if the width is set to <0:
          if ( m_rms[i][j][parity] < 0.0 ) continue ;

          double quadrant_pull = ( adc - m_mean[i][j][parity] ) / m_rms[i][j][parity] ;
          hrc_fom += quadrant_pull * quadrant_pull ;

          // Make Herschel FOM for B-side and F-side separately
          if (i<3) 
            hrc_fom_B += quadrant_pull * quadrant_pull ;
          else 
            hrc_fom_F += quadrant_pull * quadrant_pull ;
        }
        else {
          hrc_fom   = -999. ;
          hrc_fom_B = -999. ;
          hrc_fom_F = -999. ;
        }
      }
      else if (!digit) 
      {
        warning() << "Cannot retrieve digit for " << ch << endmsg;
      }
      else 
      {
        warning() << "Channel " << ch << " is masked." << endmsg;
      }
     
      // Always write out the ADC 
      test &= tup->column( prefix+ch, adc);
    }
  }

  if ( hrc_fom>0 )   log_hrc_fom   = std::log(hrc_fom) ;
  if ( hrc_fom_B>0 ) log_hrc_fom_B = std::log(hrc_fom_B) ;
  if ( hrc_fom_F>0 ) log_hrc_fom_F = std::log(hrc_fom_F) ;
  test &= tup->column( prefix+"log_hrc_fom"  +"_v"+std::to_string(m_hrcFomVnum) , log_hrc_fom ) ;
  test &= tup->column( prefix+"log_hrc_fom_B"+"_v"+std::to_string(m_hrcFomVnum) , log_hrc_fom_B ) ;
  test &= tup->column( prefix+"log_hrc_fom_F"+"_v"+std::to_string(m_hrcFomVnum) , log_hrc_fom_F ) ;

  // Fill ADC vectors
  std::vector<double> adcB,adcF;
  for (const LHCb::HCDigit* dig : *digits) {
    const int cell = int(dig->cellID().cellID());
    const int crate = cell >> 6;
    if (crate == 0) {
      adcB.push_back((double)dig->adc());
    } else if (crate == 1) {
      adcF.push_back((double) dig->adc());
    } else {
      info() << "Crate = " << crate << ". We have a problem." << endmsg;
    }
  }

  // Fill tuple
  test &= tup->farray( prefix+"adc_B", adcB.begin(), adcB.end(),prefix+"nchB",1000);
  test &= tup->farray( prefix+"adc_F", adcF.begin(), adcF.end(),prefix+"nchF",1000);

  return StatusCode(test);
}

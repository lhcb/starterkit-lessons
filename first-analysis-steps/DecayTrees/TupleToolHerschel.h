#ifndef TUPLETOOLHERSCHEL_H
#define TUPLETOOLHERSCHEL_H 1

// Include files
// from Gaudi
#include "Kernel/IEventTupleTool.h"            // Interface
#include "DecayTreeTupleBase/TupleToolBase.h"

/** @class TupleToolHerschel TupleToolHerschel.h
 *
 *  Fills Herschel information.
 *
 *  @author Dan Johnson
 *  @date   2015-07-08
 */
class TupleToolHerschel : public TupleToolBase,
                           virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolHerschel( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);
  StatusCode initialize() override;///< Algorithm initialization
  StatusCode fill( Tuples::Tuple& ) override;///< Fill tuple

  Gaudi::Property<std::string> m_DigitsLocation{this, "DigitsLocation", "Raw/HC/Digits" , "TES location of Herschel digits"};
  // Mappings (to be retrieved from conditions database).
  Gaudi::Property<unsigned int> m_crateB{this, "CrateB", 0 , "Number of B-side crate"};
  Gaudi::Property<unsigned int> m_crateF{this, "CrateF", 1 , "Number of F-side crate"};

  Gaudi::Property<std::vector<int>> m_masksB0{this, "MasksB0", {false, false, false, false}, "B0 masking"};
  Gaudi::Property<std::vector<int>> m_masksB1{this, "MasksB1", {false, false, false, false}, "B1 masking"};
  Gaudi::Property<std::vector<int>> m_masksB2{this, "MasksB2", {false, false, false, false}, "B2 masking"};
  Gaudi::Property<std::vector<int>> m_masksF1{this, "MasksF1", {false, false, false, false}, "F1 masking"};
  Gaudi::Property<std::vector<int>> m_masksF2{this, "MasksF2", {false, false, false, false}, "F2 masking"};

  Gaudi::Property<std::vector<int>> m_channelsB0{this, "ChannelsB0", {}, "B0 channels"};
  Gaudi::Property<std::vector<int>> m_channelsB1{this, "ChannelsB1", {}, "B1 channels"};
  Gaudi::Property<std::vector<int>> m_channelsB2{this, "ChannelsB2", {}, "B2 channels"};
  Gaudi::Property<std::vector<int>> m_channelsF1{this, "ChannelsF1", {}, "F1 channels"};
  Gaudi::Property<std::vector<int>> m_channelsF2{this, "ChannelsF2", {}, "F2 channels"};

  Gaudi::Property<std::vector<int>> m_sparesB0{this, "SpareChannelsB0", {}, "B0 spare channels"};
  Gaudi::Property<std::vector<int>> m_sparesB1{this, "SpareChannelsB1", {}, "B1 spare channels"};
  Gaudi::Property<std::vector<int>> m_sparesB2{this, "SpareChannelsB2", {}, "B2 spare channels"};
  Gaudi::Property<std::vector<int>> m_sparesF1{this, "SpareChannelsF1", {}, "F1 spare channels"};
  Gaudi::Property<std::vector<int>> m_sparesF2{this, "SpareChannelsF2", {}, "F2 spare channels"};

protected:
  /// Conditions database information
  Condition* m_cond;
  Condition* m_condFOM = nullptr;

  /// Channel numbers for each quadrant
  std::vector<std::vector<unsigned int> > m_channels;
  /// Masked flags for each quadrant
  std::vector<std::vector<bool> > m_masked;
  /// Retrieve the mapping from the conditions database.
  StatusCode cacheMapping();
  /// Setup the mapping for a given station.
  bool mapChannels(const std::vector<int>& channels,
                   const std::vector<int>& refs,
                   const std::vector<int>& masks,
                   const unsigned int station, const bool bwd);
  /// Print the channel mapping for information.
  void printMapping() const;

  StatusCode cacheFOMParameters();
  StatusCode setFOMParameters();

  /// FOM constants for each channel and parity
  /// station (x5) ; quadrant (x4) ; parity (x2)
  std::vector<std::vector<std::vector<float> > > m_mean;
  std::vector<std::vector<std::vector<float> > > m_rms;
  bool _makeFOM=true;

  // Input parameters from CondDB
  std::vector<float> m_meanConfig;
  std::vector<float> m_rmsConfig;
  int m_hrcFomVnum;
};

#endif

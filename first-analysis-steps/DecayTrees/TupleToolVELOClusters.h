#ifndef TUPLETOOLRECOSTATS_H
#define TUPLETOOLRECOSTATS_H 1

// Include files
// from Gaudi
#include "Kernel/IEventTupleTool.h"            // Interface
#include "DecayTreeTupleBase/TupleToolBase.h"

#include "Event/VeloCluster.h"
#include "Event/Track.h"

/** @class TupleToolVELOClusters TupleToolVELOClusters.h
 *
 *  Fills VELO information
 */
class TupleToolVELOClusters : public TupleToolBase,
                              virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolVELOClusters( const std::string& type,
                         const std::string& name,
                         const IInterface* parent);

  /// Declare properties
  Gaudi::Property< std::string > m_locationClusters{this, "VeloClusterLocation", LHCb::VeloClusterLocation::Default, "Location of VELO clusters"};

  StatusCode fill( Tuples::Tuple& ) override;///< Fill tuple
  //  int unusedVelo();

};

#endif // TUPLETOOLRECOSTATS_H


#ifndef TUPLETOOLVTXISOLN_H
#define TUPLETOOLVTXISOLN_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Kernel/IExtraInfoTool.h"            // Interface

struct IDVAlgorithm;
struct IDistanceCalculator;
struct IVertexFit;

namespace LHCb {
  class Particle;
  class Vertex;
}

/** @class TupleToolVtxIsoln TupleToolVtxIsoln.h
 *
 * \brief Fill vertex isolation information for DecayTreeTuple
 *    Take vertexed particle and add other tracks of the
 *    event, one by one, building a new vertex. By default, this tool
 *    will look for ExtraInfo filled by the VertexIsolation tool. If it
 *    is not found, the tool is run to fill the ExtraInfo.
 *
 * Leaves filled by the tool:
 *    - (head)_NumVtxWithinChi2WindowOneTrack: number of particles that generate a vertex within a chi2 window
 *    - (head)_SmallestDeltaChi2OneTrack: smallest delta chi2 when adding one track
 *    - (head)_SmallestDeltaChi2MassOneTrack: mass of the candidate with the smallest delta chi2
 *    - (head)_SmallestDeltaChi2TwoTracks: smallest delta chi2 when adding one track to the
 *      combination that has the smallest delta chi2 when adding one track
 *    - (head)_SmallestDeltaChi2MassTwoTracks: mass of the candidate with the smallest delta chi2
 *      when adding one track to the combination that has the smallest delta chi2 when adding one track
 *
 *
 *  @author Albert Puig, based on previous work from Mitesh Patel, Patrick Koppenburg
 *  @date   2013-08-05
 *
 */
class TupleToolVtxIsoln : public TupleToolBase,
                          virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolVtxIsoln( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*
           , const LHCb::Particle*
           , const std::string&
           , Tuples::Tuple& ) override;

private:

  // Isolation tool
  IExtraInfoTool *m_isolationTool ;
  struct IsolationIndices { int first; int last; } ;
  IsolationIndices m_indices ;
  // Variable names to store in the tuple
  std::map< int, std::string > m_varNames ;
};

#endif // TUPLETOOLVTXISOLN_H

// EOF

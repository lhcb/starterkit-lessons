// $Id: TupleToolPrimaries.h,v 1.7 2010-03-01 11:47:34 rlambert Exp $
#ifndef JBOREL_TUPLETOOLPRIMARIES_H
#define JBOREL_TUPLETOOLPRIMARIES_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface
#include "Event/RecVertex.h"

class DVAlgorithm ;

/** @class TupleToolPrimaries TupleToolPrimaries.h jborel/TupleToolPrimaries.h
 *
 * \brief Primary vertices properties for DecayTreeTuple
 *
 * Tuple columns:
 * - coordinates PVX, PVY, PVZ
 * - errors PVXERR, PVYERR, PVZERR
 * - vertex chi2 PVCHI
 * - vertex ndf PVNDOF
 * - Nb of tracks used to do the vertex PVNTRACKS
 *
 * \sa DecayTreeTuple
 *  @author Jeremie Borel
 *  @date   2007-11-07
 */
class TupleToolPrimaries : public TupleToolBase, virtual public IEventTupleTool {
public:
  /// Standard constructor
  TupleToolPrimaries( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolPrimaries( ){}; ///< Destructor
  StatusCode initialize() override;

  StatusCode fill( Tuples::Tuple& ) override;

private:
  double sumPT(const LHCb::RecVertex* pv) const ; ///< sum of Pt of all tracks
  std::string m_pvLocation ; ///<  PV location to be used. If empty, take context-dependent default
  //DVAlgorithm* m_dva;

};
#endif // JBOREL_TUPLETOOLPRIMARIES_H

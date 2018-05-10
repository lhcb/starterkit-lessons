// $Id: MCTupleToolPrimaries.h,v 1.2 2010-01-26 15:39:26 rlambert Exp $
#ifndef MCTUPLETOOLPRIMARIES_H
#define MCTUPLETOOLPRIMARIES_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

/** @class MCTupleToolPrimaries MCTupleToolPrimaries.h
 *
 * \brief Primary MC vertices properties for DecayTreeTuple
 *
 * Tuple columns:
 * - coordinates PVX, PVY, PVZ, PVT (time)
 *
 *  @author Patrick Koppenburg
 *  @date   2009-11-16
 */
class MCTupleToolPrimaries : public TupleToolBase, virtual public IEventTupleTool {
public:
  /// Standard constructor
  MCTupleToolPrimaries( const std::string& type,
                        const std::string& name,
                        const IInterface* parent);

  virtual ~MCTupleToolPrimaries( ); ///< Destructor
  StatusCode fill( Tuples::Tuple& ) override;

};
#endif // MCTUPLETOOLPRIMARIES_H

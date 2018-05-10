// $Id: TupleToolStripping.h,v 1.1 2010-04-26 12:49:59 pkoppenb Exp $
#ifndef TUPLETOOLSTRIPPING_H
#define TUPLETOOLSTRIPPING_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"

#include "TupleToolTriggerBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

/** @class TupleToolStripping TupleToolStripping.h
 *
 *
 *  @author Patrick Koppenburg
 *  @date   2010-04-23
 */
class TupleToolStripping : public TupleToolTriggerBase,
                           virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolStripping( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  ~TupleToolStripping( ); ///< Destructor

  StatusCode initialize() override;

  using TupleToolTriggerBase::fill;
  StatusCode fill( Tuples::Tuple& tuple ) override;

private:

  std::string m_location ; ///< HDR location

  /// property: list of stripping lines to specifically look at
  std::vector<std::string> m_strippingList;
};

#endif // TUPLETOOLSTRIPPING_H

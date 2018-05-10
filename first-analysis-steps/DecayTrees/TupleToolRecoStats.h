// $Id: TupleToolRecoStats.h,v 1.7 2010-09-09 12:22:42 pkoppenb Exp $
#ifndef TUPLETOOLRECOSTATS_H
#define TUPLETOOLRECOSTATS_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

#include "Event/Track.h"
#include "Event/RecSummary.h"
#include "HltDAQ/IReportConvert.h"
#include "HltDAQ/ReportConvertTool.h"

/** @class TupleToolRecoStats TupleToolRecoStats.h
 *
 *  Fills Reco stats, from RecSummary
 *
 *  @author Patrick Koppenburg, Fatima Soomro, Jibo He
 *  @date   2009-02-11
 */
class TupleToolRecoStats : public TupleToolBase,
                           virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolRecoStats( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolRecoStats( ); ///< Destructor
  StatusCode fill( Tuples::Tuple& ) override;///< Fill tuple

private:
  /// for converting objects in to summaries
  IReportConvert* m_conv;

};

#endif // TUPLETOOLRECOSTATS_H


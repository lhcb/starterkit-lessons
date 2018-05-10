#ifndef TUPLETOOLFILLINGSCHEME_H
#define TUPLETOOLFILLINGSCHEME_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

#include "Event/Track.h"

// CondDB
#include "DetDesc/Condition.h"
#include "GaudiKernel/IDetDataSvc.h"


/** @class TupleToolFillingScheme TupleToolFillingScheme.h
 *
 *  Fills LHC filling scheme info
 *
 *  @author Albert Bursche, Bartlomiej Rachwal
 *  @date   2015-08-15
 */
class TupleToolFillingScheme : public TupleToolBase,
                           virtual public IEventTupleTool
{

public:
  /// Standard constructor
  TupleToolFillingScheme( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolFillingScheme( ); ///< Destructor
  StatusCode fill( Tuples::Tuple& ) override;///< Fill tuple
  StatusCode initialize() override;

private:
  Condition *   m_condFilling   = nullptr;
  Condition *   m_condLHC      = nullptr;
  StatusCode cacheFill();
  StatusCode i_cacheFillingData();
  std::string m_lhc;
  std::string m_lhcFS;
  std::string m_B1FillingScheme;
  std::string m_B2FillingScheme;
  int m_BXOffset;
  int m_fillNumber = 0;
};

#endif // TUPLETOOLFILLINGSCHEME_H

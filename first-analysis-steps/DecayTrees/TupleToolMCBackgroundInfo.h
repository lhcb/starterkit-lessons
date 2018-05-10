// $Id: TupleToolMCBackgroundInfo.h,v 1.2 2010-01-26 15:39:26 rlambert Exp $
#ifndef JBOREL_TUPLETOOLMCBACKGROUNDINFO_H
#define JBOREL_TUPLETOOLMCBACKGROUNDINFO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

class IBackgroundCategory;

namespace LHCb
{
  class Particle;
}

/** @class TupleToolMCBackgroundInfo TupleToolMCBackgroundInfo.h
 *
 * \brief Fill the info from IBackgroundCategory
 *
 * An answer is only filled for composite particles.
 *
 * IBackgroundCategory controlled by property <b>IBackgroundCatagoryType</b>, default "BackgroundCategory".
 *
 * head_BKGCAT : category.
 *
 * \sa DecayTreeTuple
 *
 *  @author Jeremie Borel
 *  @date   2007-11-07
 */
class TupleToolMCBackgroundInfo : public TupleToolBase,
                                  virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolMCBackgroundInfo( const std::string& type,
                             const std::string& name,
                             const IInterface* parent);

  virtual ~TupleToolMCBackgroundInfo( ){}; ///< Destructor

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*,
                   const LHCb::Particle*,
                   const std::string&,
                   Tuples::Tuple& ) override;

private:

  std::vector<std::string> m_backCatTypes;
  std::vector<IBackgroundCategory*> m_bkgs;

};

#endif // JBOREL_TUPLETOOLMCBACKGROUNDINFO_H

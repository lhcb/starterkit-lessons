// $Id: MCTupleToolHierarchy.h,v 1.2 2010-01-26 15:39:25 rlambert Exp $
#ifndef SPOSS_TUPLETOOLMCHIERARCHY_H
#define SPOSS_TUPLETOOLMCHIERARCHY_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IMCParticleTupleTool.h"            // Interface

//struct Particle2MCLinker;
#include "Kernel/Particle2MCLinker.h"
#include "Kernel/IDaVinciAssociatorsWrapper.h"

/** @class MCTupleToolHierarchy
 *
 * \brief Fill MC hierarchy info if a link is present
 *
 * Requires association from TupleToolMCTruth, or a MCDecayTreeTuple
 *
 *
 * - head_MC_MOTHER_ID : true mc mother ID

 * - head_MC_MOTHER_KEY : true mc mother key

 * - head_MC_GD_MOTHER_ID : grand mother ID

 * - head_MC_GD_MOTHER_KEY : grand mother key

 * - head_MC_GD_GD_MOTHER_ID : grand grand mother ID

 * - head_MC_GD_GD_MOTHER_KEY : grand grand mother key

 * \sa DecayTreeTuple
 *
 *  @author Stephane Poss
 *  @date   2008-02-28
 */
class MCTupleToolHierarchy : public TupleToolBase,
                             virtual public IMCParticleTupleTool
{

public:

  /// Standard constructor
  MCTupleToolHierarchy( const std::string& type,
                        const std::string& name,
                        const IInterface* parent);

  virtual ~MCTupleToolHierarchy(){}; ///< Destructor

  StatusCode fill( const LHCb::MCParticle*
                   , const LHCb::MCParticle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

};

#endif // SPOSS_TUPLETOOLMCHIERARCHY_H

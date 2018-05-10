// $Id: TupleToolPi0Info.h,v 1.3 2010-01-26 15:39:26 rlambert Exp $
#ifndef _TUPLETOOLEPI0INFO_H
#define _TUPLETOOLEPI0INFO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
     // Interface
#include "Kernel/IParticleTupleTool.h"

#include "Kernel/Particle2MCLinker.h"
#include "Kernel/IDaVinciAssociatorsWrapper.h"


#include "Kernel/PP2MCLocation.h"

#include "Relations/RelationWeighted1D.h"

#include "CaloUtils/Calo2MC.h"
#include "CaloUtils/CaloParticle.h"

/** @class TupleToolPi0Info TupleToolPi0Info.h yamhis/TupleToolPi0Info.h
 *
 *"pi0s are special creatures they should to be treated in a special way....."
 *
 *
 *----- This Tool returns the MC particle associated to a Pi0( id and key)
 * and its whole family
 *----- The association is done via the relation table between the ProtoParticle and the MC
 *----- The pi0 type (resolved or merged) is taken into account
 *----- This tool runs in the HltContext do not forget to switch it on
 * using HltContext = True  from your DaVinci Python Script
 *This is Obviously heavily inspired from Olivier Deschamps codes for B2PiPiPi0
 *
 *
 * \sa DecayTreeTuple
 *  @author Amhis  Yasmine
 *  @date   2008-10-31
 */


//============================================================================
class TupleToolPi0Info : public TupleToolBase, virtual public IParticleTupleTool {
//==========================================================================
public:
 // Standard constructor
  TupleToolPi0Info( const std::string& type,
		    const std::string& name,
		    const IInterface* parent);

  virtual ~TupleToolPi0Info( ) = default; ///< Destructor

  StatusCode fill(const LHCb::Particle*, const LHCb::Particle*
			  ,  const std::string& ,Tuples::Tuple& ) override;
private:
  const LHCb::MCParticle* getMCPi0(const LHCb::Particle* part, double& weight);
  const LHCb::MCParticle* getMCGamma(const LHCb::Particle* part, double& weight);
private:
  typedef IRelationWeighted<LHCb::ProtoParticle,LHCb::MCParticle,double> mcTable;
  typedef mcTable::Range mcRange;
  mcRange getRange(const LHCb::Particle* part);
  mcRange getRange(const LHCb::ProtoParticle* proto);
private:
  bool m_RequireMCTruth;
  int m_Pi0ID{111};
  std::vector<std::string> m_assocInputs;
};
#endif // _TUPLETOOLEPI0INFO_H

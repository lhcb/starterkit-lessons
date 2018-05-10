// $Id: TupleToolPid.h,v 1.4 2010-03-04 14:02:03 xieyu Exp $
#ifndef JBOREL_TUPLETOOLPID_H
#define JBOREL_TUPLETOOLPID_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

/** @class TupleToolPid TupleToolPid.h
 *
 * @brief DLL and PID information to be stored in a Tuple
 *
 * \sa DecayTreeTuple
 *
 * For all particles:
 * - head_ID : particleID().pid();
 *
 * For the long lived particles (isBasicParticle()).
 * - head_PIDe : LHCb::ProtoParticle::CombDLLe
 * - head_PIDmu : LHCb::ProtoParticle::CombDLLmu
 * - head_PIDK : LHCb::ProtoParticle::CombDLLk
 * - head_PIDp : LHCb::ProtoParticle::CombDLLp
 *
 *  @author Jeremie Borel
 *  @date   2007-11-07
 *
 * Add more info:
 * -hasRich:  proto->richPID()!=0
 * -hasCalo:  proto->calo().size()>0
 * -hasMuon:  proto->muonPID()!=0
 * -isMuon:   proto->muonPID->IsMuon()
 *
 *  @date   2010-03-04
 */
class TupleToolPid : public TupleToolBase,
                     virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolPid( const std::string& type,
                const std::string& name,
                const IInterface* parent);

  virtual ~TupleToolPid(){}; ///< Destructor

  StatusCode fill( const LHCb::Particle*,
                   const LHCb::Particle*,
                   const std::string&,
                   Tuples::Tuple& ) override;

};

#endif // JBOREL_TUPLETOOLPID_H

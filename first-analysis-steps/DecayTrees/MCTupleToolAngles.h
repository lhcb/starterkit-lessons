// $Id: MCTupleToolAngles.h,v 1.3 2010-01-26 15:39:25 rlambert Exp $
#ifndef MCTUPLETOOLANGLES_H
#define MCTUPLETOOLANGLES_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IMCParticleTupleTool.h"            // Interface
#include "Math/Boost.h"
#include "GaudiKernel/Vector4DTypes.h"

/** @class MCTupleToolAngles MCTupleToolAngles.h
 *
 *  Fill MC Particle with decay angle in mother frame
 *
 * - head_TRUECosTheta : angle in mother's frame
 *
 * \sa TupleToolAngles, DecayTreeTuple, MCDecayTreeTuple
 *
 *  @author Patrick Koppenburg
 *  @date   2009-01-19
 */

class MCTupleToolAngles : public TupleToolBase,
                          virtual public IMCParticleTupleTool
{

public:

  /// Standard constructor
  MCTupleToolAngles( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  virtual ~MCTupleToolAngles( ); ///< Destructor

  StatusCode fill( const LHCb::MCParticle*
                   , const LHCb::MCParticle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  inline double cosTheta( const Gaudi::LorentzVector& mother,
                          const Gaudi::LorentzVector& mcp )
  {
    ROOT::Math::Boost boost( mother.BoostToCM() );
    const Gaudi::XYZVector boostedParticle = (boost( mcp )).Vect().unit();
    const Gaudi::XYZVector boostedMother = mother.Vect().unit();
    double cosT = boostedParticle.Dot(boostedMother) ;
    if (msgLevel(MSG::VERBOSE)) verbose() <<  mother << " "
                                          <<  mcp << " " << boostedMother
                                          << " " << boostedParticle
                                          << " " << cosT << endmsg ;
    return cosT;
  }

};

#endif // MCTUPLETOOLANGLES_H

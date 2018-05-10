// $Id: TupleToolAngles.h,v 1.5 2010-01-26 15:39:26 rlambert Exp $
#ifndef TUPLETOOLANGLES_H
#define TUPLETOOLANGLES_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Math/Boost.h"
#include "GaudiKernel/Vector4DTypes.h"


/** @class TupleToolAngles TupleToolAngles.h
 *
 *  Fill MC Particle with decay angle in mother frame
 *
 * - head_CosTheta : angle in mother's frame
 * - if WRTMother is false, will calculate angle in frame of top of tree
 *
 * \sa MCTupleToolAngles, DecayTreeTuple, MCDecayTreeTuple
 *
 *  @author Patrick Koppenburg
 *  @date   2009-01-21
 *  @author R. Lambert
 *  @date   2009-09-04
 */

class TupleToolAngles : public TupleToolBase,
                        virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolAngles( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  virtual ~TupleToolAngles( ); ///< Destructor

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
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

  const LHCb::Particle* findMother( const LHCb::Particle* top
                                    , const LHCb::Particle* P ) const;

private:

  bool m_wrtMother;

};

#endif // TUPLETOOLANGLES_H

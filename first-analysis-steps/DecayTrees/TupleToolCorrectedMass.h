#ifndef TUPLETOOLCORRECTEDMASS_H
#define TUPLETOOLCORRECTEDMASS_H 

// Include files
// from Gaudi
#include <Kernel/IDVAlgorithm.h>
#include <Kernel/GetIDVAlgorithm.h>
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "TVector3.h"

/** @class TupleToolCorrectedMass TupleToolCorrectedMass.h
 *
 * \brief Fill corrected mass (m_corr = sqrt( m_vis² + pT² ) + pT) and error on corrected mass
 *
 * - head_MCORR: corrected mass
 * - head_MCORRERR : error on corrected mass
 * - head_MCORRVERTEXERR: error on corrected mass, only taking the uncertainties on the vertices into account
 * 
 * \sa DecayTreeTuple
 *
 *  @author Michel De Cian (based on Will Sutcliffe's version)
 *  @date   2018-03-21
 */

class TupleToolCorrectedMass : public TupleToolBase, virtual public IParticleTupleTool {
  
 public:
  
  /// Standard constructor
  TupleToolCorrectedMass( const std::string& type,
			  const std::string& name,
			  const IInterface* parent);
  
  virtual StatusCode initialize() override;
  
  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;
  
 private:
  
  double mCorr(const Gaudi::LorentzVector p, const TVector3 flightDir) const;
  double dPTdx(const double dAdx, const double dBdx, const double PT, const double A, const double B) const;
  std::pair<double, double> mCorrErrors(const TVector3 sv, const TVector3 pv, const Gaudi::LorentzVector p,  
					const Gaudi::SymMatrix7x7 covP, const Gaudi::SymMatrix3x3 covPV ) const;
  
  IDVAlgorithm* m_dva;
  
};

#endif

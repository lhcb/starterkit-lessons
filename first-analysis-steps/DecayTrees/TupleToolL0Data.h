// $Id: $
#ifndef TUPLETOOLL0DATA_H
#define TUPLETOOLL0DATA_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
// Interfaces
#include "Kernel/IEventTupleTool.h"
#include "CaloUtils/CaloAlgUtils.h"
#include "L0Interfaces/IL0DUFromRawTool.h"
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0DUEmulatorTool.h"

/** @class TupleToolL0Data TupleToolL0Data.h
 *
 *  - produce tuple branches for L0DU input data extracted from the L0DU bank as defined in EDMS-845277
 *   (see $L0EVENTROOT/EVENT/L0DUBase.h for data naming)
 *  - produce the emulated L0 decision for the TCK list given in option
 *
 *  - Properties :
 *      * 'TCKList'  : list of registered TCK's for which the emulated decision is requested (vector<string>)
 *      * 'DataList' : list of L0data to be tupled (vector<string>)
 *                    -> wildcard are allowed
 *                    -> e.g.   ["*Et*","*Pt*"] to tuple the L0 data with name containing 'Et' or 'Pt'
 *                    -> e.g.   ["ALL","!*Status*"] to tuple ALL L0 data except those with name containing 'Status'
 *
 *                    -> Default : ALL L0 data used for the standard decision making
 *                               -> i.e. 'technical' data related to candidate address, status or intermediate value are rejected
 *
 *
 *      * 'ScaledData' : scale the candidate pT/eT data in MeV  (boolean - default = false, i.e. no scaling)
 *                     -> warning : the scaling factor relies on condDB for L0Calo and is partially hardcoded in L0DU s/w for L0Muon.
 *                     -> the scaling has changed in July 2015.
 *
 *  @author Olivier Deschamps
 *  @date   2011-06-08
 */
class TupleToolL0Data : public TupleToolBase,
                        virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolL0Data( const std::string& type,
                   const std::string& name,
                   const IInterface* parent );

  virtual ~TupleToolL0Data( ); ///< Destructor
  StatusCode fill(Tuples::Tuple& ) override;
  StatusCode initialize() override;

private:

  std::string rename(const std::string& name);

private:

  std::vector<std::string> m_list;
  IL0DUFromRawTool* m_l0;
  IL0DUConfigProvider* m_l0conf;
  IL0DUEmulatorTool* m_l0emu;
  bool m_scale;
  std::vector<std::string> m_tcks;

};

#endif // TUPLETOOLL0DATA_H

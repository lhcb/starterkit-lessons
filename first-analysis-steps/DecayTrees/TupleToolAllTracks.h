#ifndef TUPLETOOLAllTracks_H
#define TUPLETOOLAllTracks_H

// from Gaudi
#include "Kernel/IParticleTupleTool.h"
#include "TrackInterfaces/ITrackVertexer.h"
//from LHCb
#include "Event/Vertex.h"//includes Gaudi::LorentzVector
//from Analysis
#include "DecayTreeTupleBase/TupleToolBase.h"


/**

  @class  TupleToolAllTracks TupleToolAllTracks.h
  @brief  Isolation tool based on TupleToolVtxIsoln by Mitesh Patel, Patrick Koppenburg, using ideas from TupleToolSLTools
    The idea is to add particles, passing a selection that is set by configurable parameters,
    from given TES location(s) to a (configurable) target particle in the decay chain
    and provide information that can be used to isolate the specific decay,
    or to simply provide the information needed to study a new exclusive decay chain.

  In a pyton options-file, use e.g.:
  @code
    from Configurables import TupleToolAllTracks
    AllTracks=TupleToolAllTracks("AllTracks")
    locations = []
    loations.append('Phys/StdAllNoPIDsPions/Particles')
    locations.append("Phys/StdLoosePi02gg/Particles")
    locations.append("Phys/StdLooseAllPhotons/Particles")
    AllTracks.VertexMode = 0
    AllTracks.ANNPIDCut = 0.3 #ProbNNPID cut on input particle of type PID
    AllTracks.GhostProb = 0.5
    AllTracks.Theta = 0.012 #Cut on theta angle (less than) of the added input particle
    AllTracks.DeltaPhi = 0.005 #Cut on the difference between phi angles (abs value less than) of target and added particle
    AllTracks.NewVertexChi2 = 6
    AllTracks.MHi = 7000
    AllTracks.ImprovedVertex = 4 #new vertex chi^2 / ndof should not be worse than 4
    AllTracks.PVIPchi2 = 25
    AllTracks.CorrectedMass = True
    AllTracks.InputParticles = locations
    AllTracks.Target = Lambda_c+ #has to be defined in decay descriptor
  @endcode
  then add the tool to the head(!) of your chain.
  If a cut is not explicitly given in the config file, no cut will be made.

  The VertexMode option takes values 0,1,2 describing the following scenarios:
    - the new particle should be added to the target's mother (e.g. Lb -> (Sc->Lc {pi}) mu nu, where Sc is strongly decaying to the target and a pion we want to add)
    - the new particle should be added directly to the target decay vertex  (e.g. {psi(2S)?} -> (J/psi -> mu mu) {pi} {pi}, where the two pions are directly added to the dimuon vertex, because of the short J/psi lifetime)
    - the new particle and the target form a new vertex (e.g. {B?} -> D0 {pi}, where D0 is target, pi is added particle. Since the D0 decay vertex will be upstream of the B vertex due to it's lifetime, we need to make a new vertex)

  For a preselection of the added particles, one can use e.g. a FilterDesktop:
  @code
    pionFilter = FilterDesktop("PionFilter")
    pionFilter.Code = "(P > 2.*GeV) & (P < 200.*GeV)" \
                      "& (PT < 10.*GeV)" \
                      "& (TRCHI2DOF < 3)"
    SelStdNoPIDsPions = DataOnDemand(Location='Phys/StdAllNoPIDsPions/Particles')

    PifromB = Selection("SelPifromB",
                        Algorithm  = pionFilter,
                        RequiredSelections = [SelStdNoPIDsPions])
    SeqPifromB = SelectionSequence("SeqPifromB", TopSelection = PifromB)
  @endcode
  and pass the outputlocation to the tool (if locations is your list of TES locations)
  @code
    locations.append(SeqPifromB.outputLocation())
  @endcode

  @note
    The calculation of the corrected mass and it's error has not been cross-checked with existing algorithms yet. Use at own risk!

  @date 2015-10-08
  @version v1r1 2016-06-08 : implemented VertexMode & possibility to add particles to head
  @author Greg Ciezarek, Marco Fiore, Marian Stahl

**/

struct IDVAlgorithm;
struct IDistanceCalculator;
struct IVertexFit;
struct IPVReFitter;
struct IParticleDescendants;

using namespace LHCb;

class TupleToolAllTracks : public TupleToolBase, virtual public IParticleTupleTool {
public:
  // Standard constructor
  TupleToolAllTracks( const std::string& type, const std::string& name, const IInterface* parent);
  virtual ~TupleToolAllTracks( ){}
  StatusCode initialize() override;
  StatusCode fill(const Particle* , const Particle* , const std::string&  , Tuples::Tuple& ) override;

private:

  //Tools
  IDVAlgorithm         *m_dva;
  IDistanceCalculator  *m_dist;
  const IVertexFit     *m_pVertexFit;
  IPVReFitter          *m_pvReFitter;
  std::string           m_typeVertexFit;
  IParticleDescendants *m_particleDescendants;

  //functions
  bool                                         isTrackInDecay(const Particle *particle, const Particle *mother);
  std::pair<float,std::unique_ptr<RecVertex> > getminipchi   (const Particle *particle);
  std::vector<float>                           getcorrm      (const RecVertex *PV, const Vertex *SV, const Gaudi::LorentzVector &new_mother);

  //member variables
  std::vector<std::string> m_InputParticles;
  std::string              m_TargetParticle;
  int                      m_VModedummy;
  enum class VMode{ToMother = 0, ToTarget, NewVertex};
  VMode                    m_VertexMode;
  float                    m_ANNPIDCut;
  float                    m_theta;
  float                    m_deltaphi;
  float                    m_GhostProb;
  float                    m_DLLPIDCut;
  float                    m_VertexChi2Cut;
  float                    m_ipchi2Cut;
  float                    m_PVipchi2Cut;
  float                    m_improvedVertex;
  float                    m_HiMassCut;
  unsigned long            m_mult;
  bool                     m_addpxpypz;
  bool                     m_addcorrm;
};
#endif // TUPLETOOLAllTracks_H

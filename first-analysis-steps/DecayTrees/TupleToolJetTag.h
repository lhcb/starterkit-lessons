// $Id: TupleToolJetTagTag.h,v 1.1 2011-05-16 14:14:14 aphan Exp $
#ifndef TUPLETOOLJETTAG_H
#define TUPLETOOLJETTAG_H 1

#include "Kernel/IJetTagTool.h"
#include "Kernel/IParticleTupleTool.h"
#include "TupleToolJetsBase.h"

class TupleToolJetTag : public TupleToolJetsBase,
                        virtual public IParticleTupleTool
{

public:

  TupleToolJetTag(const std::string& type,
                  const std::string& name,
                  const IInterface* parent);

  virtual ~TupleToolJetTag(){};

  StatusCode initialize() override;

  StatusCode fill(const LHCb::Particle*,
                  const LHCb::Particle*,
                  const std::string&,
                  Tuples::Tuple&) override;

private:

  // label for ntuple
  std::string m_tagToolLabel;

  // name to retrieve tool
  std::string m_tagToolName;

protected:

  IJetTagTool* m_TagTool;
  bool m_verboseTag;   ///< get details on Tag

};

#endif // TUPLETOOLJET_H

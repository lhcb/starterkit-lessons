{% objectives "Learning Objectives" %}

* Learn the basic structure of a decfile.

{% endobjectives %}

## Controlling the decay: DecFiles

The `DecFile` controls the decay itself (i.e. what `EvtGen` does) as well as provide any event-type specific configuration (e.g. generator cuts).
They exist in the `Gen/DecFile` package which we have already checked out and built in the beginning and are given as `.dec` files in `Gen/DecFiles/dkfiles`.
For example, the `.dec` file for event-type `27175000` is `Dst_D0pi,KKmumu=DecProdCut.dec`:
```bash
# EventType: 27175000                                                                                                                                                                                                                                      
#                                                                                                                                                                                                                                                          
# Descriptor: [D*(2010)+ -> (D0 -> {K+ K- mu+ mu-}) pi+]cc                                                                                                                                                                                                 
#                                                                                                                                                                                                                                                          
# NickName: Dst_D0pi,KKmumu=DecProdCut                                                                                                                                                                                                                     
#                                                                                                                                                                                                                                                          
# Cuts: DaughtersInLHCb                                                                                                                                                                                                                                    
#                                                                                                                                                                                                                                                          
# Documentation: Forces the D* decay in generic b-bbar / c-cbar events + Requires products to be in LHCb acceptance                                                                                                                                        
# EndDocumentation                                                                                                                                                                                                                                         
#                                                                                                                                                                                                                                                          
# PhysicsWG: Charm                                                                                                                                                                                                                                         
# Tested: Yes                                                                                                                                                                                                                                              
# Responsible: Luisa Arrabito                                                                                                                                                                                                                              
# Email: unknown@<nospam>cern.ch                                                                                                                                                                                                                           
# Date: 20091215                                                                                                                                                                                                                                           
#                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                           
Alias MyD0 D0                                                                                                                                                                                                                                              
Alias MyantiD0 anti-D0                                                                                                                                                                                                                                     
ChargeConj MyD0 MyantiD0                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                           
Decay D*+sig                                                                                                                                                                                                                                               
  1.000 MyD0  pi+    VSS;                                                                                                                                                                                                                                  
Enddecay                                                                                                                                                                                                                                                   
CDecay D*-sig                                                                                                                                                                                                                                              
                                                                                                                                                                                                                                                           
Decay MyD0                                                                                                                                                                                                                                                 
  1.000 K+ K- mu+ mu- PHSP;                                                                                                                                                                                                                                
Enddecay                                                                                                                                                                                                                                                   
CDecay MyantiD0                                                                                                                                                                                                                                            
#                                                                                                                                                                                                                                                          
End  
```
The commented actually contains vital information and is parsed during the compilation to create the `<event-type>.py` file that is given to `Gauss` as one of its options!
The EventType is a series of flags which controls the generation. The rules for this are described in detail in [LHCb-2005-034](https://cds.cern.ch/record/855452/files/lhcb-2005-034.pdf) and also a [website](http://lbeventtype.web.cern.ch/) that allows you to build/parse event-types.
For example for the first digit: 1 = contains b quark, 2 = c quark, 3 = min bias ...

Similarly, the document specifies the conventions for the "NickName" - which also has to be the filename. Note that once MC has been produced from a given DecFile, it is not allowed to be changed, so you never need to worry about which version of DecFiles you are looking at when trying to understand existing samples.
The "Cuts" field specifies which one of a predetermined set of cut tools are used. Additional python code can be added to the resulting configuration file:

```bash
# InsertPythonCode:
# code ...
# EndInsertPythonCode
```

The bottom part of the decay file specifies the decay itself:
This DecFile defines a signal `D*+` which decays 100% to `D0` `pi+`, and the D0 in turn decays 100% into `K+`, `K-`, `mu+` and `mu-`. Important is the definition of "MyD0". If the decay was to "D0" rather than "MyD0", the D0 would decay via all of the decay modes implemented in DECAY.DEC.
The final part of each decay is the actual physics model used - in this case "VSS", the generic matrix element for vector to scaler-scaler transitions, and "PHSP", which is phase space only (matrix element = constant). Note that with PHSP, the decay products are completely unpolarized - for anything other than (spin0) to (spin0 spin0) this will get the angular momentum wrong!

When using pre-existing decay files, it is important to check them closely for consistency first. Many Decay files are created for a specific physics use case, and some may even get physics wrong when it comes to e.g. the relative frequencies of intermediate states in a cocktail.

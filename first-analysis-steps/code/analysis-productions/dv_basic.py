import Functors as F
import FunTuple.functorcollections as FC
from DaVinci import Options, make_config
from DaVinci.algorithms import create_lines_filter
from FunTuple import FunctorCollection
from FunTuple import FunTuple_Particles as Funtuple
from PyConf.reading import get_particles, get_pvs
from RecoConf.event_filters import require_pvs


def main(options: Options):
    line = "Hlt2B2CC_BuToJpsiKplus_JpsiToMuMu_Detached"
    data = get_particles(f"/Event/HLT2/{line}/Particles")
    line_prefilter = create_lines_filter(name=f"PreFilter_{line}", lines=[line])
    pvs = get_pvs()

    fields = {
        "Bplus": "[B+ -> (J/psi(1S) -> mu+ mu-) K+]CC",
        "Jpsi": "[B+ -> ^(J/psi(1S) -> mu+ mu-) K+]CC",
        "muplus": "[B+ -> (J/psi(1S) -> ^mu+ mu-) K+]CC",
        "muminus": "[B+ -> (J/psi(1S) -> mu+ ^mu-) K+]CC",
        "Kplus": "[B+ -> (J/psi(1S) -> mu+ mu-) ^K+]CC",
    }

    all_vars = FunctorCollection({
        "M": F.MASS,
        "P": F.P,
        "PT": F.PT
    })

    variables = {"ALL": all_vars}

    funtuple = Funtuple(
        name=line,
        tuple_name="DecayTree",
        fields=fields,
        variables=variables,
        inputs=data,
    )

    algs = {line: [line_prefilter, require_pvs(pvs), funtuple]}
    return make_config(options, algs)


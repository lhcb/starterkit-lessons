j = Job(application=DaVinci(version='v41r2'))
j.backend = Dirac()
j.name = 'First ganga job'
j.inputdata = j.application.readInputData((
    'data/'
    "MC_2016_27163002_"
    "Beam6500GeV2016MagDownNu1.625nsPythia8_Sim09b_"
    "Trig0x6138160F_Reco16_Turbo03_"
    "Stripping28NoPrescalingFlagged_ALLSTREAMS.DST.py"
))
j.application.optsfile = 'code/davinci-grid/ntuple_options_grid.py'
j.submit()

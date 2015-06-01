j = Job(application=DaVinci(version='v36r6'))
j.backend = Dirac()
j.name = 'First ganga job'
j.inputdata = j.application.readInputData('data/MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py')
j.application.optsfile = 'code/11-davinci-grid/ntuple_options_grid.py'
j.splitter = SplitByFiles(filesPerJob=1)
j.submit()

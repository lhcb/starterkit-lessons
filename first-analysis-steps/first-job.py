j = Job(name='First ganga job')
myApp = GaudiExec()
myApp.directory = "./DaVinciDev_v45r6"
j.application = myApp
j.application.options = ['ntuple-options.py']
j.application.platform = 'x86_64-centos7-gcc8-opt'
bkPath = '/MC/2016/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/27163002/ALLSTREAMS.DST'
data  = BKQuery(bkPath, dqflag=['OK']).getDataset()
j.inputdata = data[0:2]
j.backend = Dirac()
j.splitter = SplitByFiles(filesPerJob=1)
j.outputfiles = [LocalFile('DVntuple.root')]
j.submit()

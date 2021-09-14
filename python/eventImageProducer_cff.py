import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

# Create a new CMS process
process = cms.Process("EventImageProducer")

inputFiles = [
    'root://cmsxrootd.fnal.gov//store/mc/RunIISummer19UL18MiniAOD/GluGluToBulkGravitonToHHTo4B_M-1000_narrow_WZHtag_TuneCP5_PSWeights_13TeV-madgraph-pythia8/MINIAODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/20000/86AAB896-106A-4D4E-9657-DDB87FAFD1EE.root'
]

# Command line arguments
options = VarParsing.VarParsing('analysis')
options.inputFiles = inputFiles
options.maxEvents = 1000
options.parseArguments()

# Initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))

# The input file
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(options.inputFiles)
                            )

process.eventImageProducer = cms.EDProducer("EventImageProducer",
                                    pfCands=cms.InputTag("packedPFCandidates"),
                                )

outfile = 'test.root'
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(outfile) )

process.p = cms.Path(process.eventImageProducer)
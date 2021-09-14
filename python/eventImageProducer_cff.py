import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from helpers import short_name

# Create a new CMS process
process = cms.Process("EventImageProducer")

inputFiles = [
    'root://cmsxrootd.fnal.gov//store/mc/RunIISummer20UL17MiniAOD/VBF_HToInvisible_M125_TuneCP5_withDipoleRecoil_13TeV_powheg_pythia8/MINIAODSIM/106X_mc2017_realistic_v6-v1/00000/7315ADC9-D859-AB48-85F0-052CF3B25769.root'
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

outfile = '{}.root'.format( short_name(inputFiles[0]) )

process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(outfile) )

process.p = cms.Path(process.eventImageProducer)
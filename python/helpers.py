import re

def short_name(filename):
    dataset = filename.split('/')[-5]
    if 'RunIISummer20UL17' in filename:
        year = 2017
    elif 'RunIISummer20UL18' in filename:
        year = 2018
    else:
        raise RuntimeError('Could not determine year.')

    dataset = re.sub('powheg', 'pow', dataset)
    dataset = re.sub('(-|_)pythia8', '', dataset)
    dataset = re.sub('_withDipoleRecoil', '', dataset)
    dataset = re.sub('_MatchEWPDG20', '', dataset)
    dataset = re.sub('_TuneCP5', '', dataset)
    dataset = re.sub('amcatnloFXFX', 'FXFX', dataset)

    return '{}_{}'.format(dataset, year)


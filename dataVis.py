import ROOT

file = ROOT.TFile("build/output.root")

#Histograms
alpha_spectrum = file.hAlpha
lAr_spectrum = file.hEnergy

#Creating the histogram from a TTree
tree = file.alpha
hPhGenFromTuple = ROOT.TH1I("hPhDetFromTuple", "Photons Detected per alpha", 200, 0, 5000)

for alpha in tree:
    photonsDetected = alpha.photonsDetected
    hPhGenFromTuple.Fill(photonsDetected)

#Tree of NTuples
tree = file.alpha

#Editing histograms
alpha_spectrum.GetXaxis().SetTitle("Energy (MeV)")
alpha_spectrum.GetYaxis().SetTitle("#")

lAr_spectrum.GetXaxis().SetTitle("Energy (eV)")
lAr_spectrum.GetYaxis().SetTitle("#")

hPhGenFromTuple.GetXaxis().SetTitle("Photons/Alpha")
hPhGenFromTuple.GetYaxis().SetTitle("#")

#Setup of the Canvas

c1 = ROOT.TCanvas("c1", "Histograms", 1)
#c1.Divide(3,1)

#c1.cd(1)
#alpha_spectrum.Draw("HIST")
#c1.cd(2)
#lAr_spectrum.Draw("HIST")
#c1.cd(3)
hPhGenFromTuple.Draw("HIST")
c1.Draw()
    


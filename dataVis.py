import ROOT

file = ROOT.TFile("build/output.root")

#Histograms

alpha_spectrum = file.hAlpha
lAr_spectrum = file.hEnergy
photons_per_alpha = file.hPhotonsAlpha

#Editing histograms
alpha_spectrum.GetXaxis().SetTitle("Energy (MeV)")
alpha_spectrum.GetYaxis().SetTitle("#")

lAr_spectrum.GetXaxis().SetTitle("Energy (eV)")
lAr_spectrum.GetYaxis().SetTitle("#")

photons_per_alpha.GetXaxis().SetTitle("Photons/Alpha")
photons_per_alpha.GetYaxis().SetTitle("#")

#Setup of the Canvas

c1 = ROOT.TCanvas("c1", "Histograms", 1)
c1.Divide(3,1)

c1.cd(1)
alpha_spectrum.Draw("HIST")
c1.cd(2)
lAr_spectrum.Draw("HIST")
c1.cd(3)
photons_per_alpha.Draw("HIST")
c1.Draw()
    
input()

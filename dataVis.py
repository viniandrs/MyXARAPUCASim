import ROOT

file = ROOT.TFile("build/final_output.root")

#Creating the histogram from a TTree
tree = file.alpha
hPhGenFromTuple = ROOT.TH1I("hPhDetFromTuple", "Photons Detected per alpha", 200, 0, 5000)

for alpha in tree:
    photonsDetected = alpha.photonsDetected
    hPhGenFromTuple.Fill(photonsDetected)

#Editing histograms
hPhGenFromTuple.GetXaxis().SetTitle("Photons/Alpha")
hPhGenFromTuple.GetYaxis().SetTitle("#")

#Setup of the Canvas

c1 = ROOT.TCanvas("c1", "Histograms", 1)
hPhGenFromTuple.Draw("HIST")
c1.Draw()
    


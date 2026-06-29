// analysis.C
// Run after simulation: root -l analysis.C
// Reads muon_data.csv produced by EventAction
// Plots: (1) angular distribution, (2) efficiency vs angle

void analysis() {

    // Read simulation output
    const char* fname = "muon_data.csv";
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Error: muon_data.csv not found. Run simulation first.\n");
        return;
    }

    // Skip header
    char line[256];
    fgets(line, sizeof(line), f);

    // Angle bins: 0-90 degrees in 9 x 10-degree bins
    const int NBINS = 9;
    G4double binEdges[NBINS+1];
    for (int i = 0; i <= NBINS; i++) binEdges[i] = i * 10.0;

    TH1F* hTotal    = new TH1F("hTotal",    "Total muons per angle bin",    NBINS, binEdges);
    TH1F* hDetected = new TH1F("hDetected", "Detected muons per angle bin", NBINS, binEdges);
    TH1F* hEdep     = new TH1F("hEdep",     "Energy deposition in scintillator;E_{dep} [MeV];Counts",
                                100, 0, 20);
    TH1F* hAngle    = new TH1F("hAngle",    "Muon angular distribution (cos^{2} #theta);#theta [deg];Counts",
                                NBINS, binEdges);

    double theta, edep;
    int detected;
    int total = 0;

    while (fscanf(f, "%lf,%lf,%d\n", &theta, &edep, &detected) == 3) {
        hTotal->Fill(theta);
        hAngle->Fill(theta);
        hEdep->Fill(edep);
        if (detected) hDetected->Fill(theta);
        total++;
    }
    fclose(f);

    printf("Total events read: %d\n", total);

    // Efficiency histogram
    TH1F* hEff = (TH1F*) hDetected->Clone("hEff");
    hEff->SetTitle("Detection efficiency vs incidence angle;#theta [deg];Efficiency");
    hEff->Divide(hTotal);

    // Plotting
    gStyle->SetOptStat(0);
    gStyle->SetPadGridY(1);

    TCanvas* c1 = new TCanvas("c1", "Muon Detector Analysis", 1200, 400);
    c1->Divide(3, 1);

    // Panel 1: angular distribution
    c1->cd(1);
    hAngle->SetLineColor(kBlue+1);
    hAngle->SetLineWidth(2);
    hAngle->SetFillColorAlpha(kBlue+1, 0.15);
    hAngle->Draw("HIST");

    // Overlay cos^2 expectation
    TF1* fCos2 = new TF1("fCos2",
        Form("[0]*cos(x*TMath::Pi()/180)*cos(x*TMath::Pi()/180)*TMath::Pi()/180*10"),
        0, 90);
    fCos2->SetParameter(0, hAngle->Integral() * hAngle->GetBinWidth(1));
    fCos2->SetLineColor(kRed);
    fCos2->SetLineWidth(2);
    fCos2->Draw("SAME");

    TLegend* leg1 = new TLegend(0.45, 0.7, 0.88, 0.88);
    leg1->AddEntry(hAngle,  "Simulated", "f");
    leg1->AddEntry(fCos2,   "cos^{2}#theta", "l");
    leg1->SetBorderSize(0);
    leg1->Draw();

    // Panel 2: energy deposition
    c1->cd(2);
    hEdep->SetLineColor(kGreen+2);
    hEdep->SetLineWidth(2);
    hEdep->SetFillColorAlpha(kGreen+2, 0.15);
    hEdep->Draw("HIST");

    // Panel 3: efficiency vs angle
    c1->cd(3);
    hEff->SetLineColor(kRed+1);
    hEff->SetLineWidth(2);
    hEff->SetMarkerStyle(20);
    hEff->SetMarkerColor(kRed+1);
    hEff->GetYaxis()->SetRangeUser(0, 1.1);
    hEff->Draw("E1");

    c1->SaveAs("muon_efficiency.png");
    printf("Saved: muon_efficiency.png\n");
}

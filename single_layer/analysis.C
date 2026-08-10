void analysis() {

    const char* fname = "muon_data_thr0.500000.csv";
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Error: %s not found. Run ./muonSim first (default threshold = 0.5 MeV).\n", fname);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), f);

    const int NBINS = 9;
    double binEdges[NBINS+1];
    for (int i = 0; i <= NBINS; i++) binEdges[i] = i * 10.0;

    TH1F* hTotal    = new TH1F("hTotal",    "Total muons per angle bin",    NBINS, binEdges);
    TH1F* hDetected = new TH1F("hDetected", "Detected muons per angle bin", NBINS, binEdges);
    TH1F* hEdep     = new TH1F("hEdep",     "Energy deposition in scintillator;E_{dep} [MeV];Counts",
                                100, 0, 20);
    TH1F* hAngle    = new TH1F("hAngle",    "Muon angular distribution (cos^{2} #theta);#theta [deg];Counts",
                                NBINS, binEdges);

    double theta, edep;
    int detected, hit;
    int total = 0;

    while (fscanf(f, "%lf,%lf,%d,%d\n", &theta, &edep, &detected, &hit) == 4) {
        hTotal->Fill(theta);
        hAngle->Fill(theta);
        hEdep->Fill(edep);
        if (detected) hDetected->Fill(theta);
        total++;
    }
    fclose(f);

    printf("Total events read: %d\n", total);

    TH1F* hEff = new TH1F("hEff", "Detection efficiency vs incidence angle;#theta [deg];Efficiency",
                           NBINS, binEdges);
    hEff->Divide(hDetected, hTotal, 1, 1, "B");

    gStyle->SetOptStat(0);
    gStyle->SetPadGridY(1);

    TCanvas* c1 = new TCanvas("c1", "Muon Detector Analysis", 1200, 400);
    c1->Divide(3, 1);

    c1->cd(1);
    hAngle->SetLineColor(kBlue+1);
    hAngle->SetLineWidth(2);
    hAngle->SetFillColorAlpha(kBlue+1, 0.15);
    hAngle->Draw("HIST");

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

    c1->cd(2);
    hEdep->SetLineColor(kGreen+2);
    hEdep->SetLineWidth(2);
    hEdep->SetFillColorAlpha(kGreen+2, 0.15);
    hEdep->Draw("HIST");

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

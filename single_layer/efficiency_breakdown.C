void efficiency_breakdown() {

    const char* fname = "muon_data_thr0.500000.csv";
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Error: %s not found. Run ./muonSim 0.5 first.\n", fname);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), f);

    const int NBINS = 9;
    double binEdges[NBINS+1];
    for (int i = 0; i <= NBINS; i++) binEdges[i] = i * 10.0;

    TH1F* hTotal    = new TH1F("hTotal2",    "", NBINS, binEdges);
    TH1F* hHit      = new TH1F("hHit2",      "", NBINS, binEdges);
    TH1F* hDetected = new TH1F("hDetected2", "", NBINS, binEdges);

    double theta, edep;
    int detected, hit;
    int total = 0;

    while (fscanf(f, "%lf,%lf,%d,%d\n", &theta, &edep, &detected, &hit) == 4) {
        hTotal->Fill(theta);
        if (hit)      hHit->Fill(theta);
        if (detected) hDetected->Fill(theta);
        total++;
    }
    fclose(f);
    printf("Total events read: %d\n", total);

    TH1F* hAccept = new TH1F("hAccept",
        "Geometric acceptance vs angle;#theta [deg];Acceptance (hit / generated)",
        NBINS, binEdges);
    hAccept->Divide(hHit, hTotal, 1, 1, "B");

    TH1F* hIntrinsic = new TH1F("hIntrinsic",
        "Intrinsic detection efficiency vs angle;#theta [deg];Efficiency (detected / hit)",
        NBINS, binEdges);
    hIntrinsic->Divide(hDetected, hHit, 1, 1, "B");

    gStyle->SetOptStat(0);
    gStyle->SetPadGridY(1);

    TCanvas* c2 = new TCanvas("c2", "Efficiency Breakdown", 900, 400);
    c2->Divide(2, 1);

    c2->cd(1);
    hAccept->SetLineColor(kAzure+2);
    hAccept->SetMarkerColor(kAzure+2);
    hAccept->SetMarkerStyle(20);
    hAccept->GetYaxis()->SetRangeUser(0, 1.1);
    hAccept->Draw("E1");

    c2->cd(2);
    hIntrinsic->SetLineColor(kOrange+7);
    hIntrinsic->SetMarkerColor(kOrange+7);
    hIntrinsic->SetMarkerStyle(20);
    hIntrinsic->GetYaxis()->SetRangeUser(0, 1.1);
    hIntrinsic->Draw("E1");

    c2->SaveAs("efficiency_breakdown.png");
    printf("Saved: efficiency_breakdown.png\n");
}

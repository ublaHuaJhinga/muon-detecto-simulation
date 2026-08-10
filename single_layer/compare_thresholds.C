void compare_thresholds() {

    const int NBINS = 9;
    double binEdges[NBINS+1];
    for (int i = 0; i <= NBINS; i++) binEdges[i] = i * 10.0;

    std::vector<std::string> fnames = {
        "muon_data_thr0.200000.csv",
        "muon_data_thr0.500000.csv",
        "muon_data_thr1.000000.csv"
    };
    std::vector<std::string> labels = { "0.2 MeV", "0.5 MeV", "1.0 MeV" };
    int colors[3] = { kBlue+1, kGreen+2, kRed+1 };

    gStyle->SetOptStat(0);
    gStyle->SetPadGridY(1);

    TCanvas* c = new TCanvas("c_thr", "Intrinsic Efficiency vs Angle by Threshold", 700, 500);
    TLegend* leg = new TLegend(0.62, 0.65, 0.88, 0.88);
    leg->SetBorderSize(0);

    TH1F* firstDrawn = nullptr;

    for (int i = 0; i < (int)fnames.size(); i++) {
        FILE* f = fopen(fnames[i].c_str(), "r");
        if (!f) {
            printf("Warning: %s not found, skipping.\n", fnames[i].c_str());
            continue;
        }

        char line[256];
        fgets(line, sizeof(line), f);

        TH1F* hHit = new TH1F(Form("hHit_%d", i), "", NBINS, binEdges);
        TH1F* hDet = new TH1F(Form("hDet_%d", i), "", NBINS, binEdges);

        double theta, edep;
        int detected, hit;
        int n = 0;
        while (fscanf(f, "%lf,%lf,%d,%d\n", &theta, &edep, &detected, &hit) == 4) {
            if (hit) {
                hHit->Fill(theta);
                if (detected) hDet->Fill(theta);
            }
            n++;
        }
        fclose(f);
        printf("%s: %d events read\n", fnames[i].c_str(), n);

        TH1F* hEff = new TH1F(Form("hEff_%d", i),
            "Intrinsic detection efficiency vs angle, by threshold;#theta [deg];Efficiency (detected / hit)",
            NBINS, binEdges);
        hEff->Divide(hDet, hHit, 1, 1, "B");
        hEff->SetLineColor(colors[i]);
        hEff->SetMarkerColor(colors[i]);
        hEff->SetMarkerStyle(20);
        hEff->GetYaxis()->SetRangeUser(0, 1.1);

        if (!firstDrawn) {
            hEff->Draw("E1");
            firstDrawn = hEff;
        } else {
            hEff->Draw("E1 SAME");
        }

        leg->AddEntry(hEff, labels[i].c_str(), "lep");
    }

    leg->Draw();
    c->SaveAs("efficiency_vs_threshold.png");
    printf("Saved: efficiency_vs_threshold.png\n");
}

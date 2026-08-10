void layer_comparison() {

    const char* fname = "muon_data_multilayer_thr0.500000.csv";
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Error: %s not found. Run ./muonSim 0.5 first.\n", fname);
        return;
    }

    char line[512];
    fgets(line, sizeof(line), f);

    const int NBINS = 9;
    double binEdges[NBINS+1];
    for (int i = 0; i <= NBINS; i++) binEdges[i] = i * 10.0;

    TH1F* hTotal  = new TH1F("hTotal3",  "", NBINS, binEdges);
    TH1F* hDet1   = new TH1F("hDet1",    "", NBINS, binEdges);
    TH1F* hDet2   = new TH1F("hDet2",    "", NBINS, binEdges);
    TH1F* hDet3   = new TH1F("hDet3",    "", NBINS, binEdges);

    double theta, e0, e1, e2;
    int d0, d1, d2;
    int total = 0;

    while (fscanf(f, "%lf,%lf,%lf,%lf,%d,%d,%d\n",
                  &theta, &e0, &e1, &e2, &d0, &d1, &d2) == 7) {
        hTotal->Fill(theta);
        if (d0)                hDet1->Fill(theta);
        if (d0 && d1)          hDet2->Fill(theta);
        if (d0 && d1 && d2)    hDet3->Fill(theta);
        total++;
    }
    fclose(f);
    printf("Total events read: %d\n", total);

    TH1F* hEff1 = new TH1F("hEff1", "Detection efficiency vs angle, by layer requirement;#theta [deg];Efficiency",
                            NBINS, binEdges);
    TH1F* hEff2 = (TH1F*)hEff1->Clone("hEff2");
    TH1F* hEff3 = (TH1F*)hEff1->Clone("hEff3");

    hEff1->Divide(hDet1, hTotal, 1, 1, "B");
    hEff2->Divide(hDet2, hTotal, 1, 1, "B");
    hEff3->Divide(hDet3, hTotal, 1, 1, "B");

    gStyle->SetOptStat(0);
    gStyle->SetPadGridY(1);

    TCanvas* c = new TCanvas("c_layers", "Efficiency by Layer Requirement", 700, 500);

    hEff1->SetLineColor(kBlue+1);   hEff1->SetMarkerColor(kBlue+1);   hEff1->SetMarkerStyle(20);
    hEff2->SetLineColor(kGreen+2);  hEff2->SetMarkerColor(kGreen+2);  hEff2->SetMarkerStyle(21);
    hEff3->SetLineColor(kRed+1);    hEff3->SetMarkerColor(kRed+1);    hEff3->SetMarkerStyle(22);

    hEff1->GetYaxis()->SetRangeUser(0, 1.1);
    hEff1->Draw("E1");
    hEff2->Draw("E1 SAME");
    hEff3->Draw("E1 SAME");

    TLegend* leg = new TLegend(0.6, 0.65, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->AddEntry(hEff1, "1 layer",               "lep");
    leg->AddEntry(hEff2, "2-layer coincidence",    "lep");
    leg->AddEntry(hEff3, "3-layer coincidence",    "lep");
    leg->Draw();

    c->SaveAs("efficiency_by_layers.png");
    printf("Saved: efficiency_by_layers.png\n");

    printf("\nOverall (all angles) event counts:\n");
    printf("  1-layer:  %d / %d = %.3f\n", (int)hDet1->Integral(), total, hDet1->Integral()/total);
    printf("  2-layer:  %d / %d = %.3f\n", (int)hDet2->Integral(), total, hDet2->Integral()/total);
    printf("  3-layer:  %d / %d = %.3f\n", (int)hDet3->Integral(), total, hDet3->Integral()/total);
}

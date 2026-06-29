# Cosmic Muon Detector Simulation

Geant4 simulation of cosmic muon detection in a plastic scintillator slab, with ROOT-based analysis of detection efficiency as a function of incidence angle.

---

## Physics

Cosmic muons at sea level follow an angular distribution proportional to cos²θ, where θ is the zenith angle. A muon passing through a scintillator deposits energy via ionisation. The detector registers a hit when the energy deposit exceeds a threshold (0.5 MeV here). Because muons at larger angles traverse more material, the path length — and therefore energy deposition — varies with θ, making angular efficiency a meaningful diagnostic of detector response.

---

## Detector geometry

- **Material:** plastic scintillator (G4_PLASTIC_SC_VINYLTOLUENE)  
- **Dimensions:** 50 × 50 × 2 cm slab  
- **Threshold:** 0.5 MeV energy deposit  
- **Muon energy:** 4 GeV (typical sea-level mean)  
- **Physics list:** QGSP_BERT  

---

## Results

Detection efficiency vs incidence angle, with angular distribution and energy deposition spectra:

![Muon efficiency analysis](muon_efficiency.png)

The angular distribution reproduces the expected cos²θ profile. Efficiency remains high (>95%) up to ~60° and drops at large angles where the muon path through the thin slab becomes insufficient for reliable detection above threshold.

---

## How to build and run

**Requirements:** Geant4 (≥10.7), ROOT, CMake (≥3.8)

```bash
mkdir build && cd build
cmake ..
make -j4
./muonSim          # produces muon_data.csv
root -l ../analysis.C   # produces muon_efficiency.png
```

---

## File structure

```
├── main.cc                   # Run manager, wires all actions
├── DetectorConstruction.cc/hh   # Scintillator geometry
├── PrimaryGeneratorAction.cc/hh # Cosmic muon source (cos²θ)
├── EventAction.cc/hh            # Per-event data recording
├── SteppingAction.cc/hh         # Energy deposit + angle extraction
├── analysis.C                   # ROOT analysis and plotting
├── run.mac                      # Geant4 macro
└── CMakeLists.txt
```

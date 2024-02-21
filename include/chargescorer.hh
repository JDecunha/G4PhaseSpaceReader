#ifndef chargescorer_h
#define chargescorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class chargescorer : public G4VPrimitiveScorer
{
 public: // with description
      chargescorer(G4String name, G4int depth=0);
      chargescorer(G4String name, const G4String& unit, G4int depth=0);
      virtual ~chargescorer();
      static G4int FindCounts(G4int n);
      static G4int FindNumEnergyBins();
      
  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

      virtual G4double ComputeVolume(G4Step*, G4int idx);

  public:
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

      virtual void SetUnit(const G4String& unit);

  private:

      G4int firstevent;

      G4int HCID;
      G4THitsMap<G4double>* EvtMap;

};
#endif

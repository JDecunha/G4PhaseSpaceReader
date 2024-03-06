#pragma once

//Geant4
#include "globals.hh"
#include "G4VAccumulable.hh"
//STD
#include <vector>

// Not sure if Geant4 is using C++20 features yet. But we could use a concept to restrict the G4VectorAccumulable to only numeric types. 
// template<typename T>
// concept AccumulableType = std::integral<T> || std::floating_point<T>;

template <typename AccumulableType>
class G4VectorAccumulable : public G4VAccumulable
{
	public:
	    G4VectorAccumulable(const G4String& name, const G4int& size) 
	    	: G4VAccumulable(name), _accumulableVector(size, AccumulableType()) //Should default initialize the vector
	    	{ }
	    		 
	    virtual ~G4VectorAccumulable() override {}

	    virtual void Merge(const G4VAccumulable& other) override;
	    virtual void Reset() override;

	    void SetEntry(const G4int& entryNumber, const AccumulableType& entryValue);
	    void AddToEntry(const G4int& entryNumber, const AccumulableType& entryValue);
	    const std::vector<AccumulableType>& GetVector() const { return _accumulableVector; } 

	private:
		std::vector<AccumulableType> _accumulableVector;
};

#include "G4VectorAccumulable.imp"

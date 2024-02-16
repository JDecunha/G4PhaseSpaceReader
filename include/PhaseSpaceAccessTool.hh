//Standard library
#include <mutex>
#include <string>
#include <fstream>

class PhaseSpaceAccessTool //This classes uses the "Monotype" design pattern where all variables are static. Rather than the "singleton" design pattern.
{

public:

	//Default constructor and destructor are sufficient.

	void SetPhaseSpacePath(const std::string&  path);
	std::string GetNextPhaseSpaceEntry();

private:

	static bool _initialized;
	static std::string _phaseSpacePath;
	
    static std::mutex _phaseSpaceMutex;
    static std::ifstream _phaseSpaceFile;

};



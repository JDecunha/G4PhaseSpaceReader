//Standard library
#include <stdexcept>
#include <iostream>
//This project
#include "PhaseSpaceAccessTool.hh"

//Define the private static member variables (one-time definition of private static members is allowed)
bool PhaseSpaceAccessTool::_initialized = false;
std::mutex PhaseSpaceAccessTool::_phaseSpaceMutex;
std::string PhaseSpaceAccessTool::_phaseSpacePath = "";
std::ifstream PhaseSpaceAccessTool::_phaseSpaceFile;

void PhaseSpaceAccessTool::SetPhaseSpacePath(const std::string& path)
{
	std::lock_guard<std::mutex> phaseSpaceLock = std::lock_guard<std::mutex>(_phaseSpaceMutex); //Lock access to the class for thread safety
	std::string dummyVal;

	if (_initialized)
	{
		throw std::runtime_error("Cannot reinitialize phase space file. It has already been initialized.");
	}
	else
	{	
		_phaseSpacePath = path;
		_phaseSpaceFile = std::ifstream(_phaseSpacePath); //set the path to the phase space file
		std::getline(_phaseSpaceFile,dummyVal); //Skip the header line
		_initialized = true;
	}
}

std::string PhaseSpaceAccessTool::GetNextPhaseSpaceEntry()
{
	std::lock_guard<std::mutex> phaseSpaceLock = std::lock_guard<std::mutex>(_phaseSpaceMutex); //Lock access to the class for thread safety

	if (_initialized)
	{
		std::string returnVal{};
		if (std::getline(_phaseSpaceFile,returnVal))
		{
			return returnVal;
		}
		else //Probably means we have reached the end of the phase space
		{	
			_phaseSpaceFile = std::ifstream(_phaseSpacePath); //Reset the phase space to beginning
			std::getline(_phaseSpaceFile,returnVal); //Skip the header line

			if (std::getline(_phaseSpaceFile,returnVal))
			{
				return returnVal;
			}
			else
			{
				throw std::runtime_error("Unknown issue accessing phase space file.");
			}
		}
	}
	else
	{	
		throw std::runtime_error("Cannot access phase space without first initializing path to the file.");
	}
}

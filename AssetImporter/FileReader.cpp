#include "FileReader.h"
#include <iostream>
#include <fstream>

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

std::string FileReader::OpenFile(const std::string& FilePath)
{
	std::string FileString;

	std::ifstream file(FilePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		unsigned int FileSize = file.tellg();
		char* FileMemory = new char[FileSize];
		file.seekg(0, std::ios::beg);
		file.read(FileMemory, FileSize);
		file.close();

		FileString = FileMemory;

		delete[] FileMemory;
	}
	return FileString;
}

std::string FileReader::FindAndReplace(std::string& stringData, const std::string& SearchFor, const std::string& ReplaceWith)
{
	int found = 0;
	while (found = stringData.find(SearchFor, found))
	{
		if (found == std::string::npos)
		{
			return stringData;
		}
		stringData = stringData.replace(found, ReplaceWith.length(), ReplaceWith);
		found += SearchFor.length();
	}
	return stringData;
}

std::string FileReader::SearchSection(const std::string& FileString, const std::string& StartString, const std::string& EndString)
{
	std::size_t Startpos = FileString.find(StartString);
	std::size_t EndPos = FileString.find(EndString) + EndString.size();

	return FileString.substr(Startpos, EndPos - Startpos);
}

std::vector<std::string> FileReader::FindMultipleInSection(const std::string& stringInfo, const std::string StartPointString, const std::string& StartString, const std::string& EndString)
{
	std::vector<std::string> ReturnInfo;

	int StartPos = 0;
	auto NumberOfHits = StringFindCounter(stringInfo, StartPointString);
	for (int x = 0; x < NumberOfHits; x++)
	{
		std::size_t Startpos = stringInfo.find(StartString, StartPos);
		std::size_t EndPos = stringInfo.find(EndString, StartPos) + EndString.size();
		StartPos = EndPos;

		ReturnInfo.emplace_back(stringInfo.substr(Startpos, EndPos - Startpos));
	}

	return ReturnInfo;
}


std::vector<std::string> FileReader::GetSearchString(const std::string& FileString, const std::string& SearchString)
{
	std::vector<std::string> SearchStringFound;

	int SearchCount = StringFindCounter(FileString, SearchString);
	std::string tempstring = SearchString;
	std::string EndString = tempstring.insert(1, "/");
	std::size_t EndPos = 0;
	for (int x = 0; x < SearchCount; x++)
	{
		std::size_t Startpos = FileString.find(SearchString, EndPos + 1);
		EndPos = FileString.find(EndString, EndPos + 1) + EndString.size();

		std::string str3 = FileString.substr(Startpos, EndPos - Startpos);
		SearchStringFound.emplace_back(str3);
	}


	return SearchStringFound;
}

int FileReader::StringFindCounter(const std::string& FileString, const std::string& SearchString)
{
	int FindCounter = 0;
	auto SearchPos = FileString.find(SearchString);
	while (SearchPos != std::string::npos)
	{
		FindCounter++;
		SearchPos = FileString.find(SearchString, SearchPos + 1);
	}
	return FindCounter;
}

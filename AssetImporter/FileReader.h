#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class FileReader
{
private:
public:
	FileReader();
	~FileReader();

	std::string OpenFile(const std::string& FilePath);
	std::string FindAndReplace(std::string& stringData, const std::string& SearchFor, const std::string& ReplaceWith);
	std::string SearchSection(const std::string& FileString, const std::string& StartString, const std::string& EndString);
	std::vector<std::string> FindMultipleInSection(const std::string& stringInfo, const std::string StartPointString, const std::string& StartString, const std::string& EndString);
	std::vector<std::string> GetSearchString(const std::string& FileString, const std::string& SearchString);
	int StringFindCounter(const std::string& FileString, const std::string& SearchString);
};


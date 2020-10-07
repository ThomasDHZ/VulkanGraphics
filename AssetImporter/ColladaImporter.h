#pragma once
#include <iostream>
#include "FileReader.h"

struct GeomatryProperties
{
	std::string Type;
	std::vector<int> IndexList;
};

struct PolyDescriptorStruct
{
	std::string Semantic;
	std::string Source;
	std::string Offset;
	std::string Set;
};
struct PolyList
{
	std::vector<PolyDescriptorStruct> PolyStructList;
	std::vector<int> VertexIndex;
	std::vector<int> NormalIndex;
	std::vector<int> UVIndex;
	std::vector<int> ColorIndex;

	std::vector<int> VertexCount;
	std::vector<int> IndexList;
};
class ColladaImporter
{
private:
	FileReader reader;
	void LoadGeometry(const std::string& FileData);
	
	std::vector<PolyDescriptorStruct> FillOutMeshDescriptors(const std::string& stringInfo);
	GeomatryProperties SetGeoProperitesIndex(const std::vector<int> IndexList, int TotalProperites, int Offset);

	std::vector<std::string> FindTag(const std::vector<std::string>& StringList, const std::string Tag);
	std::vector<PolyList> PolyListInfo(const std::string& stringData);
	std::vector<std::string> GetXMLProperties(const std::string& stringData);
	std::string RemoveXMLInputTag(const std::string& stringData);
	std::string RemoveXMLTag(const std::string& stringData, const std::string& TagName);
	std::vector<std::string> StringListToStringList(const std::string& stringData, char SplitChar);
	std::vector<int> StringListToIntList(const std::string& stringData);
public:
	ColladaImporter();
	ColladaImporter(const std::string& FilePath);
	~ColladaImporter();
};


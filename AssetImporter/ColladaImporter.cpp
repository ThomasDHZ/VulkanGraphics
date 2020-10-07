#include "ColladaImporter.h"
#include <string>
#include <iostream>
#include <sstream>

ColladaImporter::ColladaImporter()
{
}

ColladaImporter::ColladaImporter(const std::string& FilePath)
{
	std::string FileData = reader.OpenFile(FilePath);
	LoadGeometry(FileData);
}

ColladaImporter::~ColladaImporter()
{
}

void ColladaImporter::LoadGeometry(const std::string& FileData)
{
	std::string GeometryData = reader.SearchSection(FileData, "<library_geometries>", "</library_geometries>");
	auto TempGeometryString = reader.FindAndReplace(GeometryData, "\n", " ");
    TempGeometryString = reader.FindAndReplace(TempGeometryString, "\"", " ");
	auto GeometryString = reader.GetSearchString(FileData, "<geometry");
	for (int x = 0; x < GeometryData.size(); x++)
	{
		std::vector<PolyList> meshInfo = PolyListInfo(GeometryString[x]);
	}
	auto a = 34;
}

std::vector<PolyDescriptorStruct> ColladaImporter::FillOutMeshDescriptors(const std::string& stringInfo)
{
	std::vector<PolyDescriptorStruct> descriptor;

	auto XMLMeshDescriptors = GetXMLProperties(stringInfo);
	auto MeshDescriptors = FindTag(XMLMeshDescriptors, "input");

	std::vector<std::string> MeshDescriptorList;
	for (int x = 0; x < MeshDescriptors.size(); x++)
	{
		MeshDescriptorList.emplace_back(RemoveXMLInputTag(MeshDescriptors[x]));
	}

	std::vector<std::vector<std::string>> DescripterFormated;
	for (int x = 0; x < MeshDescriptorList.size(); x++)
	{
		auto temp = StringListToStringList(MeshDescriptorList[x], ' ');
		for (int y = temp.size() - 1; y >= 0; y--)
		{
			if (reader.StringFindCounter(temp[y], "=") == 0)
			{
				temp.erase(temp.begin() + y);
			}
		}
	/*	if (reader.StringFindCounter("=") == 0)
		{
			DescripterFormated.emplace_back(StringListToStringList(MeshDescriptorList[x], ' '));
		}*/
	}

	/*if (VertexInfo)
	{

	}*/

	return std::vector<PolyDescriptorStruct>();
}

GeomatryProperties ColladaImporter::SetGeoProperitesIndex(const std::vector<int> IndexList, int TotalProperites, int Offset)
{
	GeomatryProperties VertexPos;
	for (int x = 0; x < IndexList.size(); x++)
	{
		auto a = x % TotalProperites;
		if (x % TotalProperites == Offset)
		{
			VertexPos.IndexList.emplace_back(IndexList[x]);
		}
	}
	return VertexPos;
}

std::vector<std::string> ColladaImporter::FindTag(const std::vector<std::string>& StringList, const std::string Tag)
{
	std::vector<std::string> FoundList;
	for (int x = 0; x < StringList.size(); x++)
	{
		if (StringList[x].find(Tag) != std::string::npos)
		{
			FoundList.emplace_back(StringList[x]);
		}
	}
	return FoundList;
}

std::vector<PolyList> ColladaImporter::PolyListInfo(const std::string& stringData)
{
	PolyList PolyInfo;

	const std::string StartString = "<polylist";
	const std::string EndString = "/polylist>";
	
	std::size_t Startpos = stringData.find(StartString);
	std::size_t EndPos = stringData.find(EndString, Startpos) + EndString.size();

	std::string PolyListString = stringData.substr(Startpos, EndPos - Startpos);
	PolyListString = reader.FindAndReplace(PolyListString, "\"", " ");

	auto ab = FillOutMeshDescriptors(PolyListString);


	//auto RawMeshIndexCount = RemoveXMLTag(properties[4], "vcount");
	//PolyInfo.VertexCount = StringListToIntList(RawMeshIndexCount);

	//auto GeoProperitesIndex = RemoveXMLTag(properties[5], "p");
	//PolyInfo.IndexList = StringListToIntList(GeoProperitesIndex);

	//GeomatryProperties VertexIndexInfo = SetGeoProperitesIndex(PolyInfo.IndexList, propertiesnoSpace.size(), 0);
	//GeomatryProperties NormalIndexInfo = SetGeoProperitesIndex(PolyInfo.IndexList, propertiesnoSpace.size(), 1);
	//GeomatryProperties UVIndexInfo = SetGeoProperitesIndex(PolyInfo.IndexList, propertiesnoSpace.size(), 2);

	std::vector<std::string> IndexList;

	return std::vector<PolyList>();
}

std::vector<std::string> ColladaImporter::GetXMLProperties(const std::string& stringData)
{
	std::vector<std::string> Properties;

	int StartPos = 0;


	Properties.emplace_back(reader.SearchSection(stringData, "<", ">"));
	auto NumberOfHits = reader.StringFindCounter(stringData, "<input");
	for (int x = 0; x < NumberOfHits; x++)
	{
		std::size_t Startpos = stringData.find("<input", StartPos);
		std::size_t EndPos = stringData.find("/>", StartPos) + 2;
		StartPos = EndPos;

		Properties.emplace_back(stringData.substr(Startpos, EndPos - Startpos));
	}
	Properties.emplace_back(reader.SearchSection(stringData, "<vcount>", "</vcount>"));
	Properties.emplace_back(reader.SearchSection(stringData, "<p>", "</p>"));


	return Properties;
}

std::string ColladaImporter::RemoveXMLInputTag(const std::string& stringData)
{
	const std::string StartString1 = "<input";
	auto TempString = stringData.substr(StartString1.size());


	const std::string EndString2 = "/>";
	std::size_t EndPos = TempString.find(EndString2);

	return TempString.erase(EndPos);
}

std::string ColladaImporter::RemoveXMLTag(const std::string& stringData, const std::string& TagName)
{
	const std::string StartString1 = "<" + TagName;
	const std::string EndString1 = ">";

	std::size_t Startpos = stringData.find(StartString1);
	std::size_t EndPos = stringData.find(EndString1, Startpos) + EndString1.size();

	auto TempString = stringData.substr(EndPos);


	const std::string EndString2 = "</" + TagName;
	EndPos = TempString.find(EndString2);

	return TempString.erase(EndPos);
}

std::vector<std::string> ColladaImporter::StringListToStringList(const std::string& stringData, char SplitChar)
{
	std::vector<std::string> stringList;

	std::string segment;
	std::stringstream ss;
	ss.str(stringData);
	while (std::getline(ss, segment, SplitChar))
	{
		if (segment[0] != '\0')
		{
			stringList.push_back(segment);
		}
	}
	return stringList;
}

std::vector<int> ColladaImporter::StringListToIntList(const std::string& stringData)
{
	std::vector<int> intList;

	std::string segment;
	std::stringstream ss;
	ss.str(stringData);
	while (std::getline(ss, segment, ' '))
	{
		intList.push_back(std::stoi(segment));
	}
	return intList;
}

#include "OBJLoader.h"
#include <string>

#include "Appearance.h"

using namespace std;

bool OBJLoader::FindSimilarVertex(const SimpleVertex& vertex, std::map<SimpleVertex, unsigned short>& vertToIndexMap, unsigned short& index)
{
	auto it = vertToIndexMap.find(vertex);
	
	if (it == vertToIndexMap.end())
	{
		return false;
	}
	else
	{
		index = it->second;
		return true;
	}
}

void OBJLoader::CreateIndices(const vector<XMFLOAT3>& inVertices, 
							  const vector<XMFLOAT2>& inTexCoords, 
							  const vector<XMFLOAT3>& inNormals, 
							  vector<unsigned short>& outIndices, 
							  vector<XMFLOAT3>& outVertices, 
							  vector<XMFLOAT2>& outTexCoords, 
							  vector<XMFLOAT3>& outNormals)
{
	// Mapping from an already-existing SimpleVertex to its corresponding index
	map<SimpleVertex, unsigned short> vertToIndexMap;

	pair<SimpleVertex, unsigned short> pair;

	int numVertices = inVertices.size();
	
	for(int i = 0; i < numVertices; ++i) //For each vertex
	{
		SimpleVertex vertex = {inVertices[i], inNormals[i],  inTexCoords[i]}; 

		unsigned short index;
		// See if a vertex already exists in the buffer that has the same attributes as this one
		bool found = FindSimilarVertex(vertex, vertToIndexMap, index); 
		
		if(found) //if found, re-use it's index for the index buffer
		{
			outIndices.push_back(index);
		}
		else //if not found, add it to the buffer
		{
			outVertices.push_back(vertex.Pos);
			outTexCoords.push_back(vertex.TexC);
			outNormals.push_back(vertex.Normal);
			
			unsigned short newIndex = (unsigned short)outVertices.size() - 1;
			
			outIndices.push_back(newIndex);
			
			//Add it to the map
			pair.first = vertex;
			pair.second = newIndex;
			
			vertToIndexMap.insert(pair);
		}
	}
}


Geometry OBJLoader::Load(char* filename, ID3D11Device* _pd3dDevice, vector<XMFLOAT3> *vertexData, bool invertTexCoords)
{
	string binaryFilename = filename;
	binaryFilename.append("Binary");
	ifstream binaryInFile;
	binaryInFile.open(binaryFilename, std::ios::in | std::ios::binary);

	if(!binaryInFile.good())
	{
		std::ifstream inFile;
		inFile.open(filename);

		if(!inFile.good())
		{
			return Geometry();
		}
		else
		{
			//Vectors to store the vertex positions, normals and texture coordinates. 
			vector<XMFLOAT3> verts;
			vector<XMFLOAT3> normals;
			vector<XMFLOAT2> texCoords;


			vector<unsigned short> vertIndices;
			vector<unsigned short> normalIndices;
			vector<unsigned short> textureIndices;

			string input;

			XMFLOAT3 vert;
			XMFLOAT2 texCoord;
			XMFLOAT3 normal;
			unsigned short vInd[3]; //indices for the vertex position
			unsigned short tInd[3]; //indices for the texture coordinate
			unsigned short nInd[3]; //indices for the normal
			string beforeFirstSlash;
			string afterFirstSlash;
			string afterSecondSlash;

			while(!inFile.eof()) 
			{
				inFile >> input; 

				
				if(input.compare("v") == 0) //Vertex position
				{
					inFile >> vert.x;
					inFile >> vert.y;
					inFile >> vert.z;

					verts.push_back(vert);
				}
				else if(input.compare("vt") == 0) //Texture coordinate
				{
					inFile >> texCoord.x;
					inFile >> texCoord.y;

					if(invertTexCoords) texCoord.y = 1.0f - texCoord.y;

					texCoords.push_back(texCoord);
				}
				else if(input.compare("vn") == 0) //Normal
				{
					inFile >> normal.x;
					inFile >> normal.y;
					inFile >> normal.z;

					normals.push_back(normal);
				}
				else if(input.compare("f") == 0) //Face
				{
					for(int i = 0; i < 3; ++i)
					{
						inFile >> input;
						int slash = input.find("/"); //Find first forward slash
						int secondSlash = input.find("/", slash + 1); //Find second forward slash

						//Extract from string
						beforeFirstSlash = input.substr(0, slash); //The vertex position index
						afterFirstSlash = input.substr(slash + 1, secondSlash - slash - 1); //The texture coordinate index
						afterSecondSlash = input.substr(secondSlash + 1); //The normal index

						//Parse into int
						vInd[i] = (unsigned short)atoi(beforeFirstSlash.c_str()); 
						tInd[i] = (unsigned short)atoi(afterFirstSlash.c_str());
						nInd[i] = (unsigned short)atoi(afterSecondSlash.c_str());
					}

					//Place into vectors
					for(int i = 0; i < 3; ++i)
					{
						vertIndices.push_back(vInd[i] - 1);		
						textureIndices.push_back(tInd[i] - 1);	
						normalIndices.push_back(nInd[i] - 1);	
					}
				}
			}
			inFile.close(); //Finished with input file now, all the data we need has now been loaded in
			
			for (unsigned int i = 0; i < verts.size(); i++)
			{
				vertexData->push_back(verts[i]);
			}
			//Get vectors to be of same size, ready for singular indexing
			vector<XMFLOAT3> expandedVertices;
			vector<XMFLOAT3> expandedNormals;
			vector<XMFLOAT2> expandedTexCoords;
			unsigned int numIndices = vertIndices.size();
			for(unsigned int i = 0; i < numIndices; i++)
			{
				expandedVertices.push_back(verts[vertIndices[i]]);
				expandedTexCoords.push_back(texCoords[textureIndices[i]]);
				expandedNormals.push_back(normals[normalIndices[i]]);
			}

			//Now to (finally) form the final vertex, texture coord, normal list and single index buffer using the above expanded vectors
			vector<unsigned short> meshIndices;
			meshIndices.reserve(numIndices);
			vector<XMFLOAT3> meshVertices;
			meshVertices.reserve(expandedVertices.size());
			vector<XMFLOAT3> meshNormals;
			meshNormals.reserve(expandedNormals.size());
			vector<XMFLOAT2> meshTexCoords;
			meshTexCoords.reserve(expandedTexCoords.size());

			CreateIndices(expandedVertices, expandedTexCoords, expandedNormals, meshIndices, meshVertices, meshTexCoords, meshNormals);

			Geometry meshData;

			//Turn data from vector form to arrays
			SimpleVertex* finalVerts = new SimpleVertex[meshVertices.size()];
			unsigned int numMeshVertices = meshVertices.size();
			for(unsigned int i = 0; i < numMeshVertices; ++i)
			{
				finalVerts[i].Pos = meshVertices[i];
				finalVerts[i].Normal = meshNormals[i];
				finalVerts[i].TexC = meshTexCoords[i];
			}

			//Put data into vertex and index buffers, then pass the relevant data to the MeshData object.
			ID3D11Buffer* vertexBuffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * meshVertices.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = finalVerts;

			_pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);

			meshData.vertexBuffer = vertexBuffer;
			meshData.vertexBufferOffset = 0;
			meshData.vertexBufferStride = sizeof(SimpleVertex);

			unsigned short* indicesArray = new unsigned short[meshIndices.size()];
			unsigned int numMeshIndices = meshIndices.size();
			for(unsigned int i = 0; i < numMeshIndices; ++i)
			{
				indicesArray[i] = meshIndices[i];
			}


			std::ofstream outbin(binaryFilename.c_str(), std::ios::out | std::ios::binary);
			outbin.write((char*)&numMeshVertices, sizeof(unsigned int));
			outbin.write((char*)&numMeshIndices, sizeof(unsigned int));
			outbin.write((char*)finalVerts, sizeof(SimpleVertex) * numMeshVertices);
			outbin.write((char*)indicesArray, sizeof(unsigned short) * numMeshIndices);
			outbin.close();

			ID3D11Buffer* indexBuffer;

			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD) * meshIndices.size();     
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = indicesArray;
			_pd3dDevice->CreateBuffer(&bd, &InitData, &indexBuffer);

			meshData.numberOfIndices = meshIndices.size();
			meshData.indexBuffer = indexBuffer;

			//This data has now been sent over to the GPU so we can delete this CPU-side stuff
			delete [] indicesArray;
			delete [] finalVerts;

			return meshData;
		}	
	}
	else
	{
		Geometry meshData;
		unsigned int numVertices;
		unsigned int numIndices;



		//Read in array sizes
		binaryInFile.read((char*)&numVertices, sizeof(unsigned int));
		binaryInFile.read((char*)&numIndices, sizeof(unsigned int));
		
		//Read in data from binary file
		std::vector<SimpleVertex> finalVerts;
		finalVerts.resize(numVertices);
		std::vector<unsigned short> indices;
		indices.resize(numIndices);
		for (unsigned int i = 0; i < numVertices; i++)
		{
			binaryInFile.read((char*)&finalVerts[i], sizeof(SimpleVertex));
			vertexData->push_back(finalVerts.at(i).Pos);
		}
		binaryInFile.read((char*)&indices[0], sizeof(unsigned short) * numIndices);


		

		//Put data into vertex and index buffers, then pass the relevant data to the MeshData object.
		ID3D11Buffer* vertexBuffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &finalVerts.at(0);

		_pd3dDevice->CreateBuffer(&bd, &InitData, &meshData.vertexBuffer);

		meshData.vertexBufferOffset = 0;
		meshData.vertexBufferStride = sizeof(SimpleVertex);

		ID3D11Buffer* indexBuffer;

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * numIndices;     
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &indices.at(0);
		_pd3dDevice->CreateBuffer(&bd, &InitData, &meshData.indexBuffer);

		meshData.numberOfIndices = numIndices;

		//This data has now been sent over to the GPU so we can delete this CPU-side stuff
		 indices.clear();
		 finalVerts.clear();

		return meshData;
	}
}
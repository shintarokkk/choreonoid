
#include "STLSceneLoader.h"
#include "SceneDrawables.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace std;
using namespace boost::algorithm;
using namespace cnoid;


const char* STLSceneLoader::format() const
{
    return "STL";
}


static void readVector3(string text, SgVectorArray<Vector3f>* array)
{
    trim(text);
    Vector3f value;
    int i = 0;
    split_iterator<string::iterator> iter = make_split_iterator(text, token_finder(is_space(), token_compress_on));
    while(iter != split_iterator<string::iterator>()){
        value[i++] = boost::lexical_cast<float>(*iter++);
        if(i == 3){
            array->push_back(value);
            break;
        }
    }
}


SgNode* STLSceneLoader::load(const std::string& fileName)
{
    std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);

    SgVertexArrayPtr vertices = new SgVertexArray;
    SgNormalArrayPtr normals = new SgNormalArray;

    uint8_t header[80];
    ifs.read((char *)header, 80);
    if(strncmp((char *)header, "solid", 5) != 0 ||
       header[0] == 's' && header[1] == 'o' && header[2] == 'l' && header[3] == 'i' && header[4] == 'd'){
        // stl file is in binary format
        uint32_t ntriangle;
        ifs.read((char *)&ntriangle, 4);
        for(size_t i = 0; i < ntriangle; i++){
            Vector3f value;
            for(size_t j = 0; j < 3; j++){
                float v;
                ifs.read((char *)&v, 4);
                value[j] = v;
            }
            normals->push_back(value);
            for(size_t k = 0; k < 3; k++){
                Vector3f value;
                for(size_t j = 0; j < 3; j++){
                    float v;
                    ifs.read((char *)&v, 4);
                    value[j] = v;
                }
                vertices->push_back(value);
            }
            uint16_t attrib;
            ifs.read((char *)&attrib, 2);
        }
    } else {
        // stl file is in text format
        std::ifstream ifs(fileName.c_str(), std::ios::in);
        std::string line;
        while(!ifs.eof() && getline(ifs, line)){
            trim(line);
            if(boost::istarts_with(line, "vertex")){
                readVector3(line.substr(6), vertices);
            } else if(boost::istarts_with(line, "facet normal")){
                readVector3(line.substr(12), normals);
            }
        }
    }
    SgShape* shape = 0;
    
    if(!vertices->empty()){
        shape = new SgShape;
        SgMesh* mesh = shape->getOrCreateMesh();
        mesh->setVertices(vertices);
        const int numTriangles = vertices->size() / 3;
        mesh->reserveNumTriangles(numTriangles);
        for(int i = 0; i < numTriangles; ++i){
            const int j = i * 3;
            mesh->addTriangle(j, j + 1, j + 2);
        }
        if(!normals->empty()){
            mesh->setNormals(normals);
            SgIndexArray& indices = mesh->normalIndices();
            indices.reserve(normals->size() * 3);
            for(int i = 0; i < normals->size(); ++i) {
                indices.push_back(i);
                indices.push_back(i);
                indices.push_back(i);
            }
        }
    }
    
    return shape; 
}

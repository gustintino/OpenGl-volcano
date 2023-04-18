#include "ObjectFromFile.h"


ObjectFromFile::ObjectFromFile(const char* path, glm::vec4 _color)
{
    //loads, and sets the vertex attributes, also sets the color
    numOfVert = 0;

    color = _color;
    vertices = LoadOBJFile(path);

    vao = 0;
    vbo = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 7));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 10));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
}

ObjectFromFile::ObjectFromFile(const char* _path, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
{
    //same as above but sets the default position, rotation, scale
    numOfVert = 0;

    //color = _color;
    vertices = LoadOBJFile(_path);

    vao = 0;
    vbo = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 7));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 10));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    model = glm::mat4(1.0f);
    ChangePosition(_position, _rotation, _scale);
}

ObjectFromFile::~ObjectFromFile()
{

}

std::vector<vertex> ObjectFromFile::LoadOBJFile(const char* filePath)
{
    //loads in obj files
    std::vector<glm::vec3> positions(1); //v
    std::vector<glm::vec3> normals(1); //vn
    std::vector<glm::vec2> textureCoords(1); //vt
    std::vector<glm::ivec3> faceIndices(1); //f

    std::vector<vertex> finalVertices;

    std::ifstream inObjFile(filePath);


    std::stringstream ss;
    std::string line = "";
    std::string prefix = "";
    glm::vec3 tempVector3;
    glm::vec2 tempVector2;
    glm::ivec3 tempiVector3;

    //file open error check, if it cannot find the file, the program will crash
    if (!inObjFile.is_open())
    {
        std::cout << "ERROR - Cannot find obj file!" << std::endl;
    }

    //read one line at a time from the file
    while (std::getline(inObjFile, line))
    {
        //clear the stringstream for the next line
        ss.clear();
        ss.str(line);
        //Look at the prefix - the characters before the first space on the line
        ss >> prefix;


        if (prefix == "v") //it is a vertex position so save this away
        {
            //format = v x y z
            ss >> tempVector3.x >> tempVector3.y >> tempVector3.z;
            positions.emplace_back(tempVector3);
            numOfVert++;
        }
        else if (prefix == "vt") //it is a vertex texture co-ordinate so save this away
        {
            //format = vt u v
            ss >> tempVector2.x >> tempVector2.y;
            textureCoords.emplace_back(tempVector2);
        }
        else if (prefix == "vn") //it is a vertex normal so save this away
        {
            //format = vn x y z
            ss >> tempVector3.x >> tempVector3.y >> tempVector3.z;
            normals.emplace_back(tempVector3);
        }
        else if (prefix == "f") //it is a face so save this away
        {
            //format = f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            int tempInt;

            //Slightly longer code than the rest as we need to keep going for 
            //the 3 vertex indices a face is "made up of"
            while (ss >> tempInt)
            {
                tempiVector3.x = tempInt;

                //we need to ignore a / or space so peek() checks that for us
                //if the next character is a / or space, move past it
                if (ss.peek() == '/')
                    ss.ignore(1, '/');
                else if (ss.peek() == ' ')
                    ss.ignore(1, ' ');

                ss >> tempiVector3.y;

                if (ss.peek() == '/')
                    ss.ignore(1, '/');
                else if (ss.peek() == ' ')
                    ss.ignore(1, ' ');

                ss >> tempiVector3.z;

                //Now we have the 3 indices, put this as a ivec3
                faceIndices.emplace_back(tempiVector3);
            }
        }
        else
        {
            //it is something we do not need so ignore it
        }
    }

    for (int i = 1; i < faceIndices.size(); ++i)
    {
        //There is no colour data in the .obj and for this example,
        //assume the Vertex struct/class has both position and colour to fill
        //so this example just adds a magenta colour
        //Change this to suit your needs!
        glm::vec4 colour(1, 0, 1, 1);

        //The creation of your vertex struct/class 
        //In the example, we are not using the normal or texture co-ordinate data
        vertex vertex{
               {positions[faceIndices[i].x].x,
               positions[faceIndices[i].x].y,
               positions[faceIndices[i].x].z},

               {color.r, color.g, color.b, color.a},

               {normals[faceIndices[i].z].x,
               normals[faceIndices[i].z].y,
               normals[faceIndices[i].z].z},

               {textureCoords[faceIndices[i].y].x,
               textureCoords[faceIndices[i].y].y}

            
        };

        //Add this vertex to the vertex vector
        finalVertices.emplace_back(vertex);
    }

    //output the vector of vertices filled with data
    return finalVertices;
  

}

void ObjectFromFile::Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
{
    //draw and sets everything else
    position = _position;
    rotation = _rotation;
    scale = _scale;

    model = glm::mat4(1.0f);
    ChangePosition(_position, _rotation, _scale);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void ObjectFromFile::Draw()
{
    //just draws
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

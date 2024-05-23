#include "Track.h"
#include <fstream>

Track::Track()
{
    m_loaded = false;
    m_axis = rend::vec3(1, 0, 0);
    m_scale = 1;
    m_step = 0.005;
    m_wrap = true;
    m_rollWrap = true;
    m_offset = rend::vec2(0, 0);
    m_normal = true;
    m_fast = true;
}

float Track::getScale()
{
    return m_scale;
}

void Track::setScale(float _scale)
{
    m_scale = _scale;
}

bool Track::getNormalFlag()
{
    return m_normal;
}

void Track::setNormalFlag(bool _flag)
{
    m_normal = _flag;
    m_loaded = false;
}

void Track::setFast(bool _flag)
{
    m_fast = _flag;
}

bool Track::getWrap()
{
    return m_wrap;
}

void Track::setWrap(bool _wrap)
{
    m_wrap = _wrap;
}

bool Track::getRollWrap()
{
    return m_rollWrap;
}

void Track::setRollWrap(bool _wrap)
{
    m_rollWrap = _wrap;
}

rend::vec2 Track::getOffset()
{
    return m_offset;
}

void Track::setOffset(rend::vec2 _offset)
{
    m_offset = _offset;
}

rend::vec3 Track::getAxis()
{
    return m_axis;
}

void Track::setAxis(rend::vec3 _axis)
{
    if (_axis.z == 1 || _axis.z == -1)
    {
        m_axis = rend::vec3(0, 0, 1);
    }
    else
    {
        m_axis = rend::vec3(1, 0, 0);
    }
}

void Track::onInit()
{
    m_defultPath = "models/track/coasterTrackShort";
    //m_defultPath = "models/track/Rail";
    m_pathSimple = m_defultPath + "NEW.obj";
    m_defultPathSimple = m_defultPath + ".obj";
    #ifdef __EMSCRIPTEN__
        m_path = "data/" + m_defultPath + "NEW.obj";
        m_defultPath = "data/" + m_defultPath + ".obj";
    #else		
        m_path = "../data/" + m_defultPath + "NEW.obj";
        m_defultPath = "../data/" + m_defultPath + ".obj";
    #endif
}

void Track::onTick()
{
}

void Track::setPath(std::string _path)
{
    m_pathSimple = _path;
    #ifdef __EMSCRIPTEN__
        m_path = "data/" + _path;
    #else
        m_path = "../data/" + _path;
    #endif
}

void Track::setDefultPath(std::string _path)
{
    m_defultPathSimple = _path;
    #ifdef __EMSCRIPTEN__
        m_defultPath = "data/" + _path;
    #else
        m_defultPath = "../data/" + _path;
    #endif

    m_loaded = false;
}

std::string Track::getPathSimple()
{
    return m_pathSimple;
}

std::string Track::getDefultPathSimple()
{
    return m_defultPathSimple;
}


std::string Track::getPath()
{
    return m_path;
}

void Track::setSpline(std::shared_ptr<Spline> _spline)
{
    m_spline = _spline;
}

void Track::setStep(float _step)
{
    m_step = _step;
}

float Track::getStep()
{
    return m_step;
}

void Track::create()
{
    if (!m_loaded)
    {
        loadDefult();
        m_loaded = true;
        std::cout << "Template Loaded!" << std::endl;
    }
    editData();
    createFile();
    std::cout << "New Object Created!" << std::endl;
}

void Track::editData()
{
    std::cout << "Updating Object Data..." << std::endl;

    float segmentLength;
    if (m_axis.x == 1)
    {
        segmentLength = (m_maxPos.x * m_scale) - (m_minPos.x * m_scale);
    }
    else
    {
        segmentLength = (m_maxPos.z * m_scale) - (m_minPos.z * m_scale);
    }
    float splineLength = m_spline->getLength();
    int segmentNumTotal = (splineLength / segmentLength);
    if (m_wrap) segmentNumTotal++;
    float posOffset = 0;
    if (m_minPos.x * m_scale < 0) posOffset = fabs(m_minPos.x * m_scale);
    if (m_partsDefult.size() == 0) return;

    int threadNum = std::thread::hardware_concurrency();
    //int threadNum = 8;

    //Resizes threads vector 
    m_segParts.clear();
    m_segParts.resize(segmentNumTotal);
    for (int s = 0; s < segmentNumTotal; s++)
    {
        m_segParts[s].resize(m_partsDefult.size());

        for (int p = 0; p < m_partsDefult.size(); p++)
        {
            m_segParts[s][p].vertex.resize(m_partsDefult[p].vertex.size());
            m_segParts[s][p].normal.resize(m_partsDefult[p].normal.size());
            m_segParts[s][p].face.resize(m_partsDefult[p].face.size());
            m_segParts[s][p].mat.resize(m_partsDefult[p].mat.size());
        }
    }
    
    //adds tasks to list
    m_segs.clear();
    for (int s = 0; s < segmentNumTotal; s++)
    {
        m_segs.push_back([this, s, segmentNumTotal, segmentLength, posOffset]() {
            this->mapSegToSpline(s + 1, segmentNumTotal, segmentLength, posOffset);
        });
    }

    //Creates thread
    m_threads.clear();
    for (int t = 0; t < threadNum; t++)
    {
        m_threads.emplace_back(&Track::segManager, this );
    }

    //Wait for thread to finish
    for (int i = 0; i < threadNum; i++)
    {
        m_threads[i].join();
    }
    std::cout << "Object data updated!" << std::endl;

    //Adds all of the threads data together into a single vector in the correct order
    m_parts.clear();
    for (int p = 0; p < m_partsDefult.size(); p++)
    {
        m_parts.emplace_back();

        for (int s = 0; s < segmentNumTotal; s++)
        {
            for (int v = 0; v < m_segParts[s][p].vertex.size(); v++)
            {
                m_parts[p].vertex.emplace_back();
                m_parts[p].vertex.back() = m_segParts[s][p].vertex[v];
            }
            for (int n = 0; n < m_segParts[s][p].normal.size(); n++)
            {
                m_parts[p].normal.emplace_back();
                m_parts[p].normal.back() = m_segParts[s][p].normal[n];
            }
            for (int f = 0; f < m_segParts[s][p].face.size(); f++)
            {
                m_parts[p].face.emplace_back();
                m_parts[p].face.back().vertex = m_segParts[s][p].face[f].vertex;
                m_parts[p].face.back().texture = m_segParts[s][p].face[f].texture;
                m_parts[p].face.back().normal = m_segParts[s][p].face[f].normal;
            }
            for (int m = 0; m < m_segParts[s][p].mat.size(); m++)
            {
                m_parts[p].mat.emplace_back();
                m_parts[p].mat.back().name = m_segParts[s][p].mat[m].name;
                m_parts[p].mat.back().faceNum = m_segParts[s][p].mat[m].faceNum;
            }
        }
        for (int t = 0; t < m_partsDefult[p].texture.size(); t++)
        {
            m_parts[p].texture.emplace_back();
            m_parts[p].texture.back() = m_partsDefult[p].texture[t];
        }
    }
    std::cout << "Data sorted!" << std::endl;
}


void Track::segManager()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_segMutex);

            if (!m_segs.size() == 0)
            {
                task = m_segs.back();
                m_segs.pop_back();
            }
        }
        if (m_segs.size() % 10 == 0 && m_segs.size() != 0)
        {
            std::cout << m_segs.size() << " segments left" << std::endl;
        }
        if (task)
        {
            task();
        }
        else
        {
            return;
        }
    }
}

void Track::mapSegToSpline(int _segNum, int _segTotalNum, float _segLength, float _posOffset)
{
    std::vector<DistanceMap> cache = distanceCache(_segNum, _segTotalNum, _segLength, _posOffset); //Gets spline positions for current segment

    for (int p = 0; p < m_partsDefult.size(); p++)
    {
        m_segParts[_segNum - 1][p].name = m_partsDefult[p].name;
        //Update Vertex Positions
        if (m_partsDefult[p].vertex.size() != 0)
        {
            for (int v = 0; v < m_partsDefult[p].vertex.size(); v++)
            {
                rend::vec3 newV = m_partsDefult[p].vertex[v] * m_scale;
                int marker = -1;

                for (int c = 0; c < cache.size(); c++)
                {
                    if (m_axis.x == 1)
                    {
                        if (newV.x == cache[c].distance)
                        {
                            marker = c;
                            break;
                        }
                    }
                    else
                    {
                        if (newV.z == cache[c].distance)
                        {
                            marker = c;
                            break;
                        }
                    }
                }

                if (marker != -1)
                {
                    rend::vec3 point = cache[marker].pos;
  
                    //Move vertex
                    if (m_axis.x == 1)
                    {
                        point += -((m_partsDefult[p].vertex[v].z * m_scale) + m_offset.x) * cache[marker].pri;
                    }
                    else
                    {
                        point += m_partsDefult[p].vertex[v].x * m_scale * cache[marker].pri;
                    }
                    point += -((m_partsDefult[p].vertex[v].y * m_scale) + m_offset.y) * cache[marker].bi;
                    //point += rend::vec3(0, m_offset.y, 0);

                    //Rotate Vertex
                    if (cache[marker].roll != 0)
                    {
                        point = rotateVertRoundPoint(point, cache[marker].pos, cache[marker].tan, -cache[marker].roll);
                    }

                    m_segParts[_segNum - 1][p].vertex[v] = point;
                }
            }
        }

        if (m_partsDefult[p].normal.size() != 0)
        {
            for (int n = 0; n < m_partsDefult[p].normal.size(); n++)
            {
                m_segParts[_segNum - 1][p].normal[n] = m_partsDefult[p].normal[n]; //This is the line I do not understand 
            }
        }

        //Update Faces
        if (m_partsDefult[p].face.size() != 0)
        {
            int segmentOffsetVerts = m_partsDefult[p].vertex.size() * (_segNum - 1);         //How many vertexs have been added to this part in already created segments
            int segmentOffsetNormals = m_partsDefult[p].normal.size() * (_segNum - 1);       //How many normals have been added to this part in already created segments
            int partsOffsetVerts = 0;                                                    //The total number of vertexs that will be added to parts before this one
            int partsOffsetNormals = 0;                                                  //The total number of normlas that will be added to parts before this one
            
            for (int count = 0; count < p; count++)
            {
                if (m_partsDefult[count].vertex.size() != 0)
                {
                    partsOffsetVerts += m_partsDefult[count].vertex.size() * (_segTotalNum - 1);
                }
                if (m_partsDefult[count].normal.size() != 0)
                {
                    partsOffsetNormals += m_partsDefult[count].normal.size() * (_segTotalNum - 1);
                }
            }

            for (int f = 0; f < m_partsDefult[p].face.size(); f++)
            {
                m_segParts[_segNum - 1][p].face[f].vertex = m_partsDefult[p].face[f].vertex + rend::vec3(segmentOffsetVerts) + rend::vec3(partsOffsetVerts);
                m_segParts[_segNum - 1][p].face[f].texture = m_partsDefult[p].face[f].texture;
                m_segParts[_segNum - 1][p].face[f].normal = m_partsDefult[p].face[f].normal + rend::vec3(segmentOffsetNormals) + rend::vec3(partsOffsetNormals);
            }
        }

        if (m_partsDefult[p].mat.size() != 0)
        {
            for (int m = 0; m < m_partsDefult[p].mat.size(); m++)
            {
                int segmentOffsetFace = m_partsDefult[p].face.size() * (_segNum - 1);  //How many faces have been placed in segments before this one
                m_segParts[_segNum - 1][p].mat[m].name = m_partsDefult[p].mat[m].name;
                m_segParts[_segNum - 1][p].mat[m].faceNum = m_partsDefult[p].mat[m].faceNum + segmentOffsetFace;
            }
        }
    }
}

std::vector<DistanceMap> Track::distanceCache(int _segNum, int _segTotalNum, float _segLength, float _offset)
{
    std::vector<DistanceMap> cache;

    if (m_partsDefult.size() == 0)
    {
        return cache;
    }

    float start;
    if (m_fast)
    {
        start = m_spline->step(0, (_segNum - 1) * _segLength, 0.01);
    }
    //pos = m_spline->step(0, (dis + _offset) + ((_segNum - 1) * _segLength), m_step)
    for (int p = 0; p < m_partsDefult.size(); p++)
    {
        if (m_partsDefult[p].vertex.size() == 0)
        {
            continue;
        }

        //Caluclate all of the new vertex positions
        for (int v = 0; v < m_partsDefult[p].vertex.size(); v++)
        {
            if (cache.size() == 0)
            {
                //This is needed as cache.size must be more than 1 or the for loop below will crash!
                rend::vec3 vert = m_partsDefult[p].vertex[v] * m_scale;
                float dis;
                if (m_axis.x == 1)
                {
                    dis = vert.x;
                }
                else
                {
                    dis = vert.z;
                }
                float pos;
                if (m_fast)
                {
                    pos = m_spline->step(start, dis + _offset, m_step);
                }
                else
                {
                    pos = m_spline->step(0, (dis + _offset) + ((_segNum - 1) * _segLength), m_step);
                }
                cache.emplace_back();
                cache[0].t = pos;
                cache[0].distance = dis;
                cache[0].roll = m_spline->getRoll(pos, m_rollWrap);
                cache[0].pos = m_spline->getPoint(pos);      
                cache[0].tan = m_spline->getVelocity(pos);
                cache[0].nor = m_spline->getNormal(pos);
                cache[0].bi = m_spline->getBinormal(pos);
                cache[0].pri = m_spline->getPrincipal(pos);
            }

            rend::vec3 vert = m_partsDefult[p].vertex[v] * m_scale;
            bool flag = false;

            for (int c = 0; c < cache.size(); c++)
            {
                if (vert.x == cache[c].distance)
                {
                    flag = true;
                    break;
                }
            }
            
            if (!flag)
            {
                float dis;
                if (m_axis.x == 1)
                {
                    dis = vert.x;
                }
                else
                {
                    dis = vert.z;
                }

                float pos;
                if (_segNum == _segTotalNum - 1 && m_wrap)
                {
                    float length = m_spline->getLength() - ((_segNum - 1) * _segLength);
                    float percentage = (length / _segLength);
                    if (m_fast)
                    {
                        pos = m_spline->step(start, (dis + _offset) * percentage, m_step);
                    }
                    else
                    {
                        pos = m_spline->step(0, ((dis + _offset) * percentage) + ((_segNum - 1) * _segLength), m_step);
                    }
                }
                else
                {
                    if (m_fast)
                    {
                        pos = m_spline->step(start, dis + _offset, m_step);
                    }
                    else
                    {
                        pos = m_spline->step(0, (dis + _offset) + ((_segNum - 1) * _segLength), m_step);
                    }
                }
                
                cache.emplace_back();
                cache.back().t = pos;
                cache.back().distance = dis;
                cache.back().roll = m_spline->getRoll(pos, m_rollWrap);
                cache.back().pos = m_spline->getPoint(pos);
                cache.back().tan = m_spline->getVelocity(pos);
                cache.back().nor = m_spline->getNormal(pos);
                cache.back().bi = m_spline->getBinormal(pos);
                cache.back().pri = m_spline->getPrincipal(pos);
            }
        }
    }
    //std::cout << cache.size() << std::endl;
    return cache;
}

rend::vec3 Track::rotateVertRoundPoint(rend::vec3 _vert, rend::vec3 _center, rend::vec3 _axis, float _degree)
{
    float angle = glm::radians(_degree);

    //Sine and cosine of angle
    float c = cos(angle);
    float s = sin(angle);

    //Rotation matrix components
    float x = _axis.x;
    float y = _axis.y;
    float z = _axis.z;
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;

    //Rotation Mat
    rend::mat3 rotationMat(
        rend::vec3(x2 + (1 - x2) * c, xy * (1 - c) - z * s, xz * (1 - c) + y * s),
        rend::vec3(xy * (1 - c) + z * s, y2 + (1 - y2) * c, yz * (1 - c) - x * s),
        rend::vec3(xz * (1 - c) - y * s, yz * (1 - c) + x * s, z2 + (1 - z2) * c));

    //Move vertex
    rend::vec3 translated = _vert - _center;
    rend::vec3 rotated = rotationMat * translated;
    rend::vec3 newVert = rotated + _center;

    return newVert;
                            
}

void Track::loadDefult()
{
    std::vector<std::string> lines;  //raw input
    std::vector<std::string> tokens; //cleaned input
    std::vector<std::vector<std::string>> faceTokens; //Used to sort face inputs
    std::string line;
    m_maxPos = rend::vec3(-99999);
    m_minPos = rend::vec3(999999);
    int part = 0;
    m_materials.clear();
    

    //Get data from .obj
    std::ifstream file(m_defultPath);

    if (!file.is_open())
    {
        std::cout << "Error: Could not open file " << m_defultPath << std::endl;
        return;
    }

    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();

    //Sorts data into correct vectors
    m_partsDefult.clear();


    for (size_t li = 0; li < lines.size(); ++li)
    {
        line = lines[li];
        if (line.length() < 1)
        {
            continue;
        }

        tokens = splitWhitespace(line);

        //Special markers!
        if (tokens[0] == "o" || tokens[0] == "g")
        {
            m_partsDefult.emplace_back();
            part = m_partsDefult.size() - 1;

            if (tokens.size() > 1)
            {
                m_partsDefult[part].name = tokens[1];
            }
            else
            {
                m_partsDefult[part].name = "unnamed";
            }
        }

        else if (tokens[0] == "usemtl" && tokens.size() >= 2)
        {
            m_partsDefult[part].mat.emplace_back();
            m_partsDefult[part].mat.back().name = tokens[1];
            m_partsDefult[part].mat.back().faceNum = m_partsDefult[part].face.size();
        }

        else if (tokens[0] == "mtllib" && tokens.size() >= 2)
        {
            if (tokens[1].size() > 4)
            {
                std::string type = tokens[1].substr(tokens[1].length() - 4);
                if (type == ".mtl")
                {
                    m_materials.push_back(tokens[1]);
                }
            }
        }

        //Main data markers
        else if (m_partsDefult.size() != 0)
        {

            if (tokens[0] == "v" && tokens.size() >= 4)
            {
                rend::vec3 vert(
                    std::stof(tokens[1]),
                    std::stof(tokens[2]),
                    std::stof(tokens[3])
                );
                m_partsDefult[part].vertex.push_back(vert);

                if (vert.x > m_maxPos.x) m_maxPos.x = vert.x;
                if (vert.y > m_maxPos.y) m_maxPos.y = vert.y;
                if (vert.z > m_maxPos.z) m_maxPos.z = vert.z;
                if (vert.x < m_minPos.x) m_minPos.x = vert.x;
                if (vert.y < m_minPos.y) m_minPos.y = vert.y;
                if (vert.z < m_minPos.z) m_minPos.z = vert.z;

            }

            else if (tokens[0] == "vt" && tokens.size() >= 3)
            {
                rend::vec2 tex(
                    std::stof(tokens[1]),
                    std::stof(tokens[2])
                );
                m_partsDefult[part].texture.push_back(tex);
            }

            else if (tokens[0] == "vn" && tokens.size() >= 4 && m_normal)    
            {
                rend::vec3 nor(
                    std::stof(tokens[1]),
                    std::stof(tokens[2]),
                    std::stof(tokens[3])
                );
                m_partsDefult[part].normal.push_back(nor);
            }

            else if (tokens[0] == "f" && tokens.size() >= 4)
            {
                faceTokens.clear();
                for (int i = 1; i < tokens.size(); i++)
                {
                    faceTokens.push_back(splitFaceToken(tokens[i]));
                }
                Face face;
                if (faceTokens[0].size() > 0 && faceTokens[1].size() > 0 && faceTokens[2].size() > 0)
                {
                    face.vertex = rend::vec3(
                        std::stof(faceTokens[0][0]),
                        std::stof(faceTokens[1][0]),
                        std::stof(faceTokens[2][0])
                    );
                    if (faceTokens[0].size() > 1 && faceTokens[1].size() > 1 && faceTokens[2].size() > 1)
                    {
                        face.texture = rend::vec3(
                            std::stof(faceTokens[0][1]),
                            std::stof(faceTokens[1][1]),
                            std::stof(faceTokens[2][1])
                        );
                        if (faceTokens[0].size() > 2 && faceTokens[1].size() > 2 && faceTokens[2].size() > 2 && m_normal)
                        {
                            face.normal = rend::vec3(
                                std::stof(faceTokens[0][2]),
                                std::stof(faceTokens[1][2]),
                                std::stof(faceTokens[2][2])
                            );
                        }
                    }

                    m_partsDefult.at(part).face.push_back(face);     
                }
            }
        }
    }
    
    m_loaded = true;
}

void Track::createFile()
{
    std::ofstream file(m_path);

    file << "#Roller Coaster Model - Generated by program!" << "\n";

    if (m_materials.size() != 0)
    {
        for (size_t mat = 0; mat < m_materials.size(); mat++)
        {
            file << "mtllib " << m_materials[mat] << "\n";
        }
    }
    file << "\n";

    for (size_t ob = 0; ob < m_parts.size(); ob++)
    {
        file << "o " << m_parts[ob].name << "\n";
        
        if (m_parts[ob].vertex.size() != 0)
        {
            for (size_t v = 0; v < m_parts[ob].vertex.size(); v++)
            {
                file << "v " <<
                    m_parts[ob].vertex[v].x << " " <<
                    m_parts[ob].vertex[v].y << " " <<
                    m_parts[ob].vertex[v].z << "\n";
            }
        }

        if (m_parts[ob].texture.size() != 0)
        {
            for (size_t vt = 0; vt < m_parts[ob].texture.size(); vt++)
            {
                file << "vt " <<
                    m_parts[ob].texture[vt].x << " " <<
                    m_parts[ob].texture[vt].y << "\n";
            }
        }

        if (m_parts[ob].normal.size() != 0)
        {
            for (size_t vn = 0; vn < m_parts[ob].normal.size(); vn++)
            {
                file << "vn " <<
                    m_parts[ob].normal[vn].x << " " <<
                    m_parts[ob].normal[vn].y << " " <<
                    m_parts[ob].normal[vn].z << "\n";
            }
        }

        if (m_parts[ob].face.size() != 0)
        {
            for (size_t f = 0; f < m_parts[ob].face.size(); f++)
            {
                //First check if a usemtl need to be added 
                if (m_parts[ob].mat.size() != 0)
                {
                    for (size_t m = 0; m < m_parts[ob].mat.size(); m++)
                    {
                        if (m_parts[ob].mat[m].faceNum == f)
                        {
                            file << "usemtl " << m_parts[ob].mat[m].name << "\n";
                        }
                    }
                }
            
                //Add faces
                if (m_parts[ob].face[f].texture.x != -1)
                {
                    if (m_parts[ob].face[f].normal.x > 0)
                    {
                        //Vertex, texture and normal
                        file << "f " <<
                            m_parts[ob].face[f].vertex.x << "/" <<
                            m_parts[ob].face[f].texture.x << "/" <<
                            m_parts[ob].face[f].normal.x << " " <<
                            m_parts[ob].face[f].vertex.y << "/" <<
                            m_parts[ob].face[f].texture.y << "/" <<
                            m_parts[ob].face[f].normal.y << " " <<
                            m_parts[ob].face[f].vertex.z << "/" <<
                            m_parts[ob].face[f].texture.z << "/" <<
                            m_parts[ob].face[f].normal.z << "\n";
                    }
                    else
                    {
                        //Vertex and texture
                        file << "f " <<
                            m_parts[ob].face[f].vertex.x << "/" <<
                            m_parts[ob].face[f].texture.x << " " <<
                            m_parts[ob].face[f].vertex.y << "/" <<
                            m_parts[ob].face[f].texture.y << " " <<
                            m_parts[ob].face[f].vertex.z << "/" <<
                            m_parts[ob].face[f].texture.z << "\n";
                    }
                }
                else
                {
                    //Vertex only 
                    file << "f " <<
                        m_parts[ob].face[f].vertex.x << " " <<
                        m_parts[ob].face[f].vertex.y << " " <<
                        m_parts[ob].face[f].vertex.z << "\n";
                }
            }
        }

        if (m_parts[ob].vertex.size() != 0)
        {
            file << "# Vertices: " << m_parts[ob].vertex.size();
        }
        else
        {
            file << "# Vertices: 0";
        }
        if (m_parts[ob].normal.size() != 0)
        {
            file << ", normals: " << m_parts[ob].normal.size();
        }
        else
        {
            file << ", normals: 0";
        }
        if (m_parts[ob].texture.size() != 0)
        {
            file << ", texture coordinates: " << m_parts[ob].texture.size();
        }
        else
        {
            file << ", texture coordinates: 0";
        }
        if (m_parts[ob].face.size() != 0)
        {
            file << ", faces: " << m_parts[ob].face.size() << "\n\n";
        }
        else
        {
            file << " faces: 0\n\n";
        }
    }
}

std::vector<std::string> Track::splitWhitespace(std::string _string)
{
    std::vector<std::string> rtn;
    std::string token;

    for (size_t i = 0; i < _string.length(); i++)
    {
        if (_string[i] == '\r' || _string[i] == '\n' || _string[i] == ' ' || _string[i] == '\t')
        {
            rtn.push_back(token);
            token = "";
        }
        else
        {
            token += _string[i];
        }
    }
    if (token != "")
    {
        rtn.push_back(token);
    }

    return rtn;
}

std::vector<std::string> Track::splitFaceToken(std::string _string)
{
    //Create output vector and fill it with 3 vectors
    std::vector<std::string> rtn;
    std::string number;

    for (int i = 0; i < _string.length(); i++)
    {
        if (_string[i] == '/')
        {
            rtn.push_back(number);
            number = "";
        }
        else
        {
            number += _string[i];
        }
    }
    if (number != "")
    {
        rtn.push_back(number);
    }

    return rtn;
}
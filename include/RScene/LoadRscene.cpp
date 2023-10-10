#include "LoadRscene.hpp"
/**
 * Documentation for the .rscene file format
 * =========================================
 * 
 * Texture <tex_name> <tex_path>
 * Entity <name> {}
 * Shader <shader_name> <vertshader_path> <fragshader_path>
 * RectangleCollider <widthX> <widthY> <offsetX> <offsetY>
 * 
*/

#include <fstream>

#include "RocLogger/RocLogger.hpp"

bool get_word(const std::string& inword, size_t& begin, size_t& end, std::string& outstr, char delimiter = ' ')
{
    if (end == inword.npos)
    {
        return false;
    }

    end = inword.find(delimiter, begin);
    if (end == inword.npos)
    {
        outstr = inword.substr(begin);
    }
    else
    {
        outstr = inword.substr(begin, end - begin);
        begin = end + 1;
    }

    if (outstr == "" || outstr == " ")
        return get_word(inword, begin, end, outstr, delimiter);

    return true;
}

void get_word_quotes(const std::string& instr, size_t& begin, size_t& end, std::string& outstr)
{
    begin = instr.find('"', begin) + 1;
    end = instr.find('"', begin);
    outstr = instr.substr(begin, end - begin);
    begin = end + 1;
}

void LoadScene(const std::string& filepath)
{
    LogTrace("Beginning scene load: " + filepath);

    LogInfo("Clearing any shaders");
    Shaders::clear();
    LogInfo("Clearning any textures");
    Textures::clear();
    LogInfo("Clearing all entities");
    Coordinator::Get()->DestroyAllEntities();
    LogInfo("Everything is ready for loading!");

    std::ifstream infile;
    infile.open(filepath, std::ios::in);
    if (!infile.is_open())
    {
        LogFatal("Could not open scene file " + filepath + ".\nPlease check your path.");
        return;
    }

    std::string line;
    bool done = false;
    while (std::getline(infile, line))
    {
        if (line == "")
        {
            continue;
        }

        size_t begin = 0;
        size_t end = 0;
        bool donothing = false;
        std::string word;
        while (get_word(line, begin, end, word))
        {
            if (donothing)
            {
                continue;
            }

            if (word == "#")
            {
                donothing = true;
                continue;
            }
            else if (word == " " || word == "")
            {
                continue;
            }
            else if (word == "Texture")
            {
                std::string tex_name, tex_path;
                get_word_quotes(line, begin, end, tex_name);
                get_word_quotes(line, begin, end, tex_path);

                LoadTexture(tex_path, tex_name);
            }
            else if (word == "Shader")
            {
                // Shader <shader_name> <vertshader_path> <fragshader_path>
                std::string shader_name, vertpath, fragpath;
                get_word_quotes(line, begin, end, shader_name);
                get_word_quotes(line, begin, end, vertpath);
                get_word_quotes(line, begin, end, fragpath);

                LoadShader(vertpath, fragpath, shader_name);
            }
            else if (word == "Entity")
            {
                std::string ent_name;
                get_word_quotes(line, begin, end, ent_name);
                Entity e = Coordinator::Get()->CreateEntity(ent_name);
                bool nocontinue = false;
                while (word != "{")
                {
                    if (word == "{}")
                    {
                        nocontinue = true;
                        break;
                    }
                    get_word(line, begin, end, word);
                }

                while (word != "}" && !nocontinue)
                {
                    if (end == line.npos)
                    {
                        if (!std::getline(infile, line))
                        {
                            LogError("File abruptly ended.");
                            return;
                        }
                        begin = 0;
                        end = 0;
                        donothing = false;
                    }

                    get_word(line, begin, end, word);
                    if (word == "}")
                        continue;
                    
                    if (donothing)
                    {
                        continue;
                    }

                    if (word == "#")
                    {
                        donothing = true;
                        continue;
                    }
                    else if (word == " " || word == "")
                    {
                        continue;
                    }
                    else if (word == "Transform")
                    {
                        // Transform <xpos> <ypos>
                        std::string inx, iny;
                        get_word(line, begin, end, inx);
                        get_word(line, begin, end, iny);

                        Coordinator::Get()->AddComponent<Transform>(e, Transform());
                        Transform& t = Coordinator::Get()->GetComponent<Transform>(e);
                        t.x = std::stod(inx);
                        t.y = std::stod(iny);
                    }
                    else if (word == "Sprite")
                    {
                        // Sprite <shader_id> <tex_id> <width> <height> <offsetX> <offsetY>
                        std::string shad_name, tex_name, w, h, oX, oY;

                        get_word_quotes(line, begin, end, shad_name);
                        get_word_quotes(line, begin, end, tex_name);
                        get_word(line, begin, end, w);
                        get_word(line, begin, end, h);
                        get_word(line, begin, end, oX);
                        get_word(line, begin, end, oY);

                        Coordinator::Get()->AddComponent<Sprite>(e, Sprite());
                        Sprite& s = Coordinator::Get()->GetComponent<Sprite>(e);

                        s._shader_key = shad_name;
                        s._tex_key = tex_name;
                        s.width = std::stod(w);
                        s.height = std::stod(h);
                        s.offsetX = std::stod(oX);
                        s.offsetY = std::stod(oY);
                    }
                    else if (word == "RectangleCollider")
                    {
                        std::string w, h, x, y;
                        get_word(line, begin, end, w);
                        get_word(line, begin, end, h);
                        get_word(line, begin, end, x);
                        get_word(line, begin, end, y);

                        Coordinator::Get()->AddComponent<RectangleCollider>(e, RectangleCollider());
                        RectangleCollider& rc = Coordinator::Get()->GetComponent<RectangleCollider>(e);

                        rc.width = std::stod(w);
                        rc.height = std::stod(h);
                        rc.offsetX = std::stod(x);
                        rc.offsetY = std::stod(y);
                    }
                    else{
                        try
                        {
                            Coordinator::Get()->AddComponentToEntityFromText(e, word);
                        }
                        catch(std::runtime_error e)
                        {
                            LogWarn(e.what());
                        }
                        
                    }
                }
            }
            else
            {
                LogTrace(word);
            }
        }

        LogTrace("End of line");
    }

    infile.close();

    LogInfo("Load complete!");
}

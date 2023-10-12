#include "LoadRscene.hpp"

#include <fstream>
#include <sstream>
#define STB_C_LEXER_IMPLEMENTATION
#include "stb_c_lexer.h"

#include "RocLogger/RocLogger.hpp"

// bool get_word(const std::string& inword, size_t& begin, size_t& end, std::string& outstr, char delimiter = ' ')
// {
//     if (end == inword.npos)
//     {
//         return false;
//     }

//     end = inword.find(delimiter, begin);
//     if (end == inword.npos)
//     {
//         outstr = inword.substr(begin);
//     }
//     else
//     {
//         outstr = inword.substr(begin, end - begin);
//         begin = end + 1;
//     }

//     if (outstr == "" || outstr == " ")
//         return get_word(inword, begin, end, outstr, delimiter);

//     return true;
// }

// void get_word_quotes(const std::string& instr, size_t& begin, size_t& end, std::string& outstr)
// {
//     begin = instr.find('"', begin) + 1;
//     end = instr.find('"', begin);
//     outstr = instr.substr(begin, end - begin);
//     begin = end + 1;
// }

char* GetNextString(stb_lexer& lex)
{
    LogAssert(stb_c_lexer_get_token(&lex) && "EOF Reached.");
    LogAssert((lex.token == CLEX_sqstring || lex.token == CLEX_dqstring) && "Next token is not a string, but must be.");
    return lex.string;
}

char* GetNextID(stb_lexer& lex)
{
    LogAssert(stb_c_lexer_get_token(&lex) && "EOF Reached.");
    LogAssert((lex.token == CLEX_id) && "Next token is not an id, but must be.");
    return lex.string;
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

    std::stringstream instream;
    instream << infile.rdbuf();
    std::string filecpy(instream.str());

    infile.close();

    char* store_buf = new char[1000];
    stb_lexer lex;
    stb_c_lexer_init(&lex, filecpy.c_str(), filecpy.c_str() + filecpy.size(),
        store_buf, 1000);
    
    Coordinator* cd = Coordinator::Get();

    while (stb_c_lexer_get_token(&lex))
    {
        if (lex.token != CLEX_id)
        {
            continue;
        }

        std::string ident = lex.string;

        if (ident == "Shader")
        {
            // Shader <id> <vertshader> <fragshader>
            std::string shad_id = GetNextString(lex);
            std::string vert_file = GetNextString(lex);
            std::string frag_file = GetNextString(lex);
            LoadShader(vert_file, frag_file, shad_id);
        }
        else if (ident == "Texture")
        {
            // Texture <id> <file>
            std::string tex_id = GetNextString(lex);
            std::string tex_file = GetNextString(lex);
            LoadTexture(tex_file, tex_id);
        }
        else if (ident == "Entity")
        {
            // Entity <id> {<component loop>}
            std::string ent_id = GetNextString(lex);
            Entity e = cd->CreateEntity(ent_id);
            while (stb_c_lexer_get_token(&lex))
            {
                if (lex.token == '{')
                    break;
            }
            while (stb_c_lexer_get_token(&lex))
            {
                if (lex.token == '}')
                    break;
                
                if (lex.token == CLEX_id)
                {
                    // Likely a component... Give it a try!
                    Component* comp = nullptr;
                    std::string comp_type = lex.string;
                    try
                    {
                        cd->AddComponentToEntityFromText(e, comp_type);
                        comp = cd->GetComponentAbstract(comp_type, e);

                    }
                    catch(std::runtime_error e)
                    {
                        LogWarn(e.what());
                    }
                    if (comp == nullptr) continue;
                    while (stb_c_lexer_get_token(&lex))
                    {
                        if (lex.token == '{')
                            break;
                    }
                    while (stb_c_lexer_get_token(&lex))
                    {
                        if (lex.token == '}')
                            break;
                        
                        if (lex.token == CLEX_id)
                        {
                            // A parameter (in the form of <type> <id> = <val>;)
                            std::string type = lex.string;
                            std::string id = GetNextID(lex);
                            assert(stb_c_lexer_get_token(&lex)); assert(lex.token == '=');
                            Property prop(0);
                            if (type == "uint")
                            {
                                assert(stb_c_lexer_get_token(&lex));
                                unsigned int val = lex.int_number;
                                assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                                if (comp->_setters.find(id) != comp->_setters.end())
                                {
                                    comp->_setters.at(id)(val);
                                }
                            }
                            else if (type == "int")
                            {
                                assert(stb_c_lexer_get_token(&lex));
                                int val = lex.int_number;
                                assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                                if (comp->_setters.find(id) != comp->_setters.end())
                                {
                                    comp->_setters.at(id)(val);
                                }
                            }
                            else if (type == "bool")
                            {
                                assert(stb_c_lexer_get_token(&lex));
                                bool val = lex.int_number;
                                assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                                if (comp->_setters.find(id) != comp->_setters.end())
                                {
                                    comp->_setters.at(id)(val);
                                }
                            }
                            else if (type == "double")
                            {
                                assert(stb_c_lexer_get_token(&lex));
                                double val = lex.real_number;
                                assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                                if (comp->_setters.find(id) != comp->_setters.end())
                                {
                                    comp->_setters.at(id)(val);
                                }
                            }
                            // else if (type == "string")
                            // {
                            //     std::string val = GetNextString(lex);
                            //     assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                            //     if (comp->_setters.find(id) != comp->_setters.end())
                            //     {
                            //         comp->_setters.at(id)(val);
                            //     }
                            // }
                            else continue;
                        }
                    }
                }
            }
        }
    }

    delete store_buf;

    LogInfo("Load complete!");

    // std::string line;
    // bool done = false;
    // while (std::getline(infile, line))
    // {
    //     if (line == "")
    //     {
    //         continue;
    //     }

    //     size_t begin = 0;
    //     size_t end = 0;
    //     bool donothing = false;
    //     std::string word;
    //     while (get_word(line, begin, end, word))
    //     {
    //         if (donothing)
    //         {
    //             continue;
    //         }

    //         if (word == "#")
    //         {
    //             donothing = true;
    //             continue;
    //         }
    //         else if (word == " " || word == "")
    //         {
    //             continue;
    //         }
    //         else if (word == "Texture")
    //         {
    //             std::string tex_name, tex_path;
    //             get_word_quotes(line, begin, end, tex_name);
    //             get_word_quotes(line, begin, end, tex_path);

    //             LoadTexture(tex_path, tex_name);
    //         }
    //         else if (word == "Shader")
    //         {
    //             // Shader <shader_name> <vertshader_path> <fragshader_path>
    //             std::string shader_name, vertpath, fragpath;
    //             get_word_quotes(line, begin, end, shader_name);
    //             get_word_quotes(line, begin, end, vertpath);
    //             get_word_quotes(line, begin, end, fragpath);

    //             LoadShader(vertpath, fragpath, shader_name);
    //         }
    //         else if (word == "Entity")
    //         {
    //             std::string ent_name;
    //             get_word_quotes(line, begin, end, ent_name);
    //             Entity e = Coordinator::Get()->CreateEntity(ent_name);
    //             bool nocontinue = false;
    //             while (word != "{")
    //             {
    //                 if (word == "{}")
    //                 {
    //                     nocontinue = true;
    //                     break;
    //                 }
    //                 get_word(line, begin, end, word);
    //             }

    //             while (word != "}" && !nocontinue)
    //             {
    //                 if (end == line.npos)
    //                 {
    //                     if (!std::getline(infile, line))
    //                     {
    //                         LogError("File abruptly ended.");
    //                         return;
    //                     }
    //                     begin = 0;
    //                     end = 0;
    //                     donothing = false;
    //                 }

    //                 get_word(line, begin, end, word);
    //                 if (word == "}")
    //                     continue;
                    
    //                 if (donothing)
    //                 {
    //                     continue;
    //                 }

    //                 if (word == "#")
    //                 {
    //                     donothing = true;
    //                     continue;
    //                 }
    //                 else if (word == " " || word == "")
    //                 {
    //                     continue;
    //                 }
    //                 else if (word == "Transform")
    //                 {
    //                     // Transform <xpos> <ypos>
    //                     std::string inx, iny;
    //                     get_word(line, begin, end, inx);
    //                     get_word(line, begin, end, iny);

    //                     Coordinator::Get()->AddComponent<Transform>(e, Transform());
    //                     Transform& t = Coordinator::Get()->GetComponent<Transform>(e);
    //                     t.x = std::stod(inx);
    //                     t.y = std::stod(iny);
    //                 }
    //                 else if (word == "Sprite")
    //                 {
    //                     // Sprite <shader_id> <tex_id> <width> <height> <offsetX> <offsetY>
    //                     std::string shad_name, tex_name, w, h, oX, oY;

    //                     get_word_quotes(line, begin, end, shad_name);
    //                     get_word_quotes(line, begin, end, tex_name);
    //                     get_word(line, begin, end, w);
    //                     get_word(line, begin, end, h);
    //                     get_word(line, begin, end, oX);
    //                     get_word(line, begin, end, oY);

    //                     Coordinator::Get()->AddComponent<Sprite>(e, Sprite());
    //                     Sprite& s = Coordinator::Get()->GetComponent<Sprite>(e);

    //                     s.shader_key = shad_name;
    //                     s.texture_key = tex_name;
    //                     s.width = std::stod(w);
    //                     s.height = std::stod(h);
    //                     s.offsetX = std::stod(oX);
    //                     s.offsetY = std::stod(oY);
    //                 }
    //                 else if (word == "RectangleCollider")
    //                 {
    //                     std::string w, h, x, y;
    //                     get_word(line, begin, end, w);
    //                     get_word(line, begin, end, h);
    //                     get_word(line, begin, end, x);
    //                     get_word(line, begin, end, y);

    //                     Coordinator::Get()->AddComponent<RectangleCollider>(e, RectangleCollider());
    //                     RectangleCollider& rc = Coordinator::Get()->GetComponent<RectangleCollider>(e);

    //                     rc.width = std::stod(w);
    //                     rc.height = std::stod(h);
    //                     rc.offsetX = std::stod(x);
    //                     rc.offsetY = std::stod(y);
    //                 }
    //                 else{
    //                     try
    //                     {
    //                         Coordinator::Get()->AddComponentToEntityFromText(e, word);
    //                     }
    //                     catch(std::runtime_error e)
    //                     {
    //                         LogWarn(e.what());
    //                     }
                        
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             LogTrace(word);
    //         }
    //     }

    //     LogTrace("End of line");
    // }
}

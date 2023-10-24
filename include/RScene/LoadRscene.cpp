/**
 * @file LoadRscene.cpp
 * 
 * @brief Implementation for @link LoadRscene.hpp @endlink
*/

#include "LoadRscene.hpp"

#include <fstream>
#include <sstream>
#include "stb_c_lexer.h"

#include "RocLogger/RocLogger.hpp"

#include "Base/Keyboard.hpp"

/**
 * Helper function to get the next string value from the lexer. Can
 * throw asserts, as it checks that the next string MUST be a
 * string, and not some identifier.
 * 
 * @param lex A reference to the stb_c_lexer object to obtain the
 * string from.
 * 
 * @return A char* to the string read in by the lexer.
*/
char* GetNextString(stb_lexer& lex)
{
    LogAssert(stb_c_lexer_get_token(&lex) && "EOF Reached.");
    LogAssert((lex.token == CLEX_sqstring || lex.token == CLEX_dqstring) && "Next token is not a string, but must be.");
    return lex.string;
}

/**
 * Helper function to get the next identifier from the lexer. Can
 * throw asserts, as it checks that the next string MUST be an
 * identifier, and not just some string.
 * 
 * @param lex A reference to the stb_c_lexer object to obtain the
 * string from.
 * 
 * @return A char* to the identifier read in by the lexer.
*/
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
                            else if (type == "string")
                            {
                                std::string val = GetNextString(lex);
                                assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                                if (comp->_setters.find(id) != comp->_setters.end())
                                {
                                    comp->_setters.at(id)(val);
                                }
                            }
                            else if (type == "key")
                            {
                                assert(stb_c_lexer_get_token(&lex));
                                RocketKey val = RocketKeyboard::GetKeyFromStringRep(lex.string);
                                assert(stb_c_lexer_get_token(&lex) && lex.token == ';');
                                if (val != RocketKey::K_NULL && comp->_setters.find(id) != comp->_setters.end())
                                {
                                    comp->_setters.at(id)(val);
                                }
                            }
                            else continue;
                        }
                    }
                }
            }
        }
    }

    delete store_buf;

    LogInfo("Load complete!");
}

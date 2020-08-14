#include "redhand/helper.hpp"
#include "redhand/engine.hpp"

#include <sstream>

using namespace redhand;

std::vector<std::string> redhand::helper::impl_split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)){
      tokens.push_back(token);
   }
   return tokens;
}

void redhand::helper::registerEngine(engine* main_engine) {
    m_engine = main_engine;
}

redhand::engine& redhand::helper::getEngineReference(){
    if (m_engine == nullptr){
        throw new std::invalid_argument("engine not set");
    }else{
        return m_engine->getReference();
    }
}
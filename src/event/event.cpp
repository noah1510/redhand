#include "redhand/event/event.hpp"

using namespace redhand;

template <class T>
redhand::event<class T>::event(T* raiser){
    m_raiser = raiser;
}

T* redhand::event<class T>::getRaiser(){
    return m_raiser;
}
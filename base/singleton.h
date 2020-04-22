//
// Created by domac on 2020/4/22.
//

#include "lock.h"

#ifndef CPPKAFKA_SINGLETON_H
#define CPPKAFKA_SINGLETON_H

namespace base {
    template<class T>
    class Singleton
    {
    public:
        inline static T* GetInstance()
        {
            if (m_pInstance)
                return m_pInstance;
            else
                m_pInstance = new T;
            return m_pInstance;
        };
        inline static void Destroy()
        {
            if (m_pInstance) {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        };
    private:
        static T* m_pInstance;
    };
    template<class T>
    T* Singleton<T>::m_pInstance = 0;
}//namespace baselib

#endif //CPPKAFKA_SINGLETON_H

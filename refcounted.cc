#include "vm.h"

#include "refcounted.h"

using namespace std;



Refcounted::Refcounted() : m_refcount(1) {
    
}



int Refcounted::addref() {
    return ++m_refcount;
}

int Refcounted::release() {
    int ret = --m_refcount;
    cout << "release :" << ret << endl;
    if (ret == 0) {
        delete this;
    }
    return ret;
}


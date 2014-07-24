#ifndef __REFCOUNTED_H_
#define __REFCOUNTED_H_


class Refcounted {
  public:
    Refcounted();
    virtual ~Refcounted() {};
    int addref();
    int release();

  private:
    int m_refcount;
};


#endif // __REFCOUNTED_H_

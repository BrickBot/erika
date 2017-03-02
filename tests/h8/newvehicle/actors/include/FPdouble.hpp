#ifndef _FPDOUBLE_HPP_
#define _FPDOUBLE_HPP_

#include "erika.h"

class FPdouble {
  ERIKA_DF_TYPE _v;
public:
  FPdouble(){
    ERIKA_int2df(&_v, 0);
  }

  FPdouble(const FPdouble &f){
    _v = f._v;
  }

  FPdouble(const ERIKA_INT16 f){
    ERIKA_int2df(&_v, f);
  }
    
  operator ERIKA_INT16 () const {
    return ERIKA_df2int(&_v);
  }
  

  const FPdouble & operator =(const FPdouble &f){
    _v = f._v;
    return (*this);
  };
  const FPdouble & operator +=(const FPdouble &f) { 
    ERIKA_df_add(&_v, &_v, &f._v);
    return (*this);
  };
  const FPdouble & operator -=(const FPdouble &f){
    ERIKA_df_sub(&_v, &_v, &f._v);
    return (*this);
  };
  const FPdouble & operator /=(const FPdouble &f){
    ERIKA_df_div(&_v, &_v, &f._v);
    return (*this);
  };
  const FPdouble & operator *=(const FPdouble &f) {
    ERIKA_df_mul(&_v, &_v, &f._v);
    return (*this);
  };
  
  friend bool operator ==(const FPdouble &,  const FPdouble &);
  friend bool operator !=(const FPdouble &,  const FPdouble &);
  friend bool operator >(const FPdouble &,  const FPdouble &);
  friend bool operator <(const FPdouble &,  const FPdouble &);
  friend bool operator <=(const FPdouble &,  const FPdouble &);
  friend bool operator >=(const FPdouble &,  const FPdouble &);
};

FPdouble operator +(const FPdouble &,  const FPdouble &);
FPdouble operator -(const FPdouble &,  const FPdouble &);
FPdouble operator *(const FPdouble &,  const FPdouble &);
FPdouble operator /(const FPdouble &,  const FPdouble &);


#endif



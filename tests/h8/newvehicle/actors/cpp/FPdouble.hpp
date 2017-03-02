#ifndef _FPDOUBLE_HPP_
#define _FPDOUBLE_HPP_
#include "...."

class FPdouble {
  ERIKA_DF_TYPE _v;
public:
  FPdouble();
  FPdouble(const FPdouble &);
  FPdouble(const ERIKA_INT16);
  
    
  operator ERIKA_INT16 () const;

  const FPdouble & operator =(const FPdouble &);
  const FPdouble & operator +=(const FPdouble &);
  const FPbouble & operator -=(const FPdouble &);
  const FPbouble & operator /=(const FPdouble &);
  const FPbouble & operator *=(const FPdouble &);
  
  FPdouble operator +(const FPdouble &,  const FPdouble &);
  FPdouble operator -(const FPdouble &,  const FPdouble &);
  FPdouble operator *(const FPdouble &,  const FPdouble &);
  FPdouble operator /(const FPdouble &,  const FPdouble &);

  friend bool operator ==(const FPdouble &,  const FPdouble &);
  friend bool operator !=(const FPdouble &,  const FPdouble &);
  friend bool operator >(const FPdouble &,  const FPdouble &);
  friend bool operator <(const FPdouble &,  const FPdouble &);
  friend bool operator <=(const FPdouble &,  const FPdouble &);
  friend bool operator >=(const FPdouble &,  const FPdouble &);
};

#endif




#include "FPdouble.hpp"

FPdouble::FPdouble() {
  ERIKA_int2df(&_v, 0);
};

FPdouble::FPdouble(const FPdouble & f) {
  ERIKA_df_assign(&_v, &const_cast<FPdouble &>(f) );
};

FPdouble::FPdouble(const ERIKA_INT16 f) {
  ERIKA_int2df(&_v, f);
};


FPdouble::operator ERIKA_INT16() const {
  return ERIKA_df2int(&_v);
};

const FPdouble & FPdouble::operator = (const FPdouble & f) {
  ERIKA_df_assign(&_v, &(const_cast<FPdouble&>(f)));
  return (*this);
};

const FPdouble & FPdouble::operator += (const FPdouble & f) { 
  ERIKA_df_sum(&_v, &_v, &(const_cast<FPdouble&>(f)));
  return (*this);
};

const FPdouble & FPdouble::operator -= (const FPdouble & f) {
  ERIKA_df_sub(&_v, &_v, &(const_cast<FPdouble&>(f)));
  return (*this);
};

const FPdouble & FPdouble::operator /= (const FPdouble & f) {
  ERIKA_df_mul(&_v, &_v, &(const_cast<FPdouble&>(f)));
  return (*this);
};

const FPdouble & FPdouble::operator /= (const FPdouble & f) {
  ERIKA_df_div(&_v, &_v, &(const_cast<FPdouble&>(f)));
  return (*this);
};

FPdouble operator +(const FPdouble & l,  const FPdouble &r) {
  FPdouble t = l;
  t += r;
  return t;
}

FPdouble operator -(const FPdouble & l,  const FPdouble &r) {
  FPdouble t = l;
  t -= r;
  return t;
}

FPdouble operator *(const FPdouble & l,  const FPdouble &r) {
  FPdouble t = l;
  t *= r;
  return t;
}
FPdouble operator /(const FPdouble & l,  const FPdouble &r) {
  FPdouble t = l;
  t /= r;
  return t;
}


bool operator ==(const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==0);
};


bool operator !=(const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) !=0);
};

bool operator > (const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==1);
};

bool operator < (const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==-1);
};


bool operator >= (const FPdouble &l,  const FPdouble &r) {
  return ((ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==1)||
	  (ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==0));
};

bool operator <= (const FPdouble &l,  const FPdouble &r) {
  return ((ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==-1)||
	  (ERIKA_df_cmp(&(const_cast<FPdouble &> (l._v)),&(const_cast<FPdouble &> (r._v))) ==0));
};


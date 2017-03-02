#include "include/FPdouble.hpp"

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
  return (ERIKA_df_cmp(&l._v,&r._v) ==0);
};


bool operator !=(const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&l._v,&r._v) !=0);
};

bool operator > (const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&l._v,&r._v) ==1);
};

bool operator < (const FPdouble &l,  const FPdouble &r) {
  return (ERIKA_df_cmp(&l._v,&r._v) ==-1);
};


bool operator >= (const FPdouble &l,  const FPdouble &r) {
  return ((ERIKA_df_cmp(&l._v,&r._v) ==1)||
	  (ERIKA_df_cmp(&l._v,&r._v) ==0));
};

bool operator <= (const FPdouble &l,  const FPdouble &r) {
  return ((ERIKA_df_cmp(&l._v,&r._v) ==-1)||
	  (ERIKA_df_cmp(&l._v,&r._v) ==0));
};

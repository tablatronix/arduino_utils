#ifndef number_format_h
#define number_format_h

int round_f(float x){
  return (int)round(x);
}

float round_f_2(float x){
  return round(x);
}

// inline double round( double val )
// {
//     if( val < 0 ) return ceil(val - 0.5);
//     return floor(val + 0.5);
// }

// template <typename T>
// string sFormat(const char *f, T value)
// {
//     char buffer[64];
//     snprintf(buffer, sizeof(buffer), f, value);
//     return string(buffer);
// }

// printf("When this number: %f is assigned to 2 dp, it will be: %.2f ", 94.9456, 94.9456);


#endif
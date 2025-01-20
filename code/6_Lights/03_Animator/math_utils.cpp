#include "6_Lights/03_Animator/mAnimatorLight.h"
#include <cmath> // For math constants like M_PI, fabs, sqrt

int16_t sin16_t(uint16_t theta) {
  int scale = 1;
  if (theta > 0x7FFF) {
    theta = 0xFFFF - theta;
    scale = -1;
  }
  uint32_t precal = theta * (0x7FFF - theta);
  uint64_t numerator = (uint64_t)precal * (4 * 0x7FFF);
  int32_t denominator = 1342095361 - precal;
  int16_t result = numerator / denominator;
  return result * scale;
}

int16_t cos16_t(uint16_t theta) {
  return sin16_t(theta + 0x4000);
}

uint8_t sin8_t(uint8_t theta) {
  int32_t sin16 = sin16_t((uint16_t)theta * 257);
  sin16 += 0x7FFF + 128;
  return min(sin16, int32_t(0xFFFF)) >> 8;
}

uint8_t cos8_t(uint8_t theta) {
  return sin8_t(theta + 64);
}

float sin_approx(float theta) {
  uint16_t scaled_theta = (int)(theta * (float)(0xFFFF / M_TWOPI));
  int32_t result = sin16_t(scaled_theta);
  return float(result) / 0x7FFF;
}

float cos_approx(float theta) {
  uint16_t scaled_theta = (int)(theta * (float)(0xFFFF / M_TWOPI));
  int32_t result = sin16_t(scaled_theta + 0x4000);
  return float(result) / 0x7FFF;
}

float tan_approx(float x) {
  float c = cos_approx(x);
  return c == 0.0f ? 0 : sin_approx(x) / c;
}

#define ATAN2_CONST_A 0.1963f
#define ATAN2_CONST_B 0.9817f

float atan2_t(float y, float x) {
  float abs_y = fabs(y);
  float abs_x = fabs(x);
  float r = (abs_x - abs_y) / (abs_y + abs_x + 1e-10f);
  float angle = x < 0 ? M_PI_2 + M_PI_4 : M_PI_2 - M_PI_4;
  angle += (ATAN2_CONST_A * (r * r) - ATAN2_CONST_B) * r;
  return y < 0 ? -angle : angle;
}

float acos_t(float x) {
  float negate = float(x < 0);
  float xabs = fabs(x);
  float ret = -0.0187293f * xabs;
  ret = (ret + 0.0742610f) * xabs;
  ret = (ret - 0.2121144f) * xabs;
  ret = (ret + M_PI_2) * sqrt(1.0f - xabs);
  ret = ret - 2 * negate * ret;
  return negate * M_PI + ret;
}

float asin_t(float x) {
  return M_PI_2 - acos_t(x);
}

float atan_t(float x) {
  bool neg = (x < 0);
  x = fabs(x);
  float res;
  if (x > 5.0f) {
    res = M_PI_2 - (1.0f / x);
  } else if (x > 1.0f) {
    float xx = x * x;
    res = (0.05375f * xx * xx) + (-0.326175f * xx * x) + (0.974207f * xx) + (0.089494f * x);
  } else {
    float xx = x * x;
    res = ((0.0776509570923569 * xx + -0.287434475393028) * xx + (M_PI_4 - 0.0776509570923569 - -0.287434475393028)) * x;
  }
  return neg ? -res : res;
}

float floor_t(float x) {
  return (x < 0) ? int(x) - 1 : int(x);
}

float fmod_t(float num, float denom) {
  int tquot = num / denom;
  return num - tquot * denom;
}

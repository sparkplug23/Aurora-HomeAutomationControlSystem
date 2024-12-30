#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

// setUpMatrix() - constructs ledmap array from matrix of panels with WxH pixels
// this converts physical (possibly irregular) LED arrangement into well defined
// array of logical pixels: fist entry corresponds to left-topmost logical pixel
// followed by horizontal pixels, when mAnimatorLight::Segment::maxWidth logical pixels are added they
// are followed by next row (down) of mAnimatorLight::Segment::maxWidth pixels (and so forth)
// note: matrix may be comprised of multiple panels each with different orientation
// but ledmap takes care of that. ledmap is constructed upon initialization
// so matrix should disable regular ledmap processing
void mAnimatorLight::setUpMatrix() {

    ALOG_INF(PSTR("setUpMatrix"));

  // ALOG_INF("?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????setUpMatrix()");

  // erase old ledmap, just in case.
  if (customMappingTable != nullptr) delete[] customMappingTable;
  customMappingTable = nullptr;
  customMappingSize = 0;

  // isMatrix is set in cfg.cpp or set.cpp
  if (isMatrix) {

    // calculate width dynamically because it will have gaps
    Segment::maxWidth = 1;
    Segment::maxHeight = 1;

    ALOG_INF("panel size %d", panel.size());

    for (size_t i = 0; i < panel.size(); i++) {
      Panel &p = panel[i];
      if (p.xOffset + p.width > Segment::maxWidth) {
        Segment::maxWidth = p.xOffset + p.width;
      }
      if (p.yOffset + p.height > Segment::maxHeight) {
        Segment::maxHeight = p.yOffset + p.height;
      }
    }

    ALOG_INF(PSTR("maxWidth  %d\n\r"), Segment::maxWidth);
    ALOG_INF(PSTR("maxHeight %d\n\r"), Segment::maxHeight);

    // safety check
    if (Segment::maxWidth * Segment::maxHeight > MAX_LEDS_NEO || Segment::maxWidth <= 1 || Segment::maxHeight <= 1) {
      DEBUG_PRINTLN(F("2D Bounds error."));
      isMatrix = false;
      Segment::maxWidth = _length;
      Segment::maxHeight = 1;
      panels = 0;
      panel.clear(); // release memory allocated by panels
      resetSegments2();
      return;
    }

    customMappingTable = new uint16_t[Segment::maxWidth * Segment::maxHeight];

    if (customMappingTable != nullptr) 
    {
      customMappingSize = Segment::maxWidth * Segment::maxHeight;

      // fill with empty in case we don't fill the entire matrix
      for (size_t i = 0; i< customMappingSize; i++) {
        customMappingTable[i] = (uint16_t)-1;
      }

      // we will try to load a "gap" array (a JSON file)
      // the array has to have the same amount of values as mapping array (or larger)
      // "gap" array is used while building ledmap (mapping array)
      // and discarded afterwards as it has no meaning after the process
      // content of the file is just raw JSON array in the form of [val1,val2,val3,...]
      // there are no other "key":"value" pairs in it
      // allowed values are: -1 (missing pixel/no LED attached), 0 (inactive/unused pixel), 1 (active/used pixel)
      char    fileName[32]; strcpy_P(fileName, PSTR("/2d-gaps.json")); // reduce flash footprint
      bool    isFile = FILE_SYSTEM.exists(fileName);
      size_t  gapSize = 0;
      int8_t *gapTable = nullptr;

      if (isFile && requestJSONBufferLock(20)) {
        DEBUG_PRINT(F("Reading LED gap from "));
        DEBUG_PRINTLN(fileName);
        // read the array into global JSON buffer
        if (pCONT_mfile->readObjectFromFile(fileName, nullptr, &doc)) {
          // the array is similar to ledmap, except it has only 3 values:
          // -1 ... missing pixel (do not increase pixel count)
          //  0 ... inactive pixel (it does count, but should be mapped out (-1))
          //  1 ... active pixel (it will count and will be mapped)
          JsonArray map = doc.as<JsonArray>();
          gapSize = map.size();
          if (!map.isNull() && gapSize >= customMappingSize) { // not an empty map
            gapTable = new int8_t[gapSize];
            if (gapTable) for (size_t i = 0; i < gapSize; i++) {
              gapTable[i] = constrain(map[i], -1, 1);
            }
          }
        }
        DEBUG_PRINTLN(F("Gaps loaded."));
        releaseJSONBufferLock();
      }

      DEBUG_LINE_HERE;

      uint16_t x, y, pix=0; //pixel
      for (size_t pan = 0; pan < panel.size(); pan++) {
        Panel &p = panel[pan];
        uint16_t h = p.vertical ? p.height : p.width;
        uint16_t v = p.vertical ? p.width  : p.height;
        for (size_t j = 0; j < v; j++){
          for(size_t i = 0; i < h; i++) {
            y = (p.vertical?p.rightStart:p.bottomStart) ? v-j-1 : j;
            x = (p.vertical?p.bottomStart:p.rightStart) ? h-i-1 : i;
            x = p.serpentine && j%2 ? h-x-1 : x;
            size_t index = (p.yOffset + (p.vertical?x:y)) * mAnimatorLight::Segment::maxWidth + p.xOffset + (p.vertical?y:x);
            if (!gapTable || (gapTable && gapTable[index] >  0)) customMappingTable[index] = pix; // a useful pixel (otherwise -1 is retained)
            if (!gapTable || (gapTable && gapTable[index] >= 0)) pix++; // not a missing pixel
          }
        }
      }

      DEBUG_LINE_HERE;

      // delete gap array as we no longer need it
      if (gapTable) delete[] gapTable;

      DEBUG_LINE_HERE;
      #ifdef WLED_DEBUG
      DEBUG_PRINT(F("Matrix ledmap:"));
      for (unsigned i=0; i<customMappingSize; i++) {
        if (!(i%mAnimatorLight::Segment::maxWidth)) DEBUG_PRINTLN();
        DEBUG_PRINTF("%4d,", customMappingTable[i]);
      }
      DEBUG_PRINTLN();
      #endif

      DEBUG_LINE_HERE;
    } 
    else  // memory allocation error
    {
      DEBUG_PRINTLN(F("Ledmap alloc error."));
      isMatrix = false;
      panels = 0;
      panel.clear();
      Segment::maxWidth = _length;
      Segment::maxHeight = 1;
      resetSegments2();
    }

    DEBUG_LINE_HERE;
    
  }

  ALOG_INF(PSTR("setUpMatrix Complete")); Serial.flush();
  
}
#endif


///////////////////////////////////////////////////////////
// mAnimatorLight::Segment:: routines
///////////////////////////////////////////////////////////

#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

// XY(x,y) - gets pixel index within current segment (often used to reference leds[] array element)
uint16_t IRAM_ATTR mAnimatorLight::Segment::XY(uint16_t x, uint16_t y)
{
  uint16_t width  = virtualWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
  uint16_t height = virtualHeight();  // segment height in logical pixels (is always >= 1)
  return isActive() ? (x%width) + (y%height) * width : 0;
}

void 
// IRAM_ATTR 
mAnimatorLight::Segment::setPixelColorXY(int x, int y, uint32_t col)
{

  #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
  ALOG_INF(PSTR("Segment::setPixelColorXY(%d,%d|%d,%d,%d)"), x, y, R(col), G(col), B(col));
  #endif

  if (!isActive()) return; // not active

  // DEBUG_LINE_HERE;
  if (x >= virtualWidth() || y >= virtualHeight() || x<0 || y<0) 
{
  
//  ALOG_INF(PSTR("out of segment")); 
  return;  // if pixel would fall out of virtual segment just exit
}



  // DEBUG_LINE_HERE;
  /**
   * @brief WLED brightness method replacing with mine, assumed must be applied
   * 
   */
  // uint8_t _bri_t = currentBri();
  // if (_bri_t < 255) {
  //   byte r = scale8(R(col), _bri_t);
  //   byte g = scale8(G(col), _bri_t);
  //   byte b = scale8(B(col), _bri_t);
  //   byte w = scale8(W(col), _bri_t);
  //   col = RGBW32(r, g, b, w);
  // }


    RgbcctColor c = RgbcctColor(R(col), G(col), B(col), W(col), W(col));

    /**
     * @brief Apply "GLOBAL" brightness to the colour
     * 
     */
    uint8_t bri_master = pCONT_iLight->getBriRGB_Global();
    uint8_t bri_segment = getBrightnessRGB();

    /**
     * @brief Apply "SEGMENT" _brightness_rgb to the colour ALSO (rescale global brightness value, this is similar to WLED opacity)
     * 
     */
    if(bri_segment!=255)
    {
      bri_master = scale8(bri_master, bri_segment); // adjust master by segment
    }

    // Apply global bri_master
    c.R  = scale8(c.R,  bri_master);
    c.G  = scale8(c.G,  bri_master);
    c.B  = scale8(c.B,  bri_master);
    c.W1 = scale8(c.W1, bri_master);
    c.W2 = scale8(c.W2, bri_master);

  
  // This function bypassing the 1D to 2D set function that applies brightness, so we need to apply here before calling the busmanager

    col = c.getU32();

  // DEBUG_LINE_HERE;
  if (reverse  ) x = virtualWidth()  - x - 1;
  if (reverse_y) y = virtualHeight() - y - 1;
  if (transpose) { uint16_t t = x; x = y; y = t; } // swap X & Y if segment transposed

  // DEBUG_LINE_HERE;
  x *= groupLength(); // expand to physical pixels
  // DEBUG_LINE_HERE;
  y *= groupLength(); // expand to physical pixels
  // DEBUG_LINE_HERE;
  if (x >= width() || y >= height())
  {
    
  // DEBUG_LINE_HERE;
    return;  // if pixel would fall out of segment just exit
  }
  // DEBUG_LINE_HERE;

  // ALOG_INF(PSTR("grouping %d"), grouping);

  // DEBUG_LINE_HERE;

  uint32_t tmpCol = col;
  for (int j = 0; j < grouping; j++) {   // groupping vertically
  
  // DEBUG_LINE_HERE;
    for (int g = 0; g < grouping; g++) { // groupping horizontally
    
  // DEBUG_LINE_HERE;
      uint16_t xX = (x+g), yY = (y+j);
      if (xX >= width() || yY >= height()){
        
  // DEBUG_LINE_HERE;
 continue; // we have reached one dimension's end
      }
// #ifndef WLED_DISABLE_MODE_BLEND
//       // if blending modes, blend with underlying pixel
//       if (_modeBlend) tmpCol = color_blend(getPixelColorXY(start + xX, startY + yY), col, 0xFFFFU - progress(), true);
// #endif
      #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
      ALOG_INF(PSTR("--------setPixelColorXY %d, %d, %d, %d, %d -- w%d h%d"), start + xX, startY + yY, R(tmpCol), G(tmpCol), B(tmpCol), width(), height());
      #endif

      // Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
      tkr_anim->setPixelColorXY(start + xX, startY + yY, tmpCol);

      if (mirror) { //set the corresponding horizontally mirrored pixel
        if (transpose) tkr_anim->setPixelColorXY(start + xX, startY + height() - yY - 1, tmpCol); // Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
        else           tkr_anim->setPixelColorXY(start + width() - xX - 1, startY + yY, tmpCol);// Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
      }
      if (mirror_y) { //set the corresponding vertically mirrored pixel
        if (transpose) tkr_anim->setPixelColorXY(start + width() - xX - 1, startY + yY, tmpCol);// Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
        else           tkr_anim->setPixelColorXY(start + xX, startY + height() - yY - 1, tmpCol);// Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
      }
      if (mirror_y && mirror) { //set the corresponding vertically AND horizontally mirrored pixel
        tkr_anim->setPixelColorXY(width() - xX - 1, height() - yY - 1, tmpCol);// Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
      }
      
  // DEBUG_LINE_HERE;
    }
    
  // DEBUG_LINE_HERE;
  }

  DEBUG_LINE_HERE;
}

// anti-aliased version of setPixelColorXY()
void mAnimatorLight::Segment::setPixelColorXY(float x, float y, uint32_t col, bool aa)
{
  DEBUG_LINE_HERE;
  if (!isActive()) return; // not active
  if (x<0.0f || x>1.0f || y<0.0f || y>1.0f)
  {
  
    ALOG_ERR(PSTR("not normalized setPixelColorXY"));//: x=%f, y=%f"), x, y);
    return; // not normalized
  }

  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
DEBUG_LINE_HERE;
  float fX = x * (cols-1);
  float fY = y * (rows-1);
  if (aa) {
    DEBUG_LINE_HERE;
    uint16_t xL = roundf(fX-0.49f);
    uint16_t xR = roundf(fX+0.49f);
    uint16_t yT = roundf(fY-0.49f);
    uint16_t yB = roundf(fY+0.49f);
    float    dL = (fX - xL)*(fX - xL);
    float    dR = (xR - fX)*(xR - fX);
    float    dT = (fY - yT)*(fY - yT);
    float    dB = (yB - fY)*(yB - fY);
    uint32_t cXLYT = getPixelColorXY(xL, yT);
    uint32_t cXRYT = getPixelColorXY(xR, yT);
    uint32_t cXLYB = getPixelColorXY(xL, yB);
    uint32_t cXRYB = getPixelColorXY(xR, yB);

    if (xL!=xR && yT!=yB) {
      setPixelColorXY(xL, yT, color_blend(col, cXLYT, uint8_t(sqrtf(dL*dT)*255.0f))); // blend TL pixel
      setPixelColorXY(xR, yT, color_blend(col, cXRYT, uint8_t(sqrtf(dR*dT)*255.0f))); // blend TR pixel
      setPixelColorXY(xL, yB, color_blend(col, cXLYB, uint8_t(sqrtf(dL*dB)*255.0f))); // blend BL pixel
      setPixelColorXY(xR, yB, color_blend(col, cXRYB, uint8_t(sqrtf(dR*dB)*255.0f))); // blend BR pixel
    } else if (xR!=xL && yT==yB) {
      setPixelColorXY(xR, yT, color_blend(col, cXLYT, uint8_t(dL*255.0f))); // blend L pixel
      setPixelColorXY(xR, yT, color_blend(col, cXRYT, uint8_t(dR*255.0f))); // blend R pixel
    } else if (xR==xL && yT!=yB) {
      setPixelColorXY(xR, yT, color_blend(col, cXLYT, uint8_t(dT*255.0f))); // blend T pixel
      setPixelColorXY(xL, yB, color_blend(col, cXLYB, uint8_t(dB*255.0f))); // blend B pixel
    } else {
      setPixelColorXY(xL, yT, col); // exact match (x & y land on a pixel)
    }
  } else {
    DEBUG_LINE_HERE;
    setPixelColorXY(uint16_t(roundf(fX)), uint16_t(roundf(fY)), col);
  }
  DEBUG_LINE_HERE;
}

// returns RGBW values of pixel
uint32_t mAnimatorLight::Segment::getPixelColorXY(uint16_t x, uint16_t y) 
{
  
  // Serial.println(__LINE__);
  // DEBUG_LINE_HERE;
  if (!isActive()) return 0; // not active
  // DEBUG_LINE_HERE;
  if (x >= virtualWidth() || y >= virtualHeight() || x<0 || y<0) 
  {

    ALOG_ERR(PSTR("OUT OF RANGE getPixelColorXY: x=%d, y=%d, virtualWidth=%d, virtualHeight=%d"), x, y, virtualWidth(), virtualHeight());

    return 0;  // if pixel would fall out of virtual segment just exit

  }
  DEBUG_LINE_HERE3

  if (reverse  ) x = virtualWidth()  - x - 1;
  // DEBUG_LINE_HERE;
  if (reverse_y) y = virtualHeight() - y - 1;
  // DEBUG_LINE_HERE;
  if (transpose) { uint16_t t = x; x = y; y = t; } // swap X & Y if segment transposed
  // DEBUG_LINE_HERE;
  x *= groupLength(); // expand to physical pixels
  // DEBUG_LINE_HERE;
  y *= groupLength(); // expand to physical pixels
  // DEBUG_LINE_HERE;
  if (x >= width() || y >= height()) return 0;
  
  DEBUG_LINE_HERE3
  // Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
  return tkr_anim->getPixelColorXY(start + x, startY + y);
  // DEBUG_LINE_HERE;
}

// Blends the specified color with the existing pixel color.
void mAnimatorLight::Segment::blendPixelColorXY(uint16_t x, uint16_t y, uint32_t color, uint8_t blend) {
  setPixelColorXY(x, y, color_blend(getPixelColorXY(x,y), color, blend));
}

// Adds the specified color with the existing pixel color perserving color balance.
void mAnimatorLight::Segment::addPixelColorXY(int x, int y, uint32_t color, bool fast) {
  if (!isActive()) return; // not active
  if (x >= virtualWidth() || y >= virtualHeight() || x<0 || y<0) return;  // if pixel would fall out of virtual segment just exit
  setPixelColorXY(x, y, color_add(getPixelColorXY(x,y), color, fast));
}

void mAnimatorLight::Segment::fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade) {
  if (!isActive()) return; // not active
  setPixelColorXY(x, y, color_fade(getPixelColorXY(x,y), fade, true));
}

// blurRow: perform a blur on a row of a rectangular matrix
void mAnimatorLight::Segment::blurRow(uint16_t row, fract8 blur_amount) {
  if (!isActive() || blur_amount == 0) return; // not active
  const uint_fast16_t cols = virtualWidth();
  const uint_fast16_t rows = virtualHeight();

  if (row >= rows) return;
  // blur one row
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = Black;
  for (unsigned x = 0; x < cols; x++) {
    CRGB cur = getPixelColorXY(x, row);
    CRGB before = cur;     // remember color before blur
    CRGB part = cur;
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if (x>0) {
      CRGB prev = CRGB(getPixelColorXY(x-1, row)) + part;
      setPixelColorXY_CRGB(x-1, row, prev);
    }
    if (before != cur)         // optimization: only set pixel if color has changed
      setPixelColorXY_CRGB(x, row, cur);
    carryover = part;
  }
}

// blurCol: perform a blur on a column of a rectangular matrix
void mAnimatorLight::Segment::blurCol(uint16_t col, fract8 blur_amount) {
  if (!isActive() || blur_amount == 0) return; // not active
  const uint_fast16_t cols = virtualWidth();
  const uint_fast16_t rows = virtualHeight();

  if (col >= cols) return;
  // blur one column
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = Black;
  for (unsigned y = 0; y < rows; y++) {
    CRGB cur = getPixelColorXY(col, y);
    CRGB part = cur;
    CRGB before = cur;     // remember color before blur
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if (y>0) {
      CRGB prev = CRGB(getPixelColorXY(col, y-1)) + part;
      setPixelColorXY_CRGB(col, y-1, prev);
    }
    if (before != cur)         // optimization: only set pixel if color has changed
      setPixelColorXY_CRGB(col, y, cur);
    carryover = part;
  }
}

// 1D Box blur (with added weight - blur_amount: [0=no blur, 255=max blur])
void mAnimatorLight::Segment::box_blur(uint16_t i, bool vertical, fract8 blur_amount) {
  if (!isActive() || blur_amount == 0) return; // not active
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  const uint16_t dim1 = vertical ? rows : cols;
  const uint16_t dim2 = vertical ? cols : rows;
  if (i >= dim2) return;
  const float seep = blur_amount/255.f;
  const float keep = 3.f - 2.f*seep;
  // 1D box blur
  CRGB tmp[dim1];
  for (int j = 0; j < dim1; j++) {
    uint16_t x = vertical ? i : j;
    uint16_t y = vertical ? j : i;
    int16_t xp = vertical ? x : x-1;  // "signed" to prevent underflow
    int16_t yp = vertical ? y-1 : y;  // "signed" to prevent underflow
    uint16_t xn = vertical ? x : x+1;
    uint16_t yn = vertical ? y+1 : y;
    CRGB curr = getPixelColorXY(x,y);
    CRGB prev = (xp<0 || yp<0) ? Black : getPixelColorXY(xp,yp);
    CRGB next = ((vertical && yn>=dim1) || (!vertical && xn>=dim1)) ? Black : getPixelColorXY(xn,yn);
    uint16_t r, g, b;
    r = (curr.r*keep + (prev.r + next.r)*seep) / 3;
    g = (curr.g*keep + (prev.g + next.g)*seep) / 3;
    b = (curr.b*keep + (prev.b + next.b)*seep) / 3;
    tmp[j] = CRGB(r,g,b);
  }
  for (int j = 0; j < dim1; j++) {
    uint16_t x = vertical ? i : j;
    uint16_t y = vertical ? j : i;
    setPixelColorXY_CRGB(x, y, tmp[j]);
  }
}

// blur1d: one-dimensional blur filter. Spreads light to 2 line neighbors.
// blur2d: two-dimensional blur filter. Spreads light to 8 XY neighbors.
//
//           0 = no spread at all
//          64 = moderate spreading
//         172 = maximum smooth, even spreading
//
//         173..255 = wider spreading, but increasing flicker
//
//         Total light is NOT entirely conserved, so many repeated
//         calls to 'blur' will also result in the light fading,
//         eventually all the way to black; this is by design so that
//         it can be used to (slowly) clear the LEDs to black.

void mAnimatorLight::Segment::blur1d(fract8 blur_amount) {
  const uint16_t rows = virtualHeight();
  for (unsigned y = 0; y < rows; y++) blurRow(y, blur_amount);
}

void mAnimatorLight::Segment::moveX(int8_t delta, bool wrap) {
  if (!isActive()) return; // not active
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  if (!delta || abs(delta) >= cols) return;
  uint32_t newPxCol[cols];
  for (int y = 0; y < rows; y++) {
    if (delta > 0) {
      for (int x = 0; x < cols-delta; x++)    newPxCol[x] = getPixelColorXY((x + delta), y);
      for (int x = cols-delta; x < cols; x++) newPxCol[x] = getPixelColorXY(wrap ? (x + delta) - cols : x, y);
    } else {
      for (int x = cols-1; x >= -delta; x--) newPxCol[x] = getPixelColorXY((x + delta), y);
      for (int x = -delta-1; x >= 0; x--)    newPxCol[x] = getPixelColorXY(wrap ? (x + delta) + cols : x, y);
    }
    for (int x = 0; x < cols; x++) setPixelColorXY(x, y, newPxCol[x]);
  }
}

void mAnimatorLight::Segment::moveY(int8_t delta, bool wrap) {
  if (!isActive()) return; // not active
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  if (!delta || abs(delta) >= rows) return;
  uint32_t newPxCol[rows];
  for (int x = 0; x < cols; x++) {
    if (delta > 0) {
      for (int y = 0; y < rows-delta; y++)    newPxCol[y] = getPixelColorXY(x, (y + delta));
      for (int y = rows-delta; y < rows; y++) newPxCol[y] = getPixelColorXY(x, wrap ? (y + delta) - rows : y);
    } else {
      for (int y = rows-1; y >= -delta; y--) newPxCol[y] = getPixelColorXY(x, (y + delta));
      for (int y = -delta-1; y >= 0; y--)    newPxCol[y] = getPixelColorXY(x, wrap ? (y + delta) + rows : y);
    }
    for (int y = 0; y < rows; y++) setPixelColorXY(x, y, newPxCol[y]);
  }
}

// move() - move all pixels in desired direction delta number of pixels
// @param dir direction: 0=left, 1=left-up, 2=up, 3=right-up, 4=right, 5=right-down, 6=down, 7=left-down
// @param delta number of pixels to move
// @param wrap around
void mAnimatorLight::Segment::move(uint8_t dir, uint8_t delta, bool wrap) {
  if (delta==0) return;
  switch (dir) {
    case 0: moveX( delta, wrap);                      break;
    case 1: moveX( delta, wrap); moveY( delta, wrap); break;
    case 2:                      moveY( delta, wrap); break;
    case 3: moveX(-delta, wrap); moveY( delta, wrap); break;
    case 4: moveX(-delta, wrap);                      break;
    case 5: moveX(-delta, wrap); moveY(-delta, wrap); break;
    case 6:                      moveY(-delta, wrap); break;
    case 7: moveX( delta, wrap); moveY(-delta, wrap); break;
  }
}

void mAnimatorLight::Segment::draw_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB col) {
  if (!isActive() || radius == 0) return; // not active
  // Bresenham’s Algorithm
  int d = 3 - (2*radius);
  int y = radius, x = 0;
  while (y >= x) {
    setPixelColorXY(cx+x, cy+y, col);
    setPixelColorXY(cx-x, cy+y, col);
    setPixelColorXY(cx+x, cy-y, col);
    setPixelColorXY(cx-x, cy-y, col);
    setPixelColorXY(cx+y, cy+x, col);
    setPixelColorXY(cx-y, cy+x, col);
    setPixelColorXY(cx+y, cy-x, col);
    setPixelColorXY(cx-y, cy-x, col);
    x++;
    if (d > 0) {
      y--;
      d += 4 * (x - y) + 10;
    } else {
      d += 4 * x + 6;
    }
  }
}

// by stepko, taken from https://editor.soulmatelights.com/gallery/573-blobs
void mAnimatorLight::Segment::fill_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB col) {
  if (!isActive() || radius == 0) return; // not active
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  for (int16_t y = -radius; y <= radius; y++) {
    for (int16_t x = -radius; x <= radius; x++) {
      if (x * x + y * y <= radius * radius &&
          int16_t(cx)+x>=0 && int16_t(cy)+y>=0 &&
          int16_t(cx)+x<cols && int16_t(cy)+y<rows)
        setPixelColorXY(cx + x, cy + y, col);
    }
  }
}

void mAnimatorLight::Segment::nscale8(uint8_t scale) {
  if (!isActive()) return; // not active
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
    setPixelColorXY(x, y, CRGB(getPixelColorXY(x, y)).nscale8(scale));
  }
}

//line function
void mAnimatorLight::Segment::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c) {
  if (!isActive()) return; // not active
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  if (x0 >= cols || x1 >= cols || y0 >= rows || y1 >= rows) return;
  const int16_t dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  const int16_t dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int16_t err = (dx>dy ? dx : -dy)/2, e2;
  for (;;) {
    setPixelColorXY(x0,y0,c);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

#include "font/console_font_4x6.h"
#include "font/console_font_5x8.h"
#include "font/console_font_5x12.h"
#include "font/console_font_6x8.h"
#include "font/console_font_7x9.h"

// draws a raster font character on canvas
// only supports: 4x6=24, 5x8=40, 5x12=60, 6x8=48 and 7x9=63 fonts ATM
void mAnimatorLight::Segment::drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2, int8_t rotate) {
  if (!isActive()) return; // not active
  if (chr < 32 || chr > 126) return; // only ASCII 32-126 supported
  chr -= 32; // align with font table entries
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  const int font = w*h;

  CRGB col = CRGB(color);
  CRGBPalette16 grad = CRGBPalette16(col, col2 ? CRGB(col2) : col);

  //if (w<5 || w>6 || h!=8) return;
  for (int i = 0; i<h; i++) { // character height
    uint8_t bits = 0;
    switch (font) {
      case 24: bits = pgm_read_byte_near(&console_font_4x6[(chr * h) + i]); break;  // 5x8 font
      case 40: bits = pgm_read_byte_near(&console_font_5x8[(chr * h) + i]); break;  // 5x8 font
      case 48: bits = pgm_read_byte_near(&console_font_6x8[(chr * h) + i]); break;  // 6x8 font
      case 63: bits = pgm_read_byte_near(&console_font_7x9[(chr * h) + i]); break;  // 7x9 font
      case 60: bits = pgm_read_byte_near(&console_font_5x12[(chr * h) + i]); break; // 5x12 font
      default: return;
    }
    col = ColorFromPalette(grad, (i+1)*255/h, 255, NOBLEND);
    for (int j = 0; j<w; j++) { // character width
      int x0, y0;
      switch (rotate) {
        case -1: x0 = x + (h-1) - i; y0 = y + (w-1) - j; break; // -90 deg
        case -2:
        case  2: x0 = x + j;         y0 = y + (h-1) - i; break; // 180 deg
        case  1: x0 = x + i;         y0 = y + j;         break; // +90 deg
        default: x0 = x + (w-1) - j; y0 = y + i;         break; // no rotation
      }
      if (x0 < 0 || x0 >= cols || y0 < 0 || y0 >= rows) continue; // drawing off-screen
      if (((bits>>(j+(8-w))) & 0x01)) { // bit set
        // DEBUG_LINE_HERE_MARKER;
        setPixelColorXY_CRGB(x0, y0, col);
      }
    }
  }
}

/**
 * @brief Need to move the segment.custom1 etc out of here, and into the effect itself so this function can be more generic. 
 * 
 * @param chr 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 * @param rotate 
 * @param solidPerChar 
 * @param horizontalGradient 
 * @param backgroundGradientHorizontal 
 */
void mAnimatorLight::Segment::drawCharacter_UsingGradientPalletes(  
    unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h,   
    int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundGradientHorizontal) {
  
  if (!isActive()) return; // not active
  if (chr < 32 || chr > 126) return; // only ASCII 32-126 supported
  chr -= 32; // align with font table entries
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  const int font = w * h;

  // Fetch the background gradient color from the unloaded palette using params_user[0]
  uint16_t backgroundPaletteId = custom1; // Palette ID for background
  uint32_t backgroundColor;  // Will be fetched later based on pixel position

  // Iterate over character rows (height)
  for (int i = 0; i < h; i++) { 
    uint8_t bits = 0;
    switch (font) {
      case 24: bits = pgm_read_byte_near(&console_font_4x6[(chr * h) + i]); break;
      case 40: bits = pgm_read_byte_near(&console_font_5x8[(chr * h) + i]); break;
      case 48: bits = pgm_read_byte_near(&console_font_6x8[(chr * h) + i]); break;
      case 63: bits = pgm_read_byte_near(&console_font_7x9[(chr * h) + i]); break;
      case 60: bits = pgm_read_byte_near(&console_font_5x12[(chr * h) + i]); break;
      default: return;
    }

    for (int j = 0; j < w; j++) { // character width
      int x0, y0;
      switch (rotate) {
        case -1: x0 = x + (h - 1) - i; y0 = y + (w - 1) - j; break; // -90 deg
        case -2:
        case  2: x0 = x + j;         y0 = y + (h - 1) - i; break; // 180 deg
        case  1: x0 = x + i;         y0 = y + j;         break; // +90 deg
        default: x0 = x + (w - 1) - j; y0 = y + i;         break; // no rotation
      }

      // Skip if drawing off-screen
      if (x0 < 0 || x0 >= cols || y0 < 0 || y0 >= rows) continue;

      // Use unloaded palette for background color based on pixel position (0-255)
      uint16_t _pixel_position;
      if (backgroundGradientHorizontal) {
        // Explicitly handle the last pixel (edge case)
        if (x0 == cols - 1) {
          _pixel_position = 255;
        } else {
          _pixel_position = (x0 * 255) / (cols - 1);  // Horizontal gradient
        }
      } else {
        // Explicitly handle the last pixel (edge case)
        if (y0 == rows - 1) {
          _pixel_position = 255;
        } else {
          _pixel_position = (y0 * 255) / (rows - 1);  // Vertical gradient
        }
      }

      RgbcctColor bgCol  = tkr_anim->GetColourFromUnloadedPalette2(
        backgroundPaletteId, _pixel_position,
        PALETTE_INDEX_SPANS_SEGLEN_ON,  // Scale across the segment length
        PALETTE_WRAP_ON,
        PALETTE_DISCRETE_OFF
      ); // Get the background color from the palette

      bgCol.setBrightness( speed );

      backgroundColor = bgCol.WithBrightness().getU32();

      // Calculate the character color based on the gradient logic
      uint32_t charColor;
      if (solidPerChar) {
        // Use a solid color for the entire character
        charColor = GetPaletteColour(chr, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF).getU32();
      } else {
        // Apply a gradient either horizontally or vertically
        if (horizontalGradient) {
          charColor = GetPaletteColour(constrain((x0 * 255 / cols), 0, 255), PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF).getU32();
        } else {
          charColor = GetPaletteColour(constrain((y0 * 255 / rows), 0, 255), PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF).getU32();
        }
      }

      // Set either character or background color based on the font bit
      if (((bits >> (j + (8 - w))) & 0x01)) { // bit set, draw character
        setPixelColorXY(x0, y0, charColor);
      } else { // no bit set, draw background
        setPixelColorXY(x0, y0, backgroundColor);
      }
    }
  }
}







#define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
void mAnimatorLight::Segment::wu_pixel(uint32_t x, uint32_t y, CRGB c) {      //awesome wu_pixel procedure by reddit u/sutaburosu
  if (!isActive()) return; // not active
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int i = 0; i < 4; i++) {
    CRGB led = getPixelColorXY((x >> 8) + (i & 1), (y >> 8) + ((i >> 1) & 1));
    led.r = qadd8(led.r, c.r * wu[i] >> 8);
    led.g = qadd8(led.g, c.g * wu[i] >> 8);
    led.b = qadd8(led.b, c.b * wu[i] >> 8);
    setPixelColorXY_CRGB(int((x >> 8) + (i & 1)), int((y >> 8) + ((i >> 1) & 1)), led);
  }
}
#undef WU_WEIGHT

#endif // WLED_DISABLE_2D


#endif //USE_MODULE_LIGHTS_ANIMATOR

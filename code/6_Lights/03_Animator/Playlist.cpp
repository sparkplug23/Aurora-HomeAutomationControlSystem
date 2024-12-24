#include "mAnimatorLight.h"

#ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS

void mAnimatorLight::shufflePlaylist() 
{

  ALOG_HGL(PSTR("shuffleplaylist"));
  return; //block this

  int currentIndex = playlistLen;
  PlaylistEntry temporaryValue;

  // While there remain elements to shuffle...
  while (currentIndex--) {
    // Pick a random element...
    int randomIndex = random(0, currentIndex);
    // And swap it with the current element.
    temporaryValue = playlistEntries[currentIndex];
    playlistEntries[currentIndex] = playlistEntries[randomIndex];
    playlistEntries[randomIndex] = temporaryValue;
  }
  DEBUG_PRINTLN(F("Playlist shuffle."));

}


void mAnimatorLight::unloadPlaylist() 
{

  if (playlistEntries != nullptr) {
    delete[] playlistEntries;
    playlistEntries = nullptr;
  }
  currentPlaylist = playlistIndex = -1;
  playlistLen = playlistEntryDur = playlistOptions = 0;
  DEBUG_PRINTLN(F("Playlist unloaded."));

}


int16_t mAnimatorLight::loadPlaylist(JsonObject playlistObj, byte presetId) 
{

  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK   "mAnimatorLight::loadPlaylist"));

  unloadPlaylist();

  JsonArray presets = playlistObj["ps"];
  playlistLen = presets.size();
  if (playlistLen == 0) return -1;
  if (playlistLen > 100) playlistLen = 100;

  playlistEntries = new PlaylistEntry[playlistLen];
  if (playlistEntries == nullptr) return -1;

  byte it = 0;
  for (int ps : presets) 
  {
    ALOG_INF(PSTR("mAnimatorLight::loadPlaylist: ps=%d"), ps);
    if (it >= playlistLen) break;
    playlistEntries[it].preset = ps;
    it++;
  }

  it = 0;
  JsonArray durations = playlistObj["dur"];
  if (durations.isNull()) 
  {
    ALOG_INF(PSTR("mAnimatorLight::loadPlaylist: durations.isNull()"));
    playlistEntries[0].dur = playlistObj["dur"] | 100; //10 seconds as fallback
    it = 1;
  } 
  else 
  {
    for (int dur : durations) 
    {
      ALOG_INF(PSTR("mAnimatorLight::loadPlaylist: durations.dur %d"), dur);
      if (it >= playlistLen) break;
      playlistEntries[it].dur = (dur > 1) ? dur : 100;
      it++;
    }
  }

  for (int i = it; i < playlistLen; i++) playlistEntries[i].dur = playlistEntries[it -1].dur;

  it = 0;
  JsonArray tr = playlistObj[F("transition")];
  if (tr.isNull()) 
  {
    playlistEntries[0].tr = playlistObj[F("transition")] | (transitionDelay / 100);
    it = 1;
  } 
  else 
  {
    for (int transition : tr) 
    {
      if (it >= playlistLen) break;
      playlistEntries[it].tr = transition;
      it++;
    }
  }
  for (int i = it; i < playlistLen; i++) playlistEntries[i].tr = playlistEntries[it -1].tr;

  int rep = playlistObj[F("repeat")];
  bool shuffle = false;
  if (rep < 0) 
  { //support negative values as infinite + shuffle
    rep = 0; shuffle = true;
    ALOG_INF(PSTR("Shuffle started ======================================"));
  }

  playlistRepeat = rep;
  if (playlistRepeat > 0) playlistRepeat++; //add one extra repetition immediately since it will be deducted on first start
  playlistEndPreset = playlistObj["end"] | 0;
  // if end preset is 255 restore original preset (if any running) upon playlist end
  if (playlistEndPreset == 255 && currentPreset > 0) playlistEndPreset = currentPreset;
  if (playlistEndPreset > 250) playlistEndPreset = 0;
  shuffle = shuffle || playlistObj["r"];
  if (shuffle) playlistOptions += PL_OPTION_SHUFFLE;

  currentPlaylist = presetId;
  DEBUG_PRINTLN(F("Playlist loaded."));
  

  




  return currentPlaylist;

}


void mAnimatorLight::SubTask_Playlist() 
{


  static unsigned long presetCycledTime = 0;

  // if fileDoc is not null JSON buffer is in use so just quit
  if (currentPlaylist < 0 || playlistEntries == nullptr || pCONT_mfile->fileDoc != nullptr) 
  {
    // ALOG_INF(PSTR("SubTask_Playlist return early"));    
    return;
  }


  if(mTime::TimeReached(&tSaved_playlist_debug, 1000)){
    uint32_t remaining = millis() - presetCycledTime;
    remaining /= 1000;
    ALOG_INF(PSTR("Playlist%d %d/%d secs, Preset %d/%d %d"), currentPlaylist, remaining, playlistEntryDur/10, playlistIndex, playlistLen, playlistEntries[playlistIndex].preset);
    // ALOG_INF(PSTR("playlistIndex i%d/%d"), playlistIndex, playlistLen);
    //ALOG_INF(PSTR("presetCycledTime %d<%d %d"), millis() , presetCycledTime, (100*playlistEntryDur) );
  }


  if (millis() - presetCycledTime > (100*playlistEntryDur)) 
  {

    // ALOG_INF(PSTR("tSaved_playlist_debug Running"));//, currentPlaylist, playlistEntryDur);

    presetCycledTime = millis();
    // if (pCONT_iLight->getBri_Global() == 0 || nightlightActive) return;

    ++playlistIndex %= playlistLen; // -1 at 1st run (limit to playlistLen)

    // playlist roll-over
    if (!playlistIndex) 
    {
      if (playlistRepeat == 1) { //stop if all repetitions are done
        unloadPlaylist();
        if (playlistEndPreset) applyPreset(playlistEndPreset);
        return;
      }
      if (playlistRepeat > 1) playlistRepeat--; // decrease repeat count on each index reset if not an endless playlist
      // playlistRepeat == 0: endless loop
      // if (playlistOptions & PL_OPTION_SHUFFLE) shufflePlaylist(); // shuffle playlist and start over
    }

    jsonTransitionOnce = true;
    playlistEntryDur = playlistEntries[playlistIndex].dur;
    // AddLog_Array(LOG_LEVEL_INFO, "preset arr", playlistEntries[playlistIndex].preset, 10);
    for(int i=0;i<10;i++){ Serial.println(playlistEntries[i].preset);};
    applyPreset(playlistEntries[playlistIndex].preset);

  }

  /**
   * @brief 
   
   "94":{
    "playlist":{
      "ps" :[ 32,  34,  64, 65,   66,  74,  75,  76,  77,  79,  80],    // the preset index to load
      "dur":[300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300],    // the duration, in 100ths of ms to load. So 300*100 is 30000ms or 30 seconds
      "repeat":0,"end":0,"r":1,                                         // repeat 0 means forever, repeat number means an amount of times. "end" would be the last preset to stop at. 
      "time_window":{  // If only one in array, then its the whole playlist limit. If the same length as the playlist, then each entry is the limit for that preset
        "start":[1600, 1830],  // as HHMM only
        "end":[1700, 1900]  // as HHMM only
      }    
    },
    "on":true,
    "n":"Meteor Colours"
  }





   * 
   */

}

/***
 * Playlist meanings
 * 
 * "repeat": [count] -1 means indefinitely, 0 to end is how many times
 * "dur": [array] is the tengths of seconds
 * "r":   [??] in this case appears to mean shuffle enabled, but not sure
 * "end": [index] if repeat causes the playlist to end, then this is the index for the preset/playlist to switch to
 * "ps":  [array] list of presets to load
 * "transition": [array] no longer used, no WLED transitions remained
 * 
 */
void mAnimatorLight::serializePlaylist(JsonObject sObj) 
{

  JsonObject playlist = sObj.createNestedObject(F("playlist"));
  JsonArray ps = playlist.createNestedArray("ps");
  JsonArray dur = playlist.createNestedArray("dur");

  playlist[F("repeat")] = (playlistIndex < 0) ? playlistRepeat - 1 : playlistRepeat; // remove added repetition count (if not yet running)
  playlist["end"] = playlistEndPreset;
  playlist["r"] = playlistOptions & PL_OPTION_SHUFFLE;

  for (int i=0; i<playlistLen; i++) 
  {
    ps.add(playlistEntries[i].preset);
    dur.add(playlistEntries[i].dur);
  }

}


#endif // ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
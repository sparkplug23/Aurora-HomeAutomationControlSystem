# Changelog
All notable changes to this project will be documented in this file.

Section Next, will always be "Development", which with copy and paste will become the version below it as Released (uploaded to github)


## [Unreleased] - Development

## [D0.117.0.0] - Created 12-AUG-23
### Milestone Point
Introduction of Lighting DevelopingByTypes for garage as debug suite.

### Added
- template

### Breaking Changed
- template

### Changed
- template

### Fixed
- templete

### Removed
- template


## [Unreleased] - Development

## [D0.116.0.0] - Created 29-JUN-23
### Milestone Point
Created with Multipin support on I2S (dual neopixel) and PWM. Introduction of TestGroups.

### Added
- PWM support with segments
- tested 5, 3, 2, 1 channel (rgbcct, rgb, ww_wc, white)
- RGB (3 pins) as one segment and CW/WW as second segment
- Two CW/WW channels (two 2 pins)
- Colour (hue/sat) for RGB, and CCT Temp for white warm to cold.
- Five white only outputs, as "pixels" 1-5 in a single segment. Visually the same as 5 ws2812b pixels but really 5 unique pins
- Includes per segment brightness control, and a "master" brightness
- per segment auto timeoff (eg. turn segment on for X seconds then turn off, that time also can be set up - to 1 minute of turning off)
- All on the h801

### Breaking Changed
- template

### Changed
- template

### Fixed
- templete

### Removed
- template

## [D0.113.0.0] - Created 30-12-2022
### Milestone Point
Created for the merge of all lighting branches into unified development branch for start of 2023

### Added
- template

### Breaking Changed
- template

### Changed
- template

### Fixed
- templete

### Removed
- template


## [Unreleased] - Development

## [B0.86.0.0]
### Added
- 

### Breaking Changed
- 

### Changed
- 

### Fixed
- Error with `encoded_custom` palette parsing

### Removed
- none

## [Unreleased] - Development

## [B0.84.84.114]
### Added
- first addition of WLED effects
- five variable rgbcct colour palettes, with scenes now selecting from them when static
- New "sunrise" effects added, although not completed
- Added operator functions to neopixellib for adding/subtracting colours to `RgbColor` and `RgbcctColor`

### Breaking Changed
- `WebServer` will remain disabled for most devices until it gets another optimised refactor
- `Scenes` as a stand alone mode will be removed, or remain as a mode that will configure `Effects` into the right format
- `Scene` Commands will no longer work, and will require using effect `Solid Palette` and `Solid RGBCCT` for the h801 hardware

### Changed
- move rgb variable user names to be part of device list 
- `internal_scene_colour` is now an pointer, which points to a user variable RgbcctColor. This can be switched, with a new variable tracking which is active (needs to change with palette_id setting)
- `Scene` and `Flasher` to become a new ``AnimationMode`` (ex. `Palettes`, `Effects`, `Animations`)
- `Effects` is the new animation of any effects

### Fixed
- 

### Removed
- none


## [Unreleased] - Development

## [B0.83.80.109]
### Added
- Custom light version for first beta tester
- Ifdef to disable additional webpages (beyond root) and the related buttons for stability testing of root page
- compression of OTA using python script
- Ability to disable webserver, mqtt, ntp while still enabling system to run via defaults in progmem (eg outside tree lighting)

### Breaking Changed
- esp32 neopixel not working with wifi
- Disabled webserver 

### Changed
- refactored ``ini`` files to allow esp8266 and esp32 compiles
- ``INFO_PARSING`` is now reffered to as ``COMMANDS`` as values are set with `CommandSet_x` functions to consolate `AddLog`
- `AnimationProfile` function/enum list changed to `SetLightPowerState`
- Refactored commands in lightinterface and animator (to be renamed `mRGBAddressable`)
- Created more files (cpp) under lighting to split code segments out for easier reading
- Cleaned `LanguageDefaults`

### Fixed
- RTC code to use interrupts, eliminating time drift

### Removed
- none



## [Released - Uploaded to Github]

## [9.0.0.3] - 18-12-2020 - Template for subheadings
### Added
- template

### Breaking Changed
- template

### Changed
- template

### Fixed
- templete

### Removed
- template

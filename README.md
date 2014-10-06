# Oral Histories Solution Pack

## Introduction

Adds all required Fedora objects depending on video or audio file uploaded and TRANSCRIPT datastream through the Islandora interface.
Displays transcript content along with video or audio file.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Video Solution Pack](https://github.com/Islandora/islandora_solution_pack_video)
* [Islandora Audio Solution Pack](https://github.com/Islandora/islandora_solution_pack_audio)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Select a viewer in Administration » Islandora » Oral Histories Solution Pack (admin/islandora/solution_pack_config/oralhistories) and enable transcript or caption display.
The module currently doesn't support JW Player as viewer.

## Notes

**Caution:** This Solution Pack is currently in early development stage. It only supports transcript file in [WebVTT](http://dev.w3.org/html5/webvtt/) format.
You can use [Microsoft builder](http://ie.microsoft.com/testdrive/Graphics/CaptionMaker/) to produce vtt files quickly or use your text editor to produce
vtt files according to WebVTT standard.

Here is a simple example of vtt file:
```
WEBVTT

00:00.040 --> 00:02.503
<Curry> A rocket is a launch vehicle.

00:02.503 --> 00:07.474
<Alice> It can also be a space capsule, usually in the form of a cylinder or

00:07.474 --> 00:10.577
<Curry> a tubelike structure that launches from the Earth.
```
When naming the vtt file, the last three characters should be a "_" followed by valid BCP 47 two-letter language tag, e.g. mytransctript_en.vtt is an English transcript file.

## Troubleshooting/Issues

When Caption display is true, video.js doesn't behave the same on Firefox and Chrome. On Firefox, it requires to click the caption menu once to start showing.
On Chrome, it displays automatically.

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)


## Maintainers/Sponsors
Current maintainers:

* [Lingling Jiang](https://github.com/sprklinginfo)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.



## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

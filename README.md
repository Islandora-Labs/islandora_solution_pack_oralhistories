# Oral Histories Solution Pack

## Introduction

Adds all required Fedora objects to allow users to ingest and retrieve Oral Histories (video/audio) files through the Islandora interface

**This module requires libfaac. Libfaac is not free for commercial use.**

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* FFmpeg - Compliation guides: [Ubuntu](http://ffmpeg.org/trac/ffmpeg/wiki/CentosCompilationGuide
), [CentOS](https://ffmpeg.org/trac/ffmpeg/wiki/UbuntuCompilationGuide).
* ffmpeg2theora (optional) - [Instructions](https://wiki.duraspace.org/display/ISLANDORA6122/Chapter+12+-+Installing+Solution+Pack+Dependencies)

FFmpeg version 1.1.1 has been tested. It can be downloaded [here](http://www.ffmpeg.org/releases/ffmpeg-1.1.1.tar.gz)

**NOTE**: remove the `--enable-x11grab` flag on the ffmpeg configure.


## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration



## Troubleshooting/Issues

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

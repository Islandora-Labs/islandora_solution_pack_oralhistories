# Oral Histories Solution Pack

**Still in active development, may break sometimes**

## Introduction

Adds all required Fedora objects depending on video or audio file uploaded and TRANSCRIPT and MEDIATRACK datastream through the Islandora interface.
Displays transcript content along with video or audio file.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)
* [Islandora Video Solution Pack](https://github.com/Islandora/islandora_solution_pack_video)
* [Islandora Audio Solution Pack](https://github.com/Islandora/islandora_solution_pack_audio)
* [Transcript UI](https://github.com/sprklinginfo/transcripts_ui)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Select a viewer in Administration » Islandora » Oral Histories Solution Pack (admin/islandora/solution_pack_config/oralhistories) and enable transcript or caption display.
The module currently doesn't support JW Player as viewer.

## Notes

**Caution:** This Solution Pack is currently in development stage. It only supports transcript file in a flat xml format like below:

Here is a simple xml file for transcript:
```
<?xml version="1.0" encoding="UTF-8"?>
<cues>
    <!-- If the entire transcript has one speaker only, use 'solespeaker' element.
         Then skip 'speaker' element in 'cue' element level. But DO NOT use them in both places. 
         If 'solespeaker' element presents in the document, following 'speaker' elements will be ignored. -->
    <solespeaker>One Speaker</solespeaker>
    <cue>
        <speaker>Different Speaker</speaker>
        <!-- 'start' and 'end' elements are start time and end time in seconds for the cue. -->
        <start>0.000</start>
        <end>12.124</end>
        <!-- 'transcript' and/or 'annotation' are default content tiers of the cue. 
              Extra tier(s) can be added as long as they are listed in the configuration page. 
             'transcript' element is required if 'Enable captions/subtitles display' is configured to be true. -->
        <transcript>This is the transcript text content.</transcript>
        <annotation>This is the annotation content.</annotation>
    </cue>

    <!-- add more cues with above structure.-->

</cues>
```

## Configuration

Select configuration options in Administration » Solution pack configuration » Oral Histories Solution Pack (admin/islandora/solution_pack_config/oralhistories).

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

# Oral Histories Solution Pack

**Still in active development, expecting changes or errors from time to time**

## Introduction

Adds all required Fedora objects depending on video or audio file uploaded and TRANSCRIPT / MEDIATRACK datastream through the Islandora interface.
Displays transcript content along with video or audio file.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)
* [Islandora Video Solution Pack](https://github.com/Islandora/islandora_solution_pack_video)
* [Islandora Audio Solution Pack](https://github.com/Islandora/islandora_solution_pack_audio)
* [Transcripts UI](https://github.com/sprklinginfo/transcripts_ui)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Select a viewer in Administration » Islandora » Oral Histories Solution Pack (admin/islandora/solution_pack_config/oralhistories) and enable transcript or caption display.

For "Transcripts UI" module, on Configuration » User Interface » Transcripts UI (admin/config/user-interface/transcripts), "Tiers" and "Speaker names" need be configured based on the transcript xml file in the form of TIER_ID|TIER_NAME.
For example, with simple transcript xml file below the TIER_ID will be or_transcript (xml tag with 'or_' prefix). TIER_NAME could be any text label for users to see: or_transcript|Transcript

## Notes

- We are using 'Bootstrap' compatible markup so it displays everything nicely if you are using [Bootstrap theme](https://www.drupal.org/project/bootstrap). Otherwise you need update your theme accordingly.
- This Solution Pack is currently in development stage. It only supports transcript file in a flat xml format like below:

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
        <!-- 'transcript' and/or 'translation' are default content tiers of the cue. 
              Extra tier(s) can be added as long as they are listed in the configuration page. 
             'transcript' element is required if 'Enable captions/subtitles display' is configured to be true. -->
        <transcript>This is the transcript text content.</transcript>
        <translation>This is the annotation content.</translation>
    </cue>

    <!-- add more cues with above structure.-->

</cues>
```
                                                                                                                                                                        or_annotation|Annotation
## Index transcript fields in Solr

A or_transcript_solr.xslt file (xsl/or_transcript_solr.xslt) is included in the module in order to index transcript.xml fields in Solr.

* Please drop this file to 'islandora_transforms' folder and update the foxmlToSolr.xslt file to include new xslt file.
* Modify solr schema.xml file to add or.* fields created from transcript.xml file.

```xml
<dynamicField name="or_*" type="text" indexed="true" stored="true" multiValued="true"/>
```
Please restart the solr application with new schema. 

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

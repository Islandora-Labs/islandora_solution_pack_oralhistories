# Oral Histories Solution Pack [![Build Status](https://travis-ci.org/digitalutsc/islandora_solution_pack_oralhistories.svg?branch=master)](https://travis-ci.org/digitalutsc/islandora_solution_pack_oralhistories)

**Still in active development, expecting changes or errors from time to time**

## Introduction

Adds all required Fedora objects depending on video or audio file uploaded and TRANSCRIPT / MEDIATRACK datastream through the Islandora interface.
Displays transcript content along with video or audio file.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Chaos Tools Suite](https://www.drupal.org/project/ctools)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)
* [Islandora Video Solution Pack](https://github.com/Islandora/islandora_solution_pack_video)
* [Islandora Audio Solution Pack](https://github.com/Islandora/islandora_solution_pack_audio)
* [Transcripts UI](https://github.com/sprklinginfo/transcripts_ui)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration
### Transcripts UI

In Configuration > User Interface > Transcripts UI (admin/config/user-interface/transcripts), "Tiers" and "Speaker names" need be configured based on the transcript xml file. It will look like this: TIER_ID|TIER_NAME.

For example, in the example transcript xml file below, the TIER_ID will be `or_transcript` (xml tag with 'or_' prefix). TIER_NAME could be any text label that users will see, such as `Transcript`.
Together they should look something like: `or_transcript|Transcript`.

There are a number of options included by default; you'll want to get rid of most of them. At MSUL, we have:
#### Transcript Tiers
| All tiers* | |
| ----- | ----- |
| or_transcript | English |

| Hidden tiers | |
| ----- | ----- |
| | |
This tier is empty

| Speaker names* | |
| ----- | ----- |
| or_speaker | Speaker |

* Entering at least one value here is required, or **your transcripts will not display**

You must add all TIER_ID|TIER_NAME pairs relevant to your collection to the configuration, as different transcript files may have different tiers (xml tags) and tier_names.

### Oral Histories Module

There are some configuration options in Administration » Islandora » Oral Histories Solution Pack (admin/islandora/solution_pack_config/oralhistories) page:

* Check the boxes which are relevant to your use; these might include:
  * "Create WEBVTT file for captions or subtitles" -- if you'd like the module to automatically create a caption file
  * "Enable captions/subtitles display" -- if you'd like to display the captions
  * "Enable transcript display" -- **this one is the most important** because it's what allows your transcript scroller to display
  * "Display media and transcript side-by-side" -- if you don't like the default display of having the scrolling transcript below the video/audio player, check this to move the scroller to the side
* The 'Annotation' tab is still in early development stage which should be disabled on a production server.

## Notes

**Caution:** This Solution Pack is currently in development stage. It only supports transcript files in a format like this one:

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

Once you have chosen xml tags for your use case, please make sure the structure of the child elements in <cue> tag are consistent **in the same transcript xml file**, even if some child elements are empty.
This will ensure sure those child elements are indexed with the correct sequence numbers so they can be assembled properly when displaying on 'Transcript' tab.


## Index transcript fields in Solr

An or_transcript_solr.xslt file (xsl/or_transcript_solr.xslt) is included in the module in order to index transcript.xml fields in Solr.

* Please copy this file to 'islandora_transforms' folder. Your institution/the folks running your instance will have made decisions about where this folder lives, so you'll want to follow those best practices for your site.  
* Update the foxmlToSolr.xslt file to include new xslt file.
* Modify solr schema.xml file to add or.* fields created from transcript.xml file.
* **Note** On our instance, we use type="text" for those transcript fields (as shown in the example below). You need use a correct field type based on your solr instance.

```xml
<dynamicField name="or_*" type="text" indexed="true" stored="true" multiValued="true"/>
```
* Please restart the solr application with new schema.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)


## Maintainers/Sponsors
Current maintainers:
* [Marcus Barnes](https://github.com/MarcusBarnes)
* [Nat Kanthan](https://github.com/Natkeeran)

## Contributors
* [Lingling Jiang](https://github.com/sprklinginfo)
* Edward Garrett

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.



## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

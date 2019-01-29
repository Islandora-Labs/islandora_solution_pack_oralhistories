# Oral Histories Solution Pack 
[![DOI](https://zenodo.org/badge/23933503.svg)](https://zenodo.org/badge/latestdoi/23933503) [![Build Status](https://travis-ci.org/Islandora-Labs/islandora_solution_pack_oralhistories.svg?branch=master)](https://travis-ci.org/Islandora-Labs/islandora_solution_pack_oralhistories) 

## Introduction

Provides a content model for Oral Histories and a viewer for displaying timed text content (XML or WebVTT) alongside video and audio files. 

The software leverages the existing Video and Audio solution packs to create derivatives from source files, and provides the ability to append a transcript and closed captioning file that is then exposed in a video.js based player. Transcripts can contain multiple tiers corresponding to different languages and speakers. The module is designed to acommodate any time-encoded description of a video or audio file. 

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Chaos Tools Suite](https://www.drupal.org/project/ctools)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)
* [Islandora Video Solution Pack](https://github.com/Islandora/islandora_solution_pack_video)
* [Islandora Audio Solution Pack](https://github.com/Islandora/islandora_solution_pack_audio)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

A fork of Pinedrop's [Transcripts UI](https://github.com/pinedrop/transcripts_ui) has been included as a submodule.  As part of the installation, you will need to enable this submodule either through Drupal's module administration interface or via drush on the command line.

## Configuration

### Solr

The module configuration requires a modification to the Gsearch XSLT in order to index XML and WebVTT datastreams. Information for how to do this is provided in the [project wiki](https://github.com/digitalutsc/islandora_solution_pack_oralhistories/wiki).

### Transcripts UI

Navigate to Islandora » Solution Pack Configuration » Oral Histories Solution Pack and then click the TRANSCRIPTS UI tab. (admin/islandora/solution_pack_config/oralhistories/transcripts). "Tiers" and "Speaker names" need be configured based on the transcript xml file. It will look like this: TIER_ID|TIER_NAME.

For example, in the example transcript xml file below, the TIER_ID will be `or_transcript` (xml tag with 'or_' prefix). TIER_NAME could be any text label that users will see, such as `Transcript`.
Together they should look something like: `or_transcript|Transcript`.

There are a number of options included by default; you'll want to get rid of most of them.

* Entering at least one value here is required, or **your transcripts will not display**

You must add all TIER_ID|TIER_NAME pairs relevant to your collection to the configuration, as different transcript files may have different tiers (xml tags) and tier_names.

### Oral Histories Module

There are some configuration options in Administration » Islandora » Oral Histories Solution Pack (admin/islandora/solution_pack_config/oralhistories) page:

* Check the boxes which are relevant to your use; these might include:
  * "Create WEBVTT file for captions or subtitles" -- if you'd like the module to automatically create a caption file
  * "Enable captions/subtitles display" -- if you'd like to display the captions
  * "Enable transcript display" -- **this one is the most important** because it's what allows your transcript scroller to display
  
### Themes

The viewer currently works with the [Bootstrap theme](https://www.drupal.org/project/bootstrap). For non-Bootstrap themes, including certain Bootstrap-based themes,  additional files are provided to enable bootstrap elements required by the viewer. View the [project wiki](https://github.com/digitalutsc/islandora_solution_pack_oralhistories/wiki) for additional details. 


## Transcript File Formats

This module supports transcript files in [WebVTT](https://w3c.github.io/webvtt/) and in a custom  XML format. For additional information on ingest processes, visit our [project wiki](https://github.com/digitalutsc/islandora_solution_pack_oralhistories/wiki#ingest-behaviour-for-xml-vs-webvtt-transcripts). The XML format should look similar to the following:

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

### Permissions

In order to view transcripts in the user-interface, double check that anonymous and authenticated users have the `Islandora - View repository objects` permission enabled.  You can check via the Drupal administration user-interface `People > Permissions`.

### Maintainers/Sponsors
Current maintainers:
* [Marcus Barnes](https://github.com/MarcusBarnes)
* [Nat Kanthan](https://github.com/Natkeeran)
* [Megan Kudzia](https://github.com/mkudzia)

Sponsors:
* The [Digital Scholarship Unit (DSU)](https://www.utsc.utoronto.ca/digitalscholarship/) at the University of Toronto Scarborough Library

Contributors:
* [Lingling Jiang](https://github.com/sprklinginfo)
* Edward Garrett

Note: This module would not have been possible without the support of [Chris Berkowitz](http://www.utsc.utoronto.ca/hcs/christine-berkowitz) from the Scarborough Oral Histories Project at the [Department of Historical and Cultural Studies](http://www.utsc.utoronto.ca/hcs/) at UTSC.

## Contributing

We welcome community development partners.  The [CONTRIBUTING.md](https://github.com/digitalutsc/islandora_solution_pack_oralhistories/blob/7.x/CONTRIBUTING.md) file provides guidelines on how you can contribute to the development of the Islandora Oral Histories Solution Pack.

## License

[GNU General Public License, version 3](http://www.gnu.org/licenses/gpl-3.0.txt) or later.

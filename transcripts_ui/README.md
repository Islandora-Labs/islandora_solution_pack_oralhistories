# Transcripts UI

## Introduction

Transcripts UI provides a themeable interface for interacting with
timecoded transcripts in Drupal. The module assumes a very simple
data model for transcripts, which may work for you. A transcript is
divided into a number of Time Code Units (TCUs). Each TCU has an
optional speaker in one or more transliteration formats, a start
time, and an end time. In addition, each TCU has any number of
"tiers". Tiers can be anything, but by convention they are things
like:

* a transcription of what the speaker said
* a translation of what the speaker said
* a morpheme-by-morpheme gloss of what the speaker said

The acronym "TCU" was inspired by its use within the
[TalkBank](http://talkbank.org/) system.

## Installation & Configuration

Enable the transcript_ui module.

Go to admin/config/user-interface/transcripts, and define
a list of tier identifiers and tier names. On that page you
can also define a list of speaker display formats, also in
key|label notation.

Transcripts UI generates 'Bootstrap' compatible markup, if you are
using the [Bootstrap theme](https://www.drupal.org/project/bootstrap)
with the [Bootstrap framework](http://getbootstrap.com/). Disregard
the Bootstrap classes if you are using another theme.

## Creating a UI

* To create a UI, pass your module's name along with a unique transcript identifier
and some options to transcripts_ui_ui().
* To render a UI, pass it to transcripts_ui_render().

## UI Components

Transcripts UI renders an HTML element for each of its components.
Each component element has a data-transcripts-id attribute set to
the trid. Each component also has a data-transcripts-role attribute
set to one of the following roles.

* transcript: the element in which the transcript appears.
* transcript-controls: controls that alter the appearance of the
transcript, such as tier controls and display mode controls, as
well as controls for AV playback, including buttons to play the
previous, same, and next TCUs.

Your module is responsible for providing markup including an
HTML 5 audio or video tag contained within an element that has the
following data attributes, where TRID is a transcript id shared by
other UI components. Without making this linkage, your transcript
will not be synced with the media.

* data-transcripts-id=TRID
* data-transcripts-role=video

The default way of linking a transcript to its video is illustrated
by this [JavaScript file](https://github.com/pinedrop/transcripts_ui/blob/master/js/transcripts_ui.js).

To override the default, implement [hook_transcripts_ui_js_alter](https://github.com/pinedrop/transcripts_ui/blob/master/transcripts_ui.api.php).
For example, Shanti UVA's Mediabase implements the hook in order to
grab a video tag from an iFrame introduced by the [Kaltura player](https://github.com/shanti-uva/drupal_mediabase/blob/newtheme/mb_kaltura/js/transcripts_ui.js).

Your module is also responsible for retrieving transcripts for the
UI; this is done by implementing hook_transcripts_ui_transcript().

Your module is then free to position and style the rendered components.

## Themeing Output

Components and sub-components can be differently themed by 
(a) overriding theme functions, or (b) altering render arrays.
Render arrays can be altered in many ways, including adding or replacing
css and js files. For more on the 
[Scary Render Array](http://cocoate.com/ddbook/scary-render-array), see
[Render Arrays in Drupal](https://www.drupal.org/node/930760). 

## Transcript Search

Implementations of hook_transcripts_ui_transcript() may return highlights
in addition to the full transcript, if a term search has occurred.
A search box appears within the "transcript-controls" component. Clicking on the
search button sends an AJAX request to the hook implementation, which
refreshes the transcript with new search results.

## Related Modules

[transcripts_apachesolr](https://github.com/pinedrop/transcripts_apachesolr)
is one module that makes use of transcripts_ui.

## License

[GPL, version 2 or later](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

## Acknowledgement

Pinedrop's work on Transcripts UI has been sponsored by:

* The [Digital Scholarship Unit (DSU)](https://www.utsc.utoronto.ca/digitalscholarship/)
at the UTSC Library.
* [SHANTI](http://shanti.virginia.edu/) at the University of Virginia.

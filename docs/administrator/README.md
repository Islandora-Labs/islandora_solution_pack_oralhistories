@todo/ tocheck

**Should we make separate documents to the readme.md document and link
to them from the readme to avoid a crazy long readme? If so, what should
the split look like?**

**README.md - typical islandora README**

**Includes release notes**

> DATE:
>
> Shoudl write something about the syncing of the MEDIATRACK and
> TRANSCRIPT streams if they are uploaded separately and contain
> separate content. We shoudl also try to do this and document what
> happens if you try to upload separate content and then edit TRANSCRIPT
> datastream
>
> confirmed that you can upload a separate mediatrack not derived from
> transcript (and that CC will show mediatrack different from
> transcript)
>
> confirmed that if you edit the transcript it will overwrite the
> mediatrack
>
> Document the support of WEBVTT and limitations






### Introduction for Administrators

The Oral History Solution Pack is a set of applications developed by the
Library’s Digital Scholarship Unit with early support from the
Scarborough Oral Histories Project (PI Chris Berkowitz) at the
University of Toronto Scarborough. The software leverages the existing
Video and Audio solution packs to create derivatives from source files,
and provides the ability to append a transcript and closed captioning
file that is then exposed in a video.js based player. Transcripts can
contain multiple tiers corresponding to languages, speakers,
annotations, etc.

Transcripts are produced in any application that will support the export
of delimited text files containing the following elements:

-   A legible timecode

-   One or more speakers

-   Plain text

The application does not currently support

-   In-text markup (such as html styling)

-   Overlapping tiers

Administration of the module
----------------------------

The module is administered from
**admin/islandora/solution\_pack\_config/oralhistories** by those with
appropriate permissions. From the screen, administrators can set the
following options:

-   Create WEBVTT file for captions or subtitles.

    -   Will create WEBVTT file from transcript xml file based on
        > "transcript" tier.

-   Enable captions/subtitles display

    -   Display captions or subtitles on video player when WEBVTT
        > file exists.

-   Enable transcript display

    -   Display transcript synchronously with the video or audio.

-   Display media and transcript side-by-side

    -   By default, the tran script will be displayed beneath the
        > media object.

-   Display annotation tab

    -   By default, the annotation tab will not be displayed.

File structure and naming conventions for batch ingest
------------------------------------------------------

The Islandora Oral Histories solution pack only permits batch ingest of
video files (not audio files) In order for files to be batch ingested,
they must follow a strict directory structure and naming conventions as
follows:

**Through the interface:**

**For Video Files**

I. Video file (Allowed file types: mp4, mpv, m4v, avi, mkv)

Ii. TRANSCRIPT.xml

Iii. MODS.xml

Iv. Thumbnail (Optional. Allowed file types: png, jpg)

**For Audio Files**

I. Audio file (Allowed file types: **wav mp3)**

Ii. TRANSCRIPT.xml

Iii. MODS.xml

Iv. Thumbnail (Optional. Allowed file types: png, jpg)

**Using the Fedora Video Ingesting Script:**

**For Video Files**

i\. MOV video stitched original file

ii\. MP4 (derivative of the original file)

iii\. TRANSCRIPT.xml (optional)

iv\. MODS.xml (metadata)

vi\. TN.jpg (thumbnail)

**For Audio Files**

I. MP3, AIFF, WAV original file (it cannot be larger 1GB)

ii\. TRANSCRIPT.xml (optional)

iii\. MODS.xml

iv\. TN.jpg (thumbnail)

> Notes:

-   Select a good thumbnail (you can screenshot your video, or your
    > transcription software might provide you with an option to save
    > a frame. For instance, in Inqscribe, go to File &gt; Save Current
    > Frame as JPEG, Save the image in a folder for the interview you're
    > working on.)

-   Dimensions of thumbnail 640 × 360

-   If your original video file isn’t an MOV file, transform it into a
    > .mov file

###  {#section-3}

### File Naming Conventions

All video and audio files should have the same name as the original
video or audio file. Otherwise the files should be named:

-   TN.jpg - For the Thumbnail

-   TRANSCRIPT.xml - For the transcript

-   MODS.xml - For the Metadata

### Directory structure for batch ingest

> Create folders for each interview

**For video files only**

FOLDER\_FOR\_INGEST/

> Interview\_01/
>
> Interview\_01.mov
>
> Interview\_01.mp4
>
> TN.jpg
>
> MODS.xml
>
> Interview\_02/
>
> Interview\_02.mov
>
> Interview\_02.mp4
>
> TN.jpg
>
> MODS.xml
>
> Interview\_03/
>
> Interview\_03.mov
>
> Interview\_03.mp4
>
> TN.jpg
>
> MODS.xml

Generating Transcripts
======================

In this Section:

-   [***Choosing your software***](#choosing-your-software)

    -   [***Inqscribe***](#inqscribe)

    -   [***Audacity***](#_7vt2rn84sare)

    -   [***Youtube***](#youtube)

    -   [***oTranscribe***](#otranscribe-beta)

-   [***Transcription Best Practices***](#_sct1duoexw5d)

-   [***Transcribing with Inqscribe***](#transcribing-with-inqscribe)

-   [***Creating a Transcript in > Audacity***](#creating-transcripts-with-audacity)

### Choosing your software

This documentation covers the process of creating transcripts produced in Inqscribe, or in the Open Source Audacity. Other available tools for creating transcripts are YouTube and oTranscribe.

#### Inqscribe

With the software of choice, create transcripts to videos. At UTSC, we use Inqscribe which can be purchased with an academic or student discount. [*https://www.inqscribe.com/buy.html*](https://www.inqscribe.com/buy.html).

#### Audacity

Audacity is an open audio editing and recording program. Transcribing is possible in this program. You can transcribe the audio files as well as the audio from a video file (MOV/ MP4), but you will not be able to view the video in the process. This is a drawback if you believe there are important actions that need to be expressed in the text. Audacity is the top transcription choice of the DSU, if the paid for software Inqscribe is unavailable i.e. a classroom setting.

#### YouTube

YouTube allows transcripts to be created for uploaded videos. The transcripts can be downloaded as .srt, .vtt and sbv files. As YouTube does not export transcripts as delimited files there is an added intermediate step of converting the file in OpenRefine. These can then be processed in openrefine to produce XML files.

#### oTranscribe (BETA)

oTranscribe allows for the transcribing of recorded interviews. There is a [*web based*](http://otranscribe.com/) tool and a [*downloadable tool*](https://github.com/oTranscribe/oTranscribe). The downloadable tool takes some setup.


### Creating Transcripts with Inqscribe

Figure (1) shows the example of a good transcript created through
Inqscribe. Best practices include:

-   Each line of transcription begins and is closed with a time stamp. In between these two timestamps the enter key is not used.

-   Only use the enter key to begin a new line of transcript

-   Each line of transcript lists the speaker followed by : ex. Speaker1: (Unless there is a sole speaker in the interview. See the [*Recording Speakers*](#recording-speakers) section for more information)

Figure 1: A good transcription in Inqscribe

[00:00:00.00] Daku Sherpa: The transcription text goes here on the
same line. Never start a new line or a new paragraph. Always have a
closing time stamp.[00:06:32.12]
[00:06:32.12] Interviewer: More transcription goes here.
[00:09:10.45]
[00:09:10.45] No speaker: (Pause) Daku sips his drink.
[00:20:11.15]
[00:20:11.15] Interviewer: So did you ever end up going back there?
[00:25:10.45]
[00:27:11.15] Unknown: It's too cold there. [00:30:10.45]

#### The Inqscribe Window

Figure x shows the a sample view of a video and a transcript in the
Inqscribe window.

![](media/image07.png)

1.  The video play window

2.  The buttons and tools used to start, stop and change the time in videos. You can choose a specific time through the timescale ruler. You are also able to use the slider to change the play rate making the video slower for transcription.

3.  The window where you type the physical transcription. Properly formatted timestamps are blue. If the timestamp is not blue that shows there is an error in formatting.

#### Steps for Creating a Transcript in Inqscribe

To create transcripts in Inqscribe:

1.  Open the Inqscribe program.

2.  Drag and drop the video file that you want to transcribe into the grey box located on the top left of the program that reads “Drag a media file into this window”. For video use only .mov or .mp4 files.

3.  Before you begin to transcribe the video you must insert a timestamp. To do this go to Edit &gt; Insert Time (it will insert a timestamp in the format of \[hh:mm:ss.ss\]). You can also use the hotkey CMD + ; to insert the timestamp.

4.  Type your transcribed text

5.  Insert a timestamp at the end of the line of transcribed text by going to Edit &gt; Insert Time

6.  Repeat on new line to continue transcription.

7.  Optional: When recording the audio of multiple speakers you can create create snippets that insert the timestamp and speaker's name. This will also be useful to for inserted repeating block of text. To do this go to Edit &gt; Insert Snippet and a box titled “snippets” will appear. To add a new snippet to the list click the “edit…” button. To Insert a snippet into the transcript click the “insert…” button. The image of the snippet editor (figure x) can be seen below. Snippets are entered with the formula: {\$TIME} The text you want in the transcript. An example of a snippet that identifies the speaker is: {\$TIME} Speaker 1

![](media/image22.png)

**Inqscribe transcription tips:**

-   Explore the different options offered by the program to make transcribing easier for you.

-   Changing the time in the timestamp is possible. As long as you maintain the correct format (Inqscribe highlights a valid timestamp in blue).

-   You can adjust play rate if you need to listen to a speaker carefully

#### Steps for exporting your Inqscribe transcript as a tab delimited file

Once you have completed your transcript in Inqscribe you must export it
as a Tab delimited file:

1.  Save you work by going to file &gt; Save as. Save the interview transcript with the same filename as the Interview Video format and put it in the appropriate directory and file folder.

2.  Go to File &gt; Export &gt; Tab-delimited text. The Export Settings box will open (see figure below)

3.  Ensure that the “Export Out Points” and the “Export Speaker Names” boxes are checked. In addition ensure that the Speaker Name Delimiter box has a : )See below for example).

![](media/image10.png)

4.  Input the name of your file the Target box. Use the choose… button to select the appropriate file folder directory. Then click Export.

**Creating Snippets for Shortcuts to Create Transcripts:**

In the transcript example below there are multiple speakers. The snippet
feature helps the transciber keep track of all the possible speaker
names.

![](media/image19.png)

### Creating Transcripts with Audacity

Audacity is mainly a sound editing software, but it is possible to
create a transcript in this program using the label feature. You can
transcribe the audio from a video MOV or MP4 file but, you will not be
able to watch the visual. Audacity is a free program and can be
downloaded from
[*http://www.audacityteam.org/download/*](http://www.audacityteam.org/download/).

#### The Audacity Window

![](media/image01.png)

1.  The playback controls to start and stop the audio.

2.  The magnifying glass to zoom in and out of the track. This is a very important feature because you will need to zoom in to fit in all of your labels.

3.  The playback speed control slider. Use this tool to speed up or slow down audio during transcription.

4.  The Track Pane. This pane will show you where you are in the interview. Click and drag to highlight a portion of audio you want to listen to or label.

5.  The Label Pane. This is where you enter the transcription text as labels.

#### Steps for Generating the Transcript In Audacity

1.  Open Audacity on your computer.

2.  Open the file you are transcribing by going to File&gt; Open and select the file from your directory. The track will open in the program.

3.  In Audacity you can select a section of the track to annotate. First zoom in using the magnifying glass (This is an important step as it will allow your labels to fit on the track). Then select part of the tract by clicking the start of where you want to begin your annotation and drag the cursor to the end (see example below).

![](media/image27.png)

4.  Listen to the selected audio by using the playback controls. If necessary you can slow down the audio using the slider.

5.  Add the label by going to Tracts&gt; Add Label at Selection (or use the hotkey ctrl + b (Windows)/ cmd + b (mac)). Note that the program automatically generates the time stamps.

6.  Input the text into the label. Remember to record the speaker's name using the proper format of Speaker's Name: What they are saying. Please see below for an example.

![](media/image05.png)

7.  When you have finished transcribing the audio you can read the transcript as a whole by going to Tracts&gt; Edit Labels… Here you can correct text. Note you will not be able to listed to the audio while this window is open. It is good practice to review the labels while listening to the audio once the transcript is complete to ensure it is error free.

8.  When you are finished Make sure you save your work by going to File &gt; Save Project As…

9.  Export the transcript by going to File&gt; Export Labels. Save the file with same name as the original audio/video, but with the extension .txt.


### Creating Transcripts with oTranscribe (BETA)

**Sample text-based output**

0:01 old and in the way

0:24 they'll never care about you

0:31 once I hear tell he was happy

0:38 good time

0:44 looking back to a better day

**Sample oTranscribe (json) format**

```json
{  
   "text":" <p data-l10n-id=\"\"><span class=\"timestamp\" data-timestamp=\"0:01\" contenteditable=\"false\">0:01</span> old and in the way<br></p><p data-l10n-id=\"\"><span class=\"timestamp\" data-timestamp=\"0:24\" contenteditable=\"false\">0:24</span> they'll never care about you <br></p><p data-l10n-id=\"\"><span class=\"timestamp\" data-timestamp=\"0:31\" contenteditable=\"false\">0:31</span> once I hear tell he was happy<br></p><p data-l10n-id=\"\"><span class=\"timestamp\" data-timestamp=\"0:38\" contenteditable=\"false\">0:38</span> good time<br></p><p data-l10n-id=\"\"><span class=\"timestamp\" data-timestamp=\"0:44\" contenteditable=\"false\">0:44</span> looking back to a better day<br></p>\n <p><br></p><p>\n </p>",
   "media-source":""
}
```

**Supports**

Audio (web version)

Video (desktop version)

Youtube (web and desktop version)


## **Transcription Best Practices**

#### Recording Actions

-   Use your best judgment when you are transcribing videos. If you think the content is worth capturing, then capture it in the transcript.

-   If you want to indicate pauses, actions, or side discussions that are happening in the audio or video, create them in a new time block. **For pauses, use "No speaker: (Pause)". For other situations use "No speaker: (Aside)".** Pauses and actions should not be captured unless it is relevant or very, very long (use your judgement here). For now, actions do not need to be captured in time blocks with speech or transcription text.

#### Dealing with Rapid-fire Changes in Speakers

-   Sometimes, when there is more than one speaker in the media you will encounter a rapid change between speakers or even overlap between speakers. You should continue with the convention of marking each speaker in sequence, even if there is overlap. So, a transcript with two speakers talking at the same time would look like this (note that the time codes are the same, the time codes need to be slightly off

#### Transcription Formatting

-   Start a new line when the speaker naturally takes a pause or takes a break, approximately every 1-5 sentences. Shorter sentences will look better on CC display for video as well and will be easier for the viewer to read

-   Note: **It is very important to create transcripts in a well-structured data format for future processing.**

-   Don't use:

    -   colons (":") in your transcript, other than when you're using it
        > to indicate the Speaker or that it's a Translation.

    -   ellipses ("...") they don't work in the transcripts

        -   For a time block of transcription, never start a new line or paragraph by pressing Enter. Just keep writing on the same line and let the text overflow. Separate new sentences using spaces only.

-   Do this:
    > [00:00:00.00] Daku Sherpa: The transcription text goes here on
    > the same line. Never start a new line or a new paragraph. Always
    > have a closing time stamp.[00:06:32.12]

-   Not this:
    > [00:00:00.00\] Daku Sherpa: No! Don't do this! The transcription
    > text goes here on the same line.
    > Never start a new line or a new paragraph.
    > Always have a closing time stamp.[00:06:32.12]

-   Time blocks must start on new lines. One time block per line. Don't have spaces between lines. In short: Everything should look neat and tidy.

-   **Please use UTF-8 encoding compatible characters only in your transcripts**

#### Recording Speakers

-   For an interviewer with unknown name, use "Interviewer:". For multiple interviewers, use "Interviewer1:", "Interviewer2:", and so on.

-   For unknown speaker(s) with no name, use "Unknown1:", "Unknown2:" and so on.

-   **IF ONLY ONE PERSON SPEAKS FOR THE ENTIRE INTERVIEW:** If there is only one speaker in the entire interviewer, you do not have to add the [speaker] stamp. Just indicate the speaker at the beginning:

> Sole Speaker: Phuchung
> [00:00:00.00] I'm not that kind of person, no.[00:06:32.12]
> [00:06:32.12] Yes it's very easy to talk about that. [00:09:10.45]

-   so each speaker needs to start about one second (or if not possible, do by a few milliseconds) from each other, and the text and speaker label is different:

-   [00:01:11.010] Speaker 1: འདི་བོག་ཡིག་རེད་བས། [00:01:13.020]

-   [00:01:12.010] Speaker 2: རེད་། [00:01:13.020]

 {#section-4}

#### Adding additional tiers to your transcript

Tiers are additional layers of information that can be added to your
transcript that aren’t transcribed information. This can include
translations, transliterations, annotations, and so on. Adding tiered
information can happen at the time of transcription or afterwards once
your transcript has been exported into a delimited text format.

During transcription:

After transcription:

#### Other rules

-   Some of the videos have people speaking in three languages. One in Nepali, then with someone translating the question/response in Hindi or vice versa. Transcribe the original questions from the interviewer and the responses from the interviewee in the interviewee's language.
-   Ignore side discussions and other external conversations unless you think the material is relevant to the interview.
-   Some audio files will have a lot more interview material than what is captured in the video. This leftover audio will need to be transcribed at a later point in time but for now focus on transcribing the videos.
-   In the case of multiple languages, if it’s possible to capture a language in romanized script, please do so


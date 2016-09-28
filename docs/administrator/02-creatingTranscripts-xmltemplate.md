### Converting tab delimited/text files created in Transcription Software into XML

The files created in transcription software must be converted into XML
to make it possible for ingest into Islandora. The resulting file will
be named TRANSCRIPT.xml.

**Parts of this Section:**

-   [***Simple XML Transcript Template***](#simple-transcript-xml-template)

-   [***Converting a Tab Delimited File Created in Inqscribe into XML***](#converting-a-tab-delimited-file-created-in-inqscibe-into-xml)

    -   [***Tools***](#tools)

    -   [***Converting the timestamps to seconds in Excel***](#converting-the-timestamps-to-seconds-in-excel)

    -   [***Steps to create XML from Inqscribe Tab Delimited File Using Open Refine***](#steps-to-create-xml-from-inqscribe-tab-delimited-file-using-open-refine)

-   **Optional: Validating transcripts using Oxygen XML Editor**

-   [***Converting a Tab Delimited File created in Audacity into XML***](#converting-a-tab-delimited-file-created-in-audacity-into-xml)

#### Simple Transcript XML Template

**The elements:**

-   cues

    -   cue

        -   speaker or solespeaker

        -   Start

        -   end

**Tiers (optional):**

-   Transcript

-   Translation

-   Annotation

Tiers are the additional pieces of information that you want to
associate with a start and end time and speaker in a transcript. Your
oral history might have transcriptions only, so you would then add the
&lt;transcript&gt; element into your TRANSCRIPT.xml. They might have
transcriptions and translations in which case you would add both the
&lt;transcript&gt; and &lt;translation&gt; element into your
TRANSCRIPT.xml. If a tier exists in the transcript, they always need to
appear in every cue and they always need to be in the same order.

If an oral history only has one speaker for the entire clip use only
solespeaker. If there are multiple speakers, use the speaker element.
Solespeaker and speaker should never appear together in the same
transcript.

**Example**
```xml
<?xml version="1.0" encoding="UTF-8"?>
<cues>
	<!-- If the entire transcript has one speaker only, use 'solespeaker' element. Then skip 'speaker' element in 'cue' element level. But DO NOT use them in both places. If 'solespeaker' element presents in the document, following 'speaker' elements will be ignored. -->
	<solespeaker>One Speaker</solespeaker>
	<cue>
		<speaker>Different Speaker</speaker>
		<!-- 'start' and 'end' elements are start time and end time in seconds for the cue. -->
		<start>0.000</start>
		<end>12.124</end>
		<!-- 'transcript' and/or 'annotation' are default content tiers of the cue. Extra tier(s) can be added as long as they are listed in the configuration page. 'transcript' element is required if 'Enable captions/subtitles display' is configured to be true. -->
		<transcript>This is the transcript text content.</transcript>
		<annotation>This is the annotation content.</annotation>
	</cue>
	<!-- add more cues with above structure.-->
</cues>
```

### Converting a Tab Delimited File created in Inqscibe into XML

#### Tools

There are two tools needed for this conversion

Excel

OpenRefine [*http://openrefine.org/*](http://openrefine.org/)

[*http://digitalscholarship.utsc.utoronto.ca/projects/content/blogs/converting-spreadsheets-modsxml-using-open-refine*](http://digitalscholarship.utsc.utoronto.ca/projects/content/blogs/converting-spreadsheets-modsxml-using-open-refine)

**Important:** Use UTF-8 Unicode as your text encoding format! You can
make sure you do this when you import the data into Excel.

#### Converting the timestamps to seconds in Excel

**Only use these steps for Inqscribe Transcripts**

The islandora oral histories solution pack reads time as seconds.
Instead of 00:00:23.10 it would need 23.1. To change the timestamp
follow these steps:

1.  Open Excel then go to File&gt; Open and select the transcript file you are working with and click open.

2.  The Text Import Wizard will open. Click the next button until you get to step 3 of 3 then click Finish.

3.  Remove all unicode encoded characters you see within the spreadsheet ex. Ôªø.

4.  Rename the IN column START and the OUT column END.

5.  Insert two new columns one following the IN column and one following the OUT column. Call the two new columns IN and OUT. This step can be seen below.

![](media/image23.png)

6.  You will now convert the time in the START and END columns into seconds. To do this insert the formula =A2\*86400 into the new IN Column (B2) and the formula =C2\*86400 into the new OUT column (D2). See Figure (x) for example.

![](media/image18.png)

6.  At this stage you will copy down the formula you just inserted. To do this click the cell where you just entered the formula. Then bring your cursor to the bottom right corner of that cell until it turns into a solid black cross and then double click. The format of the IN and OUT columns should match figure (x) seen below.

![](media/image09.png)

7.  Save the file as a tab delimited file. Go to File&gt; Save As... Ensure that tab delimited file is selected in the format box and click save.

#### Steps to create XML from Inqscribe Tab Delimited File Using Open Refine

**(Only use these steps for Inqscribe transcripts)**

1.  Open OpenRefine

2.  Click the Browse button and select the file you are working with and click Next&gt;Create Project. Make sure the character encoding is UTF-8 in before creating the project.

3.  **Remove** the START column and the END column with the time format 00:00.0. To do this click the down arrow in the header of the column title then click Edit column&gt;Remove this column. See example below.

![](media/image20.png)

4.  To begin the process of exporting the table to XML click Export&gt; Templating. The Templating Export box will appear.

5.  Copy and paste the text below into your the Templating Export box. Ensure that you match the appropropriate text to the input box using the bold headers as guides.

**prefix**

prefix

```xml
<?xml version="1.0" encoding="UTF-8"?>
<cues>
```

**row template (IMPORTANT: make sure your headers in your transcript
file match the headers in the row template!)**

```xml
<cue>
<speaker>{{cells["SPEAKER"].value}}</speaker>
<start>{{cells[“START"].value}}</start>
<end>{{cells[“END"].value}}</end>
<transcript>{{cells["TRANSCRIPT"].value}}</transcript>
</cue>
```

**Row template (Use this version If your transcript has tiers only)**

```xml
<cue>
<speaker>{{cells["INTERVIEWER"].value}}</speaker>
<start>{{cells["IN"].value}}</start>
<end>{{cells["OUT"].value}}</end>
<transcript>{{cells["TRANSCRIPT"].value}}</transcript>
<translation>{{cells["TRANSLATION"].value}}</translation>
<annotation>{{cells["ANNOTATION"].value}}</annotation>
</cue>
```

**row separator**

Make this box empty, remove any characters

**suffix**

```xml
</cues>
```

**Final Result**

The resulting file should match this format:

```xml
<cue>
<speaker>Dorjee Lhatoo</speaker>
<start>36.0</start>
<end>45.0</end>
<transcript> Poor company with me. She would pour me a glass, one sip and it goes right through, you see -</transcript>
</cue>

<cue>
<speaker>Interviewer2</speaker>
<start>46.0</start>
<end>49.0</end>
<transcript> [indiscernible] on fire. She sounds like great fun.</transcript>
</cue>

<cue>
<speaker>Dorjee Lhatoo</speaker>
<start>49.0</start>
<end>52.0</end>
<transcript> And then she used to bidi</transcript>
</cue>

<cue>
<speaker>Interviewer2</speaker>
<start>52.0</start>
<end>53.0</end>
<transcript> Oh!</transcript>
</cue>
```

### Converting a Tab Delimited File created in Audacity into XML

**Follow these steps if you created a transcript using Audacity**

1.  Open OpenRefine

2.  Click the Browse button and select the file you are working with and click Next. Find the box labeled “line(s) as column headers” located at the bottom right of the screen and change 1 to 0. Click the Create Project button located at the top right of the screen. See example below.

![](media/image21.png)

1.  We now need to create a new column for speaker name. The speaker name information will be contained in column 3. To create the new column click the down arrow next to the column 3 header then click Edit column&gt; Split into several columns…. A box titled split Column 3 into several columns. Change the Separator to : and click OK. An example can be seen below.

![](media/image17.png)

1.  You now need to change the column headers. To do this click the down arrow beside the column headers then click Edit column&gt; Rename this column. Enter the new name and click OK. Please see example below. You new column headers should be:

    -   IN (Replacing Column 1)

    -   OUT (Replacing Column 2)

    -   SPEAKER (Replacing Column 3 1)

    -   TRANSCRIPT (Replacing Column 3 2)

![](media/image26.png)

4.  To begin the process of exporting the table to XML click Export&gt; Templating. The Templating Export box will appear.

5.  Copy and paste the text below into your the Templating Export box. Ensure that you match the appropropriate text to the input box using the bold headers. Then click the Export button. Give file the appropriate name and place in the appropriate directory.

**prefix**

```xml
<?xml version="1.0" encoding="UTF-8"?>
<cues>
```

**row template **

```xml
<cue>
<speaker>{{cells["SPEAKER"].value}}</speaker>
<start>{{cells[“START"].value}}</start>
<end>{{cells[“END"].value}}</end>
<transcript>{{cells["TRANSCRIPT"].value}}</transcript>
</cue>
```

**Row Template (Use only If your have transcript has tiers, change this
template to match what tiers you are using.)**

```xml
<cue>
<speaker>{{cells["INTERVIEWER"].value}}</speaker>
<start>{{cells["IN"].value}}</start>
<end>{{cells["OUT"].value}}</end>
<transcript>{{cells["TRANSCRIPT"].value}}</transcript>
<translation>{{cells["TRANSLATION"].value}}</translation>
<annotation>{{cells["ANNOTATION"].value}}</annotation>
</cue>
```

**row separator**

none

**suffix**

```xml
</cues>
```

8.  The resulting XML file should have the structure seen below

**Final Result**

```xml
<cue>
<speaker>Dorjee Lhatoo</speaker>
<start>36.0</start>
<end>45.0</end>
<transcript> Poor company with me. She would pour me a glass, one sip and it goes right through, you see -</transcript>
</cue>

<cue>
<speaker>Interviewer2</speaker>
<start>46.0</start>
<end>49.0</end>
<transcript> [indiscernible] on fire. She sounds like great fun.</transcript>
</cue>

<cue>
<speaker>Dorjee Lhatoo</speaker>
<start>49.0</start>
<end>52.0</end>
<transcript> And then she used to bidi</transcript>
</cue>

<cue>
<speaker>Interviewer2</speaker>
<start>52.0</start>
<end>53.0</end>
<transcript> Oh!</transcript>
</cue>
```

What you need for the Ingest Final Package
==========================================

### In this Section:

-   [*Create MODS metadata*](#create-mods-metadata)

-   [*Ingest into Fedora*](#_r8p0qywqgjwh)

### Create MODS metadata

Create a table containing the metadata using google sheets

Follow the steps below to create MODS metadata pertaining to the video
file to be ingested into Islandora:

1.  Go to the Google Sheet:
    > [*https://docs.google.com/spreadsheets/d/16AyEKRdqaWWj9Yeo9prmvZiKNonDEJg9FI2hLF26ukE/edit?usp=sharing*](https://docs.google.com/spreadsheets/d/16AyEKRdqaWWj9Yeo9prmvZiKNonDEJg9FI2hLF26ukE/edit?usp=sharing).

2.  Make sure to create a copy of the spreadsheet to work on for each
    > metadata record created. To create a copy go to file&gt;create
    > a copy.

3.  Enter the appropriate metadata into the copy starting at line 2
    > overwriting the cells containing descriptions. Delete all metadata
    > in the following rows that do not pertain to your video or
    > audio recording.

4.  Save your file as a tab delimited format by going to File&gt;
    > Download as &gt; Tab-separated Variables (TSV)

### Convert Metadata table in MODS XML

1.  Open OpenRefine

2.  Click browse and select the metadata TSV file you are working with.
    > Click next&gt; Create Project.

3.  To begin the process of exporting the table to XML click
    > Export&gt; Templating. The Templating Export box will appear.

4.  Copy and paste the text below into your the Templating Export box.
    > Ensure that you match the appropropriate text to the input box
    > using the bold headers. Then click the Export button. Give file
    > the appropriate name and place in the appropriate directory.

**Prefix**

```xml
<mods xmlns="http://www.loc.gov/mods/v3" 
xmlns:mods="http://www.loc.gov/mods/v3"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:xlink="http://www.w3.org/1999/xlink">
```

**Row Template (IMPORTANT: replace sound\_recording with videorecording
if your oral history is a video)**

```xml
<titleInfo>
	<title>{{cells["Title"].value}}</title>
	<subTitle>{{cells["Subtitle"].value}}</subTitle>
</titleInfo>
<name type="personal">
	<namePart>{{cells["Name"].value}}</namePart>
	<role>
		<roleTerm authority="marcrelator" type="text">{{cells["Role of Name"].value}}</roleTerm>
	</role>
</name>
<name type="personal">
	<namePart>{{cells["Name2"].value}}</namePart>
	<role>
		<roleTerm authority="marcrelator" type="text">{{cells["Role of Name 2"].value}}</roleTerm>
	</role>
</name>
<typeOfResource></typeOfResource>
<genre/>
<originInfo>
	<dateIssued>{{cells["Date"].value}}</dateIssued>
	<publisher/>
	<place>
		<placeTerm authority="marccountry"/>
	</place>
	<place>
		<placeTerm type="text"/>
	</place>
</originInfo>
<language>
	<languageTerm authority="iso639-2b" type="code">{{cells["Language1"].value}}</languageTerm>
</language>
<language>
	<languageTerm authority="iso639-2b" type="code">{{cells["Language2"].value}}</languageTerm>
</language>
<language>
	<languageTerm authority="iso639-2b" type="code">{{cells["Language3"].value}}</languageTerm>
</language>
<abstract>{{cells["Description"].value}}</abstract>
<identifier/>
<physicalDescription>
	<form authority="marccategory">sound_recording</form>
	<extent/>
</physicalDescription>
<note/>
<subject>
	<topic>{{cells["Topic1"].value}}</topic>
	<topic>{{cells["Topic2"].value}}</topic>
	<topic>{{cells["Topic3"].value}}</topic>
	<topic>{{cells["Topic4"].value}}</topic>
	<geographic/>
	<temporal/>
	<hierarchicalGeographic>
		<continent>{{cells["Continent"].value}}</continent>
		<country>{{cells["Country"].value}}</country>
		<province>{{cells["Province"].value}}</province>
		<region/>
		<county/>
		<city>{{cells["City"].value}}</city>
		<citySection>{{cells["City Section"].value}}</citySection>
	</hierarchicalGeographic>
	<cartographics>
		<coordinates>{{cells["Coordinates"].value}}</coordinates>
	</cartographics>
</subject>undefined</mods>
```

**Row Separator**

none

**For Suffix**

```xml
</mods>
```

**Final Result**

```xml
<mods
	xmlns="http://www.loc.gov/mods/v3"
	xmlns:mods="http://www.loc.gov/mods/v3"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:xlink="http://www.w3.org/1999/xlink">
	<titleInfo>
		<title>Title of Oral History</title>
		<subTitle/>
	</titleInfo>
	<name type="personal">
		<namePart>Kim Day</namePart>
		<role>
			<roleTerm authority="marcrelator" type="text">Speaker</roleTerm>
		</role>
	</name>
	<typeOfResource>moving image</typeOfResource>
	<genre/>
	<originInfo>
		<dateIssued>2001-05-21</dateIssued>
		<publisher/>
		<place>
			<placeTerm authority="marccountry"/>
		</place>
		<place>
			<placeTerm type="text"/>
		</place>
	</originInfo>
	<language>
		<languageTerm authority="iso639-2b" type="code">fre</languageTerm>
	</language>
	<abstract>A description of the video goes here</abstract>
	<identifier/>
	<physicalDescription>
		<form authority="marccategory">sound_recording</form>
		<extent/>
	</physicalDescription>
	<note></note>
	<subject>
		<topic>cold</topic>
		<geographic/>
		<temporal/>
		<hierarchicalGeographic>
			<continent>North America</continent>
			<country>Canada</country>
			<province>Ontario</province>
			<region/>
			<county/>
			<city>Toronto</city>
			<citySection>Scarborough</citySection>
		</hierarchicalGeographic>
		<cartographics>
			<coordinates>43.787098, -79.185808</coordinates>
		</cartographics>
		<topic>winter</topic>
		<topic>city life</topic>
	</subject>
	<language/>
	<name type="personal">
		<namePart>Merle Haggard</namePart>
		<role>
			<roleTerm authority="marcrelator" type="text">Interviewer</roleTerm>
		</role>
	</name>
	<language>
		<languageTerm authority="iso639-2b" type="code">eng</languageTerm>
	</language>
</mods>
```

Ingest into Fedora
------------------

[*https://github.com/digitalutsc/fedora\_video\_ingesting*](https://github.com/digitalutsc/fedora_video_ingesting)

Ingest into Islandora through the interface - recommended file formats
and sizes

  **Document type**                               **Content types**   **Islandora accepted file formats**   **Size restrictions \***
  ----------------------------------------------- ------------------- ------------------------------------- --------------------------
  Oral history, digital storytelling, interview   audio recording     wav, mp3                              less than 200 mb
                                                  video recording     ogg, mp4, mov, qt, m4v, avi, mkv      less than 200 mb

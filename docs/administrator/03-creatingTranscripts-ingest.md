What you need for the Ingest Final Package
==========================================

### In this Section:

-   [*Create MODS metadata*](#create-mods-metadata)

-   [*Ingest into Fedora*](#_r8p0qywqgjwh)

###  {#section-6}

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

&lt;mods

xmlns="http://www.loc.gov/mods/v3"

xmlns:mods="http://www.loc.gov/mods/v3"

xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

xmlns:xlink="http://www.w3.org/1999/xlink"&gt;

**Row Template (IMPORTANT: replace sound\_recording with videorecording
if your oral history is a video)**

&lt;titleInfo&gt;

&lt;title&gt;{{cells\["Title"\].value}}&lt;/title&gt;

&lt;subTitle&gt;{{cells\["Subtitle"\].value}}&lt;/subTitle&gt;

&lt;/titleInfo&gt;

&lt;name type="personal"&gt;

&lt;namePart&gt;{{cells\["Name"\].value}}&lt;/namePart&gt;

&lt;role&gt;

&lt;roleTerm authority="marcrelator" type="text"&gt;{{cells\["Role of
Name"\].value}}&lt;/roleTerm&gt;

&lt;/role&gt;

&lt;/name&gt;

&lt;name type="personal"&gt;

&lt;namePart&gt;{{cells\["Name2"\].value}}&lt;/namePart&gt;

&lt;role&gt;

&lt;roleTerm authority="marcrelator" type="text"&gt;{{cells\["Role of
Name 2"\].value}}&lt;/roleTerm&gt;

&lt;/role&gt;

&lt;/name&gt;

&lt;typeOfResource&gt;&lt;/typeOfResource&gt;

&lt;genre/&gt;

&lt;originInfo&gt;

&lt;dateIssued&gt;{{cells\["Date"\].value}}&lt;/dateIssued&gt;

&lt;publisher/&gt;

&lt;place&gt;

&lt;placeTerm authority="marccountry"/&gt;

&lt;/place&gt;

&lt;place&gt;

&lt;placeTerm type="text"/&gt;

&lt;/place&gt;

&lt;/originInfo&gt;

&lt;language&gt;

&lt;languageTerm authority="iso639-2b"
type="code"&gt;{{cells\["Language1"\].value}}&lt;/languageTerm&gt;

&lt;/language&gt;

&lt;language&gt;

&lt;languageTerm authority="iso639-2b"
type="code"&gt;{{cells\["Language2"\].value}}&lt;/languageTerm&gt;

&lt;/language&gt;

&lt;language&gt;

&lt;languageTerm authority="iso639-2b"
type="code"&gt;{{cells\["Language3"\].value}}&lt;/languageTerm&gt;

&lt;/language&gt;

&lt;abstract&gt;{{cells\["Description"\].value}}&lt;/abstract&gt;

&lt;identifier/&gt;

&lt;physicalDescription&gt;

&lt;form authority="marccategory"&gt;**sound\_recording**&lt;/form&gt;

&lt;extent/&gt;

&lt;/physicalDescription&gt;

&lt;note/&gt;

&lt;subject&gt;

&lt;topic&gt;{{cells\["Topic1"\].value}}&lt;/topic&gt;

&lt;topic&gt;{{cells\["Topic2"\].value}}&lt;/topic&gt;

&lt;topic&gt;{{cells\["Topic3"\].value}}&lt;/topic&gt;

&lt;topic&gt;{{cells\["Topic4"\].value}}&lt;/topic&gt;

&lt;geographic/&gt;

&lt;temporal/&gt;

&lt;hierarchicalGeographic&gt;

&lt;continent&gt;{{cells\["Continent"\].value}}&lt;/continent&gt;

&lt;country&gt;{{cells\["Country"\].value}}&lt;/country&gt;

&lt;province&gt;{{cells\["Province"\].value}}&lt;/province&gt;

&lt;region/&gt;

&lt;county/&gt;

&lt;city&gt;{{cells\["City"\].value}}&lt;/city&gt;

&lt;citySection&gt;{{cells\["City Section"\].value}}&lt;/citySection&gt;

&lt;/hierarchicalGeographic&gt;

&lt;cartographics&gt;

&lt;coordinates&gt;{{cells\["Coordinates"\].value}}&lt;/coordinates&gt;

&lt;/cartographics&gt;

&lt;/subject&gt;

&lt;/mods&gt;

**Row Separator**

none

**For Suffix**

&lt;/mods&gt;

**Final Result**

&lt;mods

xmlns="http://www.loc.gov/mods/v3"

xmlns:mods="http://www.loc.gov/mods/v3"

xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

xmlns:xlink="http://www.w3.org/1999/xlink"&gt;

&lt;titleInfo&gt;

&lt;title&gt;Title of Oral History&lt;/title&gt;

&lt;subTitle/&gt;

&lt;/titleInfo&gt;

&lt;name type="personal"&gt;

&lt;namePart&gt;Kim Day&lt;/namePart&gt;

&lt;role&gt;

&lt;roleTerm authority="marcrelator"
type="text"&gt;Speaker&lt;/roleTerm&gt;

&lt;/role&gt;

&lt;/name&gt;

&lt;typeOfResource&gt;moving image&lt;/typeOfResource&gt;

&lt;genre/&gt;

&lt;originInfo&gt;

&lt;dateIssued&gt;2001-05-21&lt;/dateIssued&gt;

&lt;publisher/&gt;

&lt;place&gt;

&lt;placeTerm authority="marccountry"/&gt;

&lt;/place&gt;

&lt;place&gt;

&lt;placeTerm type="text"/&gt;

&lt;/place&gt;

&lt;/originInfo&gt;

&lt;language&gt;

&lt;languageTerm authority="iso639-2b"
type="code"&gt;fre&lt;/languageTerm&gt;

&lt;/language&gt;

&lt;abstract&gt;A description of the video goes here&lt;/abstract&gt;

&lt;identifier/&gt;

&lt;physicalDescription&gt;

&lt;form authority="marccategory"&gt;sound\_recording&lt;/form&gt;

&lt;extent/&gt;

&lt;/physicalDescription&gt;

&lt;note&gt;&lt;/note&gt;

&lt;subject&gt;

&lt;topic&gt;cold&lt;/topic&gt;

&lt;geographic/&gt;

&lt;temporal/&gt;

&lt;hierarchicalGeographic&gt;

&lt;continent&gt;North America&lt;/continent&gt;

&lt;country&gt;Canada&lt;/country&gt;

&lt;province&gt;Ontario&lt;/province&gt;

&lt;region/&gt;

&lt;county/&gt;

&lt;city&gt;Toronto&lt;/city&gt;

&lt;citySection&gt;Scarborough&lt;/citySection&gt;

&lt;/hierarchicalGeographic&gt;

&lt;cartographics&gt;

&lt;coordinates&gt;43.787098, -79.185808&lt;/coordinates&gt;

&lt;/cartographics&gt;

&lt;topic&gt;winter&lt;/topic&gt;

&lt;topic&gt;city life&lt;/topic&gt;

&lt;/subject&gt;

&lt;language/&gt;

&lt;name type="personal"&gt;

&lt;namePart&gt;Merle Haggard&lt;/namePart&gt;

&lt;role&gt;

&lt;roleTerm authority="marcrelator"
type="text"&gt;Interviewer&lt;/roleTerm&gt;

&lt;/role&gt;

&lt;/name&gt;

&lt;language&gt;

&lt;languageTerm authority="iso639-2b"
type="code"&gt;eng&lt;/languageTerm&gt;

&lt;/language&gt;

&lt;/mods&gt;

Ingest into Fedora
------------------

[*https://github.com/digitalutsc/fedora\_video\_ingesting*](https://github.com/digitalutsc/fedora_video_ingesting)

Ingest into Islandora through the interface - recommended file formats
and sizes

  **Document type**                               **Content types**   **Islandora accepted file formats**   **Size restrictions \***
  ----------------------------------------------- ------------------- ------------------------------------- --------------------------
  Oral history, digital storytelling, interview   audio recording     wav, mp3                              less than 200 mb
                                                  video recording     ogg, mp4, mov, qt, m4v, avi, mkv      less than 200 mb

 {#section-7}

Installing on Vagrant (for testing)\
====================================

-   Install or start up Islandora
    > Vagrant (https://github.com/Islandora-Labs/islandora\_vagrant)

-   Install Transcript UI

    -   git clone
        > [*https://github.com/sprklinginfo/transcripts\_ui.git*](https://github.com/sprklinginfo/transcripts_ui.git)

    -   drush en transcripts\_ui -y

-   Install Oral Histories Module

    -   git clone
        > [*https://github.com/digitalutsc/islandora\_solution\_pack\_oralhistories.git*](https://github.com/digitalutsc/islandora_solution_pack_oralhistories.git)

    -   Go to Drupal modules page and enable the oral histories module

-   Go to Transcript UI and configure default and any additional
    > transcript tiers and speakers needed.

    -   Tiers

        -   or\_transcript|Transcript

        -   or\_translation|Translation

    -   Speaker names

        -   or\_speaker|Speaker

        -   or\_solespeaker|Speaker

-   Configure Solr (copy xslt, add field, restart tomcat)

    -   cd
        > /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora\_transforms

    -   cp
        > /var/www/drupal/sites/all/modules/islandora\_solution\_pack\_oralhistories/xsl/or\_transcript\_solr.xslt
        > ./

    -   cd ..

    -   Edit foxmlToSolr.xslt, and add the following line

        -   &lt;xsl:include
            > href="/var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora\_transforms/or\_transcript\_solr.xslt"/&gt;

    -   Edit /usr/local/solr/collection1/conf/schema.xml and add the
        > following dynamicfield:

        -   &lt;dynamicField name="or\_\*" type="text" indexed="true"
            > stored="true" multiValued="true"/&gt;

    -   Restart the tomcat

-   Configure Module Options:

    -   Go to admin/islandora/solution\_pack\_config/oralhistories
        > (in Drupal)

-   Manual Testing - Uploading an oral history object/media and a
    > transcript

    -   Get test fixtures from here:
        > https://github.com/digitalutsc/islandora\_solution\_pack\_oralhistories/tree/master/tests/fixtures

    -   Go to
        > islandora/object/islandora%253Aoralhistories\_collection/manage
        > (in Drupal)

    -   Click Add an object to this collection and continue with the
        > workflow to ingest the media and transcript files

-   Automated Testing - Running ingest/purge tests for Oral History

    -   Enable Testing module

    -   Go to admin/config/development/testing

    -   Ensure drupal filter at
        > /usr/local/fedora/server/config/filter-drupal.xml is writable
        > by the server.

    -   Scroll down to oral history tests, check them and run the tests

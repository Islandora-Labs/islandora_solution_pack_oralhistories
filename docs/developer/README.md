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


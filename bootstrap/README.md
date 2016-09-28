Oralhistories module uses bootstrap markup heavily.  If drupal uses non bootstrap base theme, then transcript UI may not look as expected.

A SASS built bootstrap theme targetting the div.islandora-oralhistories-content elements resolves this issue.  You can use the bootstrap provided with the module or you can build your own by following this reference: http://stackoverflow.com/questions/11831346/applying-css-styles-only-to-certain-elements

To include the custom build bootstrap, add the following lines in islandora_oralhistories.info.

* stylesheets[all][] = bootstrap/css/oh-bootstrap-all.css
* scripts[] = bootstrap/js/bootstrap.min.js

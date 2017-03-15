  <?php
  /**
   * The testSolrResponse.php script is a helper script to help determine if
   * the Solr is communicating properly with the Oral Histories Solution Pack.
   *
   * Place this entire script within an Execute PHP code block in your Islandora
   * instance.  For a production site, this block should be configured to be
   * limited to admin users and to specific page.  Typically once does not
   * include PHP tags when added PHP code into Execute PHP code blocks.
   *
   * To configure the script, update the $pid, $tiers, $speaker and $url
   * variables as appropriate to your installation.
   *
   * Expected return values:
   *
   * Success: var_dump of the Solr JSON response that should include details
   * of the transcript.
   *
   * Failure:  Error logged or var_dump of empty JSON response.
   *
   * Next steps:
   *  - Double check that the values of $pid, $tiers, $speaker, and $url are
   *  valid.
   *  - Check Solr configuration.  Check islandora_solution_pack_oralhistories
   *  issues or wiki for trouble shooting tips.
   */
  // Replace 'object:pid' with the PID of an oral histories object
  $pid = str_replace('-', ':', 'object:pid');
  // Replace 'or_transcript' with one of the tiers from your installation.
  $tiers = array("or_transcript");
  // Replace 'or_solespeaker' with one of the speakers from your installation.
  $speaker = array("or_solespeaker");
  // Replace 'localhost:8080/solr' with the path to your Solr instance.
  $url = parse_url('localhost:8080/solr');

  // Now we query solr fields to buid $tcus
  $qualifier = 'PID:' . '"' . $pid . '"';

  // solr fields to be returned.
  $fields = array('PID', 'or_cue_id', 'or_start', 'or_end');

  $fields = array_merge($fields, $tiers);
  $fields = array_merge($fields, $speaker);

  // Build solr query.
  $query = "$qualifier";
  $query .= "&sort=or_start asc";

  $params = array(
      'fl' => $fields,
      'qt' => 'standard',
    );

  $solr = new Apache_Solr_Service($url['host'], $url['port'], $url['path'] . '/');
  $solr->setCreateDocuments(FALSE);
  try {
    $results = $solr->search($query, 0, 1000, $params);
    $json = json_decode($results->getRawResponse(), TRUE);
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Oralhistories', $e, 'Got an exception while searching transcripts for callback.', array(), WATCHDOG_ERROR);
  }

  var_dump($json['response']['docs']);

  ?>

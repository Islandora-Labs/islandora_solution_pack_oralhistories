  <?php

  $pid = str_replace('-', ':', 'object:pid');
  $tiers = array("or_transcript");
  $speaker = array("or_solespeaker");
  $url = parse_url('localhost:8080/solr');

 // Now we query solr fields to buid $tcus
  $qualifier = 'PID:' . '"' . $pid . '"';

  
  // solr fields to be returned.
  $fields = array('PID', 'or_cue_id', 'or_start', 'or_end');

  $fields = array_merge($fields, $tiers);
  $fields = array_merge($fields, $speaker);

  // Build solr query.
  //$query = "$qualifier AND " . 'RELS_EXT_hasModel_uri_mt:"islandora:oralhistoriesCModel"';
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
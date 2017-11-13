<?php

/**
 * @file
 * Utility functions.
 */

require_once(__DIR__ . '/lib/VttConverter.php');

/**
 * Helper function to build transcript content.
 *
 * @param string $url
 *   String with file url.
 *
 * @return string
 *   All cues in vtt file.
 */
function vtt_transcript_content($url, $trid) {
  $options = array(
    'term' => isset($_GET['term']) ? '"' . $_GET['term'] . '"' : '',
    'type' => 'VTT',
    'url' => $url,
  );
  $ui = transcripts_ui_ui('islandora_oralhistories', $trid, $options);
  return theme('islandora_transcript_scroller', transcripts_ui_render($ui));
}

/**
 * Helper function to parse vtt file.
 */
function parse_vtt($data = '') {

  $oConverter = new VttConverter();
  $oVttData = $oConverter->fileContentToInternalFormat($data);

  $cues = array();
  foreach ($oVttData as $key => $vttData) {
    $start_time = $vttData['start'];
    $end_time = $vttData['end'];
    $name = $vttData['name'];
    $lines = $vttData['lines'];
    $lines= array_map('trim', $lines);
    $text = implode(" ", $lines);

    $cues[] = array(
      'start_time' => $start_time,
      'end_time' => $end_time,
      'text' => $text,
      'name' => $name,
    );
  }
  return $cues;
}


/**
 * Helper function to convert timestamp to seconds.
 */
function time_seconds($time = '') {
  $t = explode(':', $time);
  $sec = (float) ($t[count($t) - 1]) + (int) ($t[count($t) - 2]) * 60;
  if (count($t) > 2) {
    $sec += (int) ($t[count($t) - 3]) * 3600;
  }
  return $sec;
}

/**
 * Helper function to convert seconds to mm:ss.ttt.
 */
function time_mm_ss($seconds) {
  $t = round((float) $seconds, 3);
  return sprintf('%02d:%02d.%03d', ($t / 60), ($t % 60), (fmod($t, 1.0) * 1000));
}


/**
 * Helper function to prepare transcript content display.
 */
function xml_transcript_content($trid) {
  // How will options get through when function
  // is not called as part of page request?
  $options = array(
    'term' => isset($_GET['term']) ? '"' . $_GET['term'] . '"' : '',
    'type' => 'XML',
  );

  $ui = transcripts_ui_ui('islandora_oralhistories', $trid, $options);

  if ($ui->tcuCount > 0) {
    return theme('islandora_transcript_scroller', transcripts_ui_render($ui));
  }
  else {
    return t("This transcript is still being processed for display.");
  }
}

/**
 * Implements hook_transcripts_ui_transcript().
 */
function islandora_oralhistories_transcripts_ui_transcript($ui) {
  $tcus = array();
  $tiers = array_keys($ui->tiers);
  $speaker = array_keys($ui->speakernames);
  $options = $ui->options;
  $transcript_type = $options["type"];

  // If VTT process it using compose_vtt_transcript_as_transcripts_ui function.
  if ($transcript_type == "VTT") {
    return compose_vtt_transcript_as_transcripts_ui($ui);
  }

  // Now we query solr fields to buid $tcus.
  $pid = str_replace('-', ':', $ui->shorttrid);

  $qualifier = 'PID:"' . $pid . '"';
  // Solr fields to be returned.
  $fields = array('PID', 'or_cue_id', 'or_start', 'or_end');

  $fields = array_merge($fields, $tiers);
  $fields = array_merge($fields, $speaker);

  // Build solr query.
  // $query = "$qualifier AND " .
  // 'RELS_EXT_hasModel_uri_mt:"islandora:oralhistoriesCModel"';
  $query = "$qualifier";

  if ($options['term'] == '') {
    $params = array(
      'fl' => $fields,
      'qt' => 'standard',
    );
  }
  else {
    // highlighting.
    $arr = array();
    foreach ($tiers as $tier) {
      $arr[] = "$tier:" . $options['term'];
    }
    $query .= ' AND (' . implode(" OR ", $arr) . ')';
    if (isset($options['hits_only']) && !$options['hits_only']) {
      $query .= " OR *:*";
    }

    $params = array(
      'fl' => $fields,
      'qt' => 'standard',
      'hl' => 'true',
      'hl.fl' => implode(' ', $tiers),
      'hl.fragsize' => 0,
      'hl.simple.pre' => "<em>",
      'hl.simple.post' => "</em>",
    );
  }
  $url = parse_url(variable_get('islandora_solr_url', 'localhost:8080/solr'));
  $solr = new Apache_Solr_Service($url['host'], $url['port'], $url['path'] . '/');
  $solr->setCreateDocuments(FALSE);
  try {
    $results = $solr->search($query, 0, 1000, $params);
    $json = json_decode($results->getRawResponse(), TRUE);
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Oralhistories', $e, 'Got an exception while searching transcripts for callback.', array(), WATCHDOG_ERROR);
  }

  if (isset($json['response']['docs'][0])) {
    $doc = $json['response']['docs'][0] ? $json['response']['docs'][0] : NULL;
    $counter = isset($doc['or_cue_id']) ? count($doc['or_cue_id']) : 0;

    for ($i = 0; $i < $counter; $i++) {
      $tcus[$i] = new stdClass();
      $tcus[$i]->trid = str_replace(':', '-', $doc['PID']);
      $tcus[$i]->id = $doc['or_cue_id'][$i];
      $tcus[$i]->start = (float) $doc['or_start'][$i];
      $tcus[$i]->end = (float) $doc['or_end'][$i];

      // Muti tiers and speakernames are parsed.
      foreach ($tiers as $t) {
        if (isset($doc[$t])) {
          $tcus[$i]->$t = $doc[$t][$i];
        }
      }
      foreach ($speaker as $s) {
        if (isset($doc[$s])) {
          $tcus[$i]->$s = $doc[$s][$i];
        }
      }
    }
  } else {
    watchdog('Islandora Oralhistories', "Solr query for '$query' in " . __FUNCTION__ . " (file " . __FILE__ . ") returned no results. ", $variables = array(), $severity = WATCHDOG_WARNING, $link = NULL);
  }

  $highlights = isset($json['highlighting']) ? $json['highlighting'] : NULL;
  return array($tcus, $highlights);
}

/**
 * This will use the islandora functions to return the datastream that is specified by the $url.
 * 
 * @param type $url
 */
function islandora_oralhistories_get_datastream_from_url($url) {
  // This is a bit of a hacky way to pull the $pid and $dsid from the $url for the datastream view.
  @list($junk, $keep) = explode("/islandora/object/", $url);
  @list($pid, $keep) = explode("/datastream/", $keep);
  $pid = urldecode($pid);
  $dsid = str_replace("/view", "", $keep);

  $islandora_object = islandora_object_load($pid);
  return ((is_object($islandora_object) && (isset($islandora_object[$dsid]))) ? $islandora_object[$dsid]->content : '');
}

/**
 * We want to display vtt transcript in similar way to xml transcript.
 *
 * @param object $ui
 *   UI object.
 *
 * @return array
 *   Array of tcues in format needed for Transcripts UI.
 */
function compose_vtt_transcript_as_transcripts_ui($ui) {

  $options = $ui->options;
  $url = $options["url"];

  // Get content from VTT file.
  $content = islandora_oralhistories_get_datastream_from_url($url);

  // Parse the VTT for tcues data.
  $cues = parse_vtt($content);

  // Loop and put in a format needed by transcripts_ui
  $tcus = array();
  $i = 0;
  foreach ($cues as $cue) {
    $tcus[$i] = new stdClass();
    $tcus[$i]->trid = $i;
    $tcus[$i]->id = $i;
    $tcus[$i]->start = (float) $cue['start_time'];
    $tcus[$i]->end = (float) $cue['end_time'];
    $tcus[$i]->or_transcript = $cue['text'];
    $tcus[$i]->or_speaker = $cue['name'];
    $i++;
  }
  // VTT does not have highlights ?
  $highlights = NULL;
  return array($tcus, $highlights);
}

<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Helper function to build transcript content.
 *
 * @param string $url
 *   string with file url.
 *
 * @return string
 *   All cues in vtt file.
 */
function vtt_transcript_content($url) {
  $content = file_get_contents($url);
  $output = parse_vtt($content);

  return $output;
}

/**
 * Helper function to parse vtt file.
 *
 */
function parse_vtt($data = '') {
  $output = '';
  $cues = array();
  $lines = preg_split('/\n\n/', $data);
  foreach ($lines as $line) {
    if ($line == 'WEBVTT') {
      continue;
    }
    if (!empty($line)) {
      $parts = preg_split('/\n/', $line);
      $time = explode(' --> ', $parts[0]);
      //dpm($time, 'time');
      $start_time = time_seconds($time[0]);
      $end_time = time_seconds($time[1]);
      $text = preg_replace('/</', '&lt;', $parts[1]);
      $text = preg_replace('/>/', '&gt;', $text);
      $cues[] = array(
        'start_time' => $start_time,
        'end_time' => $end_time,
        'text' => $text
      );
    }
  } // end foreach
  $i = 0;
  foreach ($cues as $cue) {
    $output .= '<p><span class="transcript" id="transcript' . $i . '" ';
    $output .= 'data-start-time="' . $cue['start_time'] . '" ';
    $output .= 'data-end-time="' . $cue['end_time'] . '">';
    $output .= $cue['text'] . '</span></p>';
    $i++;
  }
  return $output;
}


/**
 * Helper function to convert timestamp to seconds.
 *
 */
function time_seconds($time = '') {
  $t = explode(':', $time);
  $sec = (float)($t[count($t)-1]) + (int)($t[count($t)-2]) * 60;
  if (count($t) > 2) {
    $sec += (int)($t[count($t)-3]) * 3600;
  }
  return $sec;
}

/**
 * Helper function to convert seconds to mm:ss.ttt.
 *
 */
function time_mm_ss($seconds) {
  $t = round((float)$seconds, 3);
  return sprintf('%02d:%02d.%03d', ($t/60), ($t%60), (fmod($t, 1.0) * 1000));
}


/**
 * Helper function to prepare transcript content display.
 *
 */
function xml_transcript_content($trid) {
  // how will options get through when function is not called as part of page request?
  $options = array(
    'term' => isset($_GET['term']) ? '"' . $_GET['term'] . '"' : '',
  );

  $ui = transcripts_ui_ui('islandora_oralhistories', $trid, $options);

  if ($ui->tcuCount > 0) {
    return theme('islandora_transcript_scroller', transcripts_ui_render($ui));
  } else {
    return t("This transcript is still being processed for display.");
  }
}

function islandora_oralhistories_transcripts_ui_transcript($ui) {
  $highlights = NULL;

  $tcus = array();
  $tier = array();
  $speaker = array();

  // Now we query solr fields to buid $tcus
  $pid = str_replace('-', ':', $ui->shorttrid);

  $qualifier = 'PID:' . '"' . $pid . '"';
  // solr fields to be returned.
  $fields = array('PID', 'or_cue_id', 'or_start', 'or_end');
  foreach ($ui->tiers as $key => $value) {
    $tier[] = $key;
  }
  foreach ($ui->speakernames as $key => $value) {
    $speaker[] = $key;
  }
  $fields = array_merge($fields, $tier);
  $fields = array_merge($fields, $speaker);
  $params = array(
    'fl' => $fields,
  );

  // Build solr query.
  $query = "$qualifier AND " . 'RELS_EXT_hasModel_uri_mt:"islandora:oralhistoriesCModel"';

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

  $doc = $json['response']['docs'][0] ? $json['response']['docs'][0] : NULL;
  $counter = count($doc['or_cue_id']);

  for ($i = 0; $i < $counter; $i++) {
    $tcus[$i] = new stdClass();
    $tcus[$i]->trid = str_replace(':', '-', $doc['PID']);
    $tcus[$i]->id = $doc['or_cue_id'][$i];
    $tcus[$i]->start = (float)$doc['or_start'][$i];
    $tcus[$i]->end = (float)$doc['or_end'][$i];

    // Muti tiers and speakernames are parsed.
    foreach ($tier as $t) {
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
  return array($tcus, $highlights);
}
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
 * Helper function to prepare transcript content display.
 *
 */
function xml_transcript_content($trid, $tiers) {
  // how will options get through when function is not called as part of page request?
  $options = array(
    'term' => isset($_GET['term']) ? '"' . $_GET['term'] . '"' : '',
  );

  // prefix 'or_' to match our solr fields.
  $solr_tiers = array();
  foreach ($tiers as $key => $value) {
    $solr_key = 'or_' . $key;
    $solr_tiers[$solr_key] = trim($value);
  }

  variable_set('transcripts_ui_tiers',$solr_tiers);

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

  $tcus[0] = new stdClass();
  $tcus[0]->id = 'or-99000-1'; // tcu id
  $tcus[0]->speaker = 'Anna';
  $tcus[0]->start = 0.2;
  $tcus[0]->end = 22.2;
  $tcus[0]->or_transcript = 'How is everything going?';
  $tcus[0]->or_annotation = 'smiling...';
  $tcus[0]->trid = 'or-99000'; //PID replace : with -

  $tcus[1] = new stdClass();
  $tcus[1]->id = 'or-99000-2'; // tcu id
  $tcus[1]->speaker = 'Peter';
  $tcus[1]->start = 21;
  $tcus[1]->end = 35.2;
  $tcus[1]->or_transcript = 'Hey I am fine! How about you?';
  $tcus[1]->or_annotation = 'Bang noise... stop reading...';
  $tcus[1]->trid = 'or-99000'; //PID replace : with -

  return array($tcus, $highlights);
}
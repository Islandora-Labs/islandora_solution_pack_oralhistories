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
function xml_transcript_content($data = '') {
  $output = '';

  return $output;
}

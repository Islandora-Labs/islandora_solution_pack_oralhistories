<?php
/**
 * @file
 * Create derivatives from TRANSCRIPT xml file.
 */

/**
 * This function will create an MP4 file for upload.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param bool $force
 *   Whether derivative generation is being forced or not.
 *
 * @return array
 *   An array describing the outcome of the MP4 creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_oralhistories_create_vtt(AbstractObject $object, $force = TRUE) {
  module_load_include('inc', 'islandora_oralhistories', 'includes/utilities');
  if ($force || !isset($object['MEDIATRACK'])) {
    if ($object['TRANSCRIPT']->mimetype == 'text/vtt') {
      // @todo: uploaded as vtt file, so probably just copy the TRANSCRIPT DS?
      return;
    }
    elseif ($object['TRANSCRIPT']->mimetype == 'application/xml') {
      $transcript = $object['TRANSCRIPT']->content;
      $cues = simplexml_load_string($transcript);
      dpm($cues, '$cues');
      $vtt = "WEBVTT" . PHP_EOL;
      // A blank line, which is equivalent to two consecutive newlines.
      $vtt .= PHP_EOL;
      foreach ($cues as $cue) {
        $start = time_mm_ss((string)$cue->start);
        $end = time_mm_ss((string)$cue->end);
        $vtt .= $start . ' --> ' . $end . PHP_EOL;
        $vtt .= (string)$cue->transcript . PHP_EOL;
        $vtt .= PHP_EOL;
      }
      if ($cues->cue->transcript['xml:lang']) {
        $dsid = 'MEDIATRACK-' . strtoupper((string)$cues->cue->transcript['xml:lang']);
      }
      else {
        $dsid = 'MEDIATRACK';
      }
      $filename = $dsid . '.vtt';
      $dest = file_build_uri($filename);
      $file = file_save_data($vtt, $dest, FILE_EXISTS_REPLACE);
      islandora_oralhistories_add_datastream($object, $dsid, $file->uri);
      file_delete($file);
    }
  }
}

/**
 * Adds the given file as a datastream to the given object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 *
 * @return array
 *   An array describing the outcome of the datastream addition.
 */
function islandora_oralhistories_add_datastream(AbstractObject $object, $datastream_id, $file_uri) {
  try {
    $ingest = !isset($object[$datastream_id]);
    $mime_detector = new MimeDetect();

    if ($ingest) {
      $ds = $object->constructDatastream($datastream_id, "M");
      $ds->label = $datastream_id;
    }
    else {
      $ds = $object[$datastream_id];
    }
    $ds->mimetype = $mime_detector->getMimetype($file_uri);
    $ds->setContentFromFile(drupal_realpath($file_uri));
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    return array(
      'success' => TRUE,
      'messages' => array(
        array(
          'message' => t('Created @dsid derivative for (@pid).'),
          'message_sub' => array(
            '@dsid' => $datastream_id,
            '@pid' => $object->id,
          ),
          'type' => 'dsm',
        ),
      ),
    );
  }
  catch (exception $e) {
    return array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('Oral Histories solution pack failed to add @dsid datastream for @pid. Error message: @message<br/>Stack: @stack'),
          'message_sub' => array(
            '@dsid' => $datastream_id,
            '@pid' => $object->id,
            '@message' => $e->getmessage(),
            '@stack' => $e->getTraceAsString(),
          ),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
}
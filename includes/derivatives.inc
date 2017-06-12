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

      // Get solespeaker if element is present
      $solespeaker = "";
      if(isset($cues->solespeaker)){
        $solespeaker = "<v " . (string)$cues->solespeaker . ">";
      }

      $vtt = "WEBVTT" . PHP_EOL;
      // A blank line, which is equivalent to two consecutive newlines.
      $vtt .= PHP_EOL;
      foreach ($cues as $key => $cue) {

        // Don't add to vtt cue if element is not cue
        if($key == "solespeaker"){
          continue;
        }

        // Get speaker value, if solespeaker set, overwrite it with that
        // Related to Redmine#6753 and Github#2
        $speaker = "";
        if(isset($cue->speaker)){
          $speaker = "<v " . (string)$cue->speaker . ">";
        }
        if($solespeaker != ""){
          $speaker = $solespeaker;
        }

        $start = time_mm_ss((string)$cue->start);
        $end = time_mm_ss((string)$cue->end);
        $vtt .= $start . ' --> ' . $end . PHP_EOL;
        // Remove empty lines.  Not supported in VTT.
        $transcript_text = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", (string)$cue->transcript);
        $vtt .= $speaker . $transcript_text . PHP_EOL;
        $vtt .= PHP_EOL;
      }

      $transcript_fileName = $object['TRANSCRIPT']->label;
      $dsid_suffix = get_dsid_suffix($transcript_fileName);

      $dsid = 'MEDIATRACK' . $dsid_suffix;
      $filename = $dsid . '.vtt';
      $dest = file_build_uri($filename);
      $file = file_save_data($vtt, $dest, FILE_EXISTS_REPLACE);
      $result = islandora_oralhistories_add_datastream($object, $dsid, $file->uri);
      file_delete($file);
      return $result;
    }
  }
}

/**
 * Gets the language suffix from transcript xml file if there is any.
 */

function get_dsid_suffix($transcript_fileName) {
  $pathInfo = pathinfo($transcript_fileName);
  $transcript_fileName = $pathInfo['filename'];

  $dsid_suffix = "";
  $dsid_arr = explode("_", $transcript_fileName);
  if (isset($dsid_arr[1])) {
    $dsid_suffix = strtolower($dsid_arr[1]);
    $dsid_suffix = "_" . $dsid_suffix;
  }
  return $dsid_suffix;
}

/**
 * This function will create an xml derivative datastream
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
function islandora_oralhistories_create_vtt_indexing_datastream(AbstractObject $object, $force = TRUE, $hook) {
  module_load_include('inc', 'islandora_oralhistories', 'includes/utilities');
  try
  {
    $destination_dsid = $hook['destination_dsid'];
    $source_dsid = $hook['source_dsid'];

    $filename = $destination_dsid . '.xml';
    $dest = file_build_uri($filename);
    $content = $object[$source_dsid]->content;

    $cues = parse_vtt($content);
    $xml = new SimpleXMLElement('<?xml version="1.0" encoding="utf-8"?><cues></cues>');

    foreach ($cues as $cue) {
      $newCue = $xml->addChild('cue');
      $newCue->addChild('start', (float)$cue['start_time']);
      $newCue->addChild('end', (float)$cue['end_time']);

      $child = $newCue->addChild('speaker');
      $child_node = dom_import_simplexml($child);
      $child_owner = $child_node->ownerDocument;
      $child_node->appendChild($child_owner->createCDATASection($cue['name']));

      $lang_code = "";
      $dsid_arr = explode("_", $destination_dsid);
      if (isset($dsid_arr[1])) {
        $lang_code = "_" . strtolower($dsid_arr[1]);
      }

      $child = $newCue->addChild('vtt_text' . $lang_code);
      $child_node = dom_import_simplexml($child);
      $child_owner = $child_node->ownerDocument;
      $child_node->appendChild($child_owner->createCDATASection($cue['text']));
    }

    $contentXML = $xml->asXML();
    $file = file_save_data($contentXML, $dest, FILE_EXISTS_REPLACE);
    islandora_oralhistories_add_datastream($object, $destination_dsid, $file->uri);
    file_delete($file);
  }
  catch (exception $e) {
    watchdog('Islandora Oralhistories', 'Unable to create vtt indexing datastream: ' . $e->getmessage());
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
    $ds_mimetype = $mime_detector->getMimetype($file_uri);

    if ($ds->mimeType != $ds_mimetype) {
      $ds->mimeType = $ds_mimetype;
    }

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
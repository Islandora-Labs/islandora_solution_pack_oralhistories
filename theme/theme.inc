<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements hook_preprocess().
 */
function islandora_oralhistories_preprocess_islandora_oralhistories(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora_oralhistories', 'includes/utilities');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $module_path = drupal_get_path('module', 'islandora_oralhistories');

  // Add required bootstrap-select css and js files.
  drupal_add_css('//cdnjs.cloudflare.com/ajax/libs/bootstrap-select/1.6.4/css/bootstrap-select.min.css', 'external');
  drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/bootstrap-select/1.6.4/js/bootstrap-select.min.js', 'external');

  // Add videojs.
  drupal_add_css($module_path . '/lib/videojs/video-js.css');
  drupal_add_js($module_path . '/lib/videojs/videojs.js');

  // Use jquery.ui tabs.
  drupal_add_library('system', 'ui.tabs');

  $object = $variables['object'];
  $repository = $object->repository;
  // We should eventually remove the DC object and dc_array code as it only
  // exists to not break legacy implementations.
  try {
    if (isset($object['DC']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['DC'])) {
      $dc = $object['DC']->content;
      $dc_object = DublinCore::importFromXMLString($dc);
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_oralhistories__' . str_replace(':', '_', $object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);

  // Construct $variables['islandora_content'] dynamically.
  $variables['islandora_content'] = array();

  // Prepare viewer params for media file.
  $viewer_params = array(
    'pid' => $object->id,
  );

  // Deferred derivative generation makes it possible that there won't be a
  // media dsid immediately.
  $media_dsid = FALSE;
  if (preg_match('/^video/i', $object['OBJ']->mimetype)) {
    $viewer_params += array(
      'media_tag' => 'video',
    );
    if (isset($object['MP4'])) {
      $media_dsid = 'MP4';
    }
    else {
      if (variable_get('islandora_video_play_obj', TRUE)) {
        if ($object['OBJ'] && $object['OBJ']->mimetype == 'video/mp4') {
          $max_file_size = ((int) variable_get('islandora_video_max_obj_size', 500)) * 1024 * 1024;
          if ($object['OBJ']->size < $max_file_size) {
            $media_dsid = 'OBJ';
          }
        }
      }
    }

  }

  if (preg_match('/^audio/i', $object['OBJ']->mimetype)) {
    $media_dsid = 'PROXY_MP3';
    $viewer_params += array(
      'media_tag' => 'audio',
    );

  }

  try {
    if ($media_dsid && isset($object[$media_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$media_dsid])) {
      $media_url = url("islandora/object/{$object->id}/datastream/$media_dsid/view", array('absolute' => TRUE));
      $viewer_params += array(
        'mime' => $object[$media_dsid]->mimetype,
        'url' => $media_url,
      );
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving datastream %s %t', array('%s' => $media_dsid, '%t' => $e->getMessage())), 'error', FALSE);
  }

  // Thumbnail.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $viewer_params += array(
      'tn' => url("islandora/object/{$object->id}/datastream/TN/view", array('absolute' => TRUE)),
    );
  }

  // Get all transcript datastreams if more than one?
  $tracks = array();
  $transcript_mimetype = '';
  foreach ($object as $datastream) {

    $isMediatrack = FALSE;
    $get_track = FALSE;
    if (preg_match('/^TRANSCRIPT/', $datastream->id)) {
      if (strpos($datastream->mimetype, 'xml') !== FALSE) {
        $transcript_mimetype = 'xml';
      }
      else {
        $transcript_mimetype = 'vtt';
        $isMediatrack = TRUE;
      }
      $get_track = TRUE;
    }

    if (preg_match('/^MEDIATRACK/', $datastream->id)) {
      $get_track = TRUE;
      $isMediatrack = TRUE;
    }

    // Ensure that even if MEDIATRACK is not present, show TRANSCRIPT
    // This works, but need to get more clarity as to how the TRANSCRIPT
    // and MEDIATRACK content are handled at the front end.
    if ($get_track == TRUE) {
      $track = array();

      $source_url = url("islandora/object/{$object->id}/datastream/$datastream->id/view", array('absolute' => TRUE));

      $track['dsid'] = $datastream->id;
      $track['dslabel'] = $datastream->label;
      $track['source_url'] = $source_url;

      $track['MEDIATRACK'] = FALSE;
      if ($isMediatrack) {
        $track['MEDIATRACK'] = TRUE;

        // Language Code.
        $lang_code = 'en';
        $dsid_arr = explode("_", $track['dsid']);
        $dslabel_arr = explode("_", $track['dslabel']);
        if (isset($dsid_arr[1])) {
          $track['lang_code'] = strtolower($dsid_arr[1]);
        } elseif ($track['dsid'] == "TRANSCRIPT" && isset($dslabel_arr[1])) {
          $fileName = strtolower($dslabel_arr[1]);
          $language_code = pathinfo($fileName, PATHINFO_FILENAME);
          $track['lang_code'] = $language_code;
        } else {
          $track['lang_code'] = $lang_code;
        }

        // Default Language.
        $track_defaults_setting_str = "";
        if (NULL !== variable_get('islandora_oralhistories_vtt_default_language')) {
          $track_defaults_setting_str = variable_get('islandora_oralhistories_vtt_default_language');
        }
        $track_defaults_setting_str = str_replace(' ', '', $track_defaults_setting_str);
        $track_defaults_setting_str = explode(",", $track_defaults_setting_str);

        $track['default'] = "";
        if (in_array($track['lang_code'], $track_defaults_setting_str)) {
          $track['default'] = "default";
        }
      } else {
        $track['lang_code'] = '';
        $track['default'] = '';
      }

      if (isset($object[$datastream->id]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$datastream->id])) {
        $tracks[] = $track;
      }
    }

  }
  if (!empty($tracks)) {
    $viewer_params += array(
      'tracks' => $tracks,
    );
  }

  if (variable_get('islandora_oralhistories_enbable_transcript_display') && !empty($tracks)) {
    foreach ($tracks as $track) {
      $trid = str_replace(':', '-', $object->id);
      switch ($transcript_mimetype) {
        case 'vtt':
          $viewer_params += array(
            'transcript_content' => vtt_transcript_content($track['source_url'], $trid),
          );
          break;

        case 'xml':
          $viewer_params += array(
            'transcript_content' => xml_transcript_content($trid),
          );
          break;
      }
    }

  }

  $viewer_params += array(
    'enable_caption_display' => variable_get('islandora_oralhistories_enable_caption_display'),
    'enable_transcript_display' => variable_get('islandora_oralhistories_enbable_transcript_display'),
  );

  $viewer = theme('islandora_html5_video_stack', array('params' => $viewer_params));

  $variables['islandora_content']['viewer'] = $viewer;

  // Pass settings to javascript.
  $js_settings = array(
    'objectId' => $object->id,
    'enableTranscriptDisplay' => variable_get('islandora_oralhistories_enable_transcript_display'),
    'enableCaptionDisplay' => variable_get('islandora_oralhistories_enable_caption_display'),
    'tracks' => $tracks,
  );

  if ($object->models[0] == 'islandora:oralhistoriesCModel') {
    drupal_add_js(array('islandoraOralhistories' => $js_settings), 'setting');
    drupal_add_js($module_path . '/js/oralhistories.js');
  }
  return array('' => $viewer);
}

/**
 * Implements hook_preprocess_HOOK().
 */
function islandora_oralhistories_preprocess_islandora_native_html5(array &$variables) {
  $params = $variables['params'];
  $variables['tn'] = $params['tn'];
  $variables['sources'][] = array('url' => $params['url'], 'mime' => $params['mime']);
  $variables['tracks'] = $params['tracks'];
  $variables['enable_caption_display'] = $params['enable_caption_display'];
  $variables['enable_transcript_display'] = $params['enable_transcript_display'];
  $variables['transcript_content'] = $params['transcript_content'];

}

/**
 * Implements hook_process_HOOK().
 */
function islandora_oralhistories_process_islandora_html5_video_stack(&$variables) {
  // The trid-PID with : being replaced by -
  $variables['params']['trid'] = 'trid-' . str_replace(':', '-', $variables['params']['pid']);
}

/**
 * Implements theme_islandora_oralhistories_transcript_edit_form().
 */
function theme_islandora_oralhistories_transcript_edit_form($variables) {
  // Renders all elements of the transcript edit form.
  return drupal_render_children($variables['form']);
}

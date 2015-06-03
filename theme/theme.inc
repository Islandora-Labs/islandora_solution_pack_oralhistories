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

  // Add required css and js files.
  drupal_add_css('https://cdnjs.cloudflare.com/ajax/libs/bootstrap-select/1.6.4/css/bootstrap-select.min.css', 'external');
  drupal_add_js('https://cdnjs.cloudflare.com/ajax/libs/bootstrap-select/1.6.4/js/bootstrap-select.min.js', 'external');

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

  // construct $variables['islandora_content'] dynamically.
  $variables['islandora_content'] = array();
  $viewer_params = array();

  // prepare viewer params for media file.
  try {
    if (isset($object['MP4']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MP4'])) {
      $media_dsid = 'MP4';
      $mime = 'video/mp4';
      $viewer_params = array(
        'pid' => $object->id,
      );
      $viewer_params += array(
        'media_tag' => 'video',
      );

      // Video player.
      if (isset($object[$media_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$media_dsid])) {
        $media_url = url("islandora/object/{$object->id}/datastream/$media_dsid/view");
        $viewer_params += array(
          'mime' => $mime,
          'url' => $media_url,
        );
      }
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving datastream %s %t', array('%s' => $media_dsid, '%t' => $e->getMessage())), 'error', FALSE);
  }

  try {
    if (isset($object['PROXY_MP3']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PROXY_MP3'])) {
      $media_dsid = 'PROXY_MP3';
      $mime = 'audio/mpeg';

      $viewer_params = array(
        'pid' => $object->id,
      );
      $viewer_params += array(
        'media_tag' => 'audio',
      );
      // Audio player.
      if (isset($object[$media_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$media_dsid])) {
        $media_url = url("islandora/object/{$object->id}/datastream/$media_dsid/view");
        $viewer_params += array(
          'mime' => $mime,
          'url' => $media_url,
        );
      }
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

  // get all transcript datastreams if more than one?
  $tracks= array();
  $transcript_mimetype = '';
  foreach ($object as $datastream) {
    if (preg_match('/TRANSCRIPT/', $datastream->id)) {
      if (strpos($datastream->mimetype,'xml') !== false) {
        $transcript_mimetype = 'xml';
      }
      else {
        $transcript_mimetype = 'vtt';
      }
    }
    if (preg_match('/MEDIATRACK/', $datastream->id)) {
      $track = array();

      $source_url = url("islandora/object/{$object->id}/datastream/$datastream->id/view", array('absolute' => TRUE));

      // @todo: how to determine language code? default to 'en'.
      $lang_code = 'en';

      $track['dsid'] = $datastream->id;
      $track['lang_code'] = $lang_code;
      $track['source_url'] =  $source_url;


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
      switch ($transcript_mimetype) {
        case 'vtt':
          $viewer_params += array(
            'transcript_content' => vtt_transcript_content($track['source_url']),
          );
          break;

        case 'xml':
          $trid = str_replace(':', '-', $object->id);
          $tiers = variable_get('islandora_oralhistories_transcripts_viewer_tiers');
          $viewer_params += array(
            'transcript_content' => xml_transcript_content($trid, $tiers),
          );
          break;
      }
    }

  }

  $viewer_params += array(
    'enable_caption_display' => variable_get('islandora_oralhistories_enable_caption_display'),
    'enable_transcript_display' => variable_get('islandora_oralhistories_enbable_transcript_display'),
  );

//  $viewer = theme('islandora_native_html5', array('params' => $viewer_params));

  $viewer = theme('islandora_html5_video_stack', array('params' => $viewer_params));
  $variables['islandora_content']['viewer'] = $viewer;

  // pass settings to javascript.
  $js_settings = array(
    'enbableTranscriptDisplay' => variable_get('islandora_oralhistories_enbable_transcript_display'),
    'enbableCaptionDisplay' => variable_get('islandora_oralhistories_enable_caption_display'),
    'tracks' => $tracks,
  );

  if (variable_get('islandora_oralhistories_enbable_transcript_display') && $object->models[0] == 'islandora:oralhistoriesCModel' ) {
    drupal_add_js(array('islandora_oralhistories' => $js_settings), 'setting');
    drupal_add_js(drupal_get_path('module', 'islandora_oralhistories') . '/js/oralhistories.js');
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
 * Implements hook_theme_registry_alter().
 */
function islandora_oralhistories_theme_registry_alter(&$theme_registry) {
  // Override the default islandora-videojs.tpl.php.
  if (isset($theme_registry['islandora_videojs'])) {
    $module_path = drupal_get_path('module', 'islandora_oralhistories');
    $theme_registry['islandora_videojs']['theme path'] = $module_path;
    $theme_registry['islandora_videojs']['template'] = $module_path . '/theme/islandora-videojs';
  }
}

/**
 * Implements hook_process_HOOK().
 */
function islandora_oralhistories_process_islandora_videojs(array &$variables) {
  $params = $variables['params'];
  $variables['tracks'] = $params['tracks'];
  $variables['enable_caption_display'] = $params['enable_caption_display'];
}

/**
 * Implements hook_theme().
 */
function theme_islandora_html5_video_stack($vars){
  $trid ='trid-' . str_replace(':', '-', $vars['params']['pid']); // trid-PID with : being replaced by -
  $url = $vars['params']['url'];
  $enable_transcript_display = $vars['params']['enable_caption_display'];
  $tracks = array_key_exists('tracks', $vars['params']) ? $vars['params']['tracks'] : NULL;
  $out = "<div data-transcripts-role='video' data-transcripts-id='{$trid}'>";
  $out .= "<div align='center' class='embed-responsive embed-responsive-4by3'>";
  $out .= "<video class='embed-responsive-item' controls>";
  $out .= "<source src='$url' />";
  if ($tracks && $enable_transcript_display) {
    foreach ($tracks as $key => $track) {
      $out .= '<track id="track' . $key . '" src="' . $track['source_url'] . '" srclang="' . $track['lang_code'] . '"
           kind="captions" label="' . $track['lang_code']. '" default>';
    }
  }
  $out .= "</video>";
  $out .= "</div>";
  $out .= "</div>";
  if  (array_key_exists('transcript_content', $vars['params'])) {
    $out .= $vars['params']['transcript_content'];
  }
  return $out;
}
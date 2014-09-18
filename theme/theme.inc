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
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
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
  $variables['islandora_content'] = '';
  // get viewer for media file.
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
      $media_viewer_type = 'islandora_video_viewers';
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
      $media_viewer_type = 'islandora_audio_viewers';
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

  // TRANSCRIPT url.
  if (isset($object['TRANSCRIPT']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TRANSCRIPT'])) {
    $viewer_params += array(
      'transcript_url' => url("islandora/object/{$object->id}/datastream/TRANSCRIPT/view", array('absolute' => TRUE)),
    );
  }
  $viewer = islandora_get_viewer($viewer_params, $media_viewer_type , $object);

  if ($viewer) {
    $variables['islandora_content'] = $viewer;
  }
  else {
    $variables['islandora_content'] = <<<EOT
<{$viewer_params['media_tag']} id="islandora-oralhistories-media-player" class="media-default-skin"
               controls preload="auto" width="640px" height="264px"
               poster="{$viewer_params['tn']}">
          <source src="{$viewer_params['url']}" type="{$viewer_params['mime']}" />
          <track kind="captions" src="{$viewer_params['transcript_url']}" srclang="en" label="English" default>
    </{$viewer_params['media_tag']}>
EOT;
}
  $tracks= array();
  foreach ($object as $datastream) {
    if (preg_match('/TRANSCRIPT/', $datastream->id)) {
      $tracks[] = array(
        'dsid' => $datastream->id,
        'file_name' => $datastream->label,
        'content' => $datastream->content,
        'source_url' => url("islandora/object/{$object->id}/datastream/$datastream->id/view", array('absolute' => TRUE)),
      );
    }
  }
  dpm($tracks, 'Track list');
  dpm($object['TRANSCRIPT']->content, 'TRANSCRIPT datastream');
  dpm($object['TRANSCRIPT']->label, 'TRANSCRIPT label');
  dpm($viewer, 'viewer');
  $js_settings = array(
    'enbableTranscriptDisplay' => variable_get('islandora_oralhistories_enbable_transcript_display'),
    'enableCaptionDisplay' => variable_get('islandora_oralhistories_enable_caption_display'),
    'tracks' => $tracks,
  );
  dpm(variable_get('islandora_oralhistories_enable_caption_display'), 'caption display');
  if (variable_get('islandora_oralhistories_enbable_transcript_display') && $object->models[0] == 'islandora:oralhistoriesCModel' ) {
    drupal_add_js(array('islandora_oralhistories' => $js_settings), 'setting');
    drupal_add_js(drupal_get_path('module', 'islandora_oralhistories') . '/js/oralhistories.js');
  }

  return array('' => $viewer);
}

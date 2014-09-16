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


  try {
    if (isset($object['MP4']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MP4'])) {
      $viewer_dsid = 'MP4';
      $mime = 'video/mp4';
      $media_params = array(
        'pid' => $object->id,
      );
      $media_params += array(
        'media_tag' => 'video',
      );

      // Video player.
      if (isset($object[$viewer_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$viewer_dsid])) {
        $media_url = url("islandora/object/{$object->id}/datastream/$viewer_dsid/view");
        $media_params += array(
          'mime' => $mime,
          'url' => $media_url,
        );
      }
      $media_viewer_type = 'islandora_video_viewers';
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving datastream %s %t', array('%s' => $viewer_dsid, '%t' => $e->getMessage())), 'error', FALSE);
  }

  try {
    if (isset($object['PROXY_MP3']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PROXY_MP3'])) {
      $viewer_dsid = 'PROXY_MP3';
      $mime = 'audio/mpeg';

      $media_params = array(
        'pid' => $object->id,
      );
      $media_params += array(
        'media_tag' => 'audio',
      );
      // Audio player.
      if (isset($object[$viewer_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$viewer_dsid])) {
        $media_url = url("islandora/object/{$object->id}/datastream/$viewer_dsid/view");
        $media_params += array(
          'mime' => $mime,
          'url' => $media_url,
        );
      }
      $media_viewer_type = 'islandora_audio_viewers';
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving datastream %s %t', array('%s' => $viewer_dsid, '%t' => $e->getMessage())), 'error', FALSE);
  }


  // Thumbnail.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $media_params += array(
      'tn' => url("islandora/object/{$object->id}/datastream/TN/view", array('absolute' => TRUE)),
    );
  }

  // TRANSCRIPT url.
  if (isset($object['TRANSCRIPT']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TRANSCRIPT'])) {
    $media_params += array(
      'transcript_url' => url("islandora/object/{$object->id}/datastream/TRANSCRIPT/view", array('absolute' => TRUE)),
    );
  }
  $viewer = islandora_get_viewer($media_params, $media_viewer_type , $object);
  $variables['islandora_content'] = '';
  if ($viewer) {
    $variables['islandora_content'] = $viewer;
  }
  else {
    $variables['islandora_content'] = <<<EOT
<{$media_params['media_tag']} id="islandora-oralhistories-media-player" class="media-default-skin"
               controls preload="auto" width="50%" height="25%"
               poster="{$media_params['tn']}">
          <source src="{$media_params['url']}" type="{$media_params['mime']}" />
          <track kind="captions" src="{$media_params['transcript_url']}" srclang="en" label="English" default>
    </{$media_params['media_tag']}>
EOT;
}
  return array('' => $viewer);
}

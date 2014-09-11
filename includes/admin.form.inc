<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_oralhistories_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form = array();
  // Get viewer table.
  $viewer_table = islandora_viewers_form('islandora_oralhistories_viewers', 'video/mp4');
  $form += $viewer_table;

  // Viewer stuff.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_oralhistories_admin_submit'),
  );

  // Misc config.
  $form['islandora_oralhistories_video_retain_original'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep original file after ingest'),
    '#description' => t('Stores the originally uploaded video file for archival purposes.'),
    '#default_value' => variable_get('islandora_oralhistories_video_retain_original', TRUE),
  );

  // FFMPEG configuration.
  $form['islandora_oralhistories_video_ffmpeg_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to ffmpeg executable.'),
    '#default_value' => variable_get('islandora_oralhistories_video_ffmpeg_path', 'ffmpeg'),
    '#description' => t('Path to ffmpeg on your server.'),
    '#required' => TRUE,
  );
  $form['islandora_oralhistories_video_make_archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create .mkv derivative'),
    '#description' => t('Appends a video datastream to the object using the Matroska container.'),
    '#default_value' => variable_get('islandora_oralhistories_video_make_archive', TRUE),
  );
  $form['islandora_oralhistories_video_make_mp4_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create display MP4 locally'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_oralhistories_video_make_mp4_locally', TRUE),
  );

  // Theora configuration.
  $form['islandora_oralhistories_video_make_ogg_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create OGG locally'),
    '#description' => t("Check this box if an optional OGG archival datastream is desired and should being created locally. Please note that OGG derivatives are no longer used within Islandora and this option remains to be consistent."),
    '#default_value' => variable_get('islandora_oralhistories_video_make_ogg_locally', FALSE),
  );
  $form['islandora_oralhistories_video_ffmpeg2theora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Theora executable'),
    '#description' => t('Path to Theora conversion program on your server'),
    '#default_value' => variable_get('islandora_oralhistories_video_ffmpeg2theora_path', 'ffmpeg2theora'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_oralhistories_video_make_ogg_locally"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['islandora_oralhistories_video_make_thumbnail_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create thumbnail locally'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_oralhistories_video_make_thumbnail_locally', TRUE),
  );
  return system_settings_form($form);
}

/**
 * Form submit for allowing the deletion of the viewer variable.
 */
function islandora_oralhistories_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_oralhistories_video_viewers');
      variable_del('islandora_oralhistories_video_ffmpeg_path');
      variable_del('islandora_oralhistories_video_retain_original');
      variable_del('islandora_oralhistories_video_make_archive');
      variable_del('islandora_oralhistories_video_make_mp4_locally');
      variable_del('islandora_oralhistories_video_make_ogg_locally');
      variable_del('islandora_oralhistories_video_ffmpeg2theora_path');
      break;
  }
}
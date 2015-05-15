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
//  $viewer_table = islandora_viewers_form('islandora_oralhistories_viewers', 'video/mp4');
//  $form += $viewer_table;

  $form['islandora_oralhistories_enable_caption_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable captions/subtitles display'),
    '#description' => t('Display captions or subtitles on video player.'),
    '#default_value' => variable_get('islandora_oralhistories_enable_caption_display', FALSE),
  );
  $form['islandora_oralhistories_enbable_transcript_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable transcript display'),
    '#description' => t('Display transcript synchronously with the video or audio.'),
    '#default_value' => variable_get('islandora_oralhistories_enbable_transcript_display', FALSE),
  );

  $form['transcripts_viewer_tier_settings'] = array(
    '#title' => t('Transcript tiers'),
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
  );
  $default_tiers = array(
    'transcript' => 'Transcript',
    'annotation' => 'Annotation',
  );
  $form['transcripts_viewer_tier_settings']['transcripts_viewer_tiers'] = array(
    '#title' => t('Tiers'),
    '#type' => 'textarea',
    '#rows' => 10,
    '#default_value' => list_allowed_values_string(variable_get('transcripts_viewer_tiers', $default_tiers)),
    '#description' => t('Enter a list of tiers, one per line, each in the form TIER_ID|TIER_NAME. TIER_ID is the element name in the transcript xml file.'),
    '#required' => TRUE,
  );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_oralhistories_admin_submit'),
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
      variable_del('islandora_oralhistories_enable_caption_display');
      variable_del('islandora_oralhistories_enbable_transcript_display');
      break;
  }

  $tiers = list_extract_allowed_values($form_state['values']['transcripts_viewer_tiers'], 'list_text', FALSE);
  $form_state['values']['transcripts_viewer_tiers'] = $tiers;
}
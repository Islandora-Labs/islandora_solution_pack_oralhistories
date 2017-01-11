<?php

function transcripts_ui_admin()
{
    $form = array();

    $form['transcripts_ui_tier_settings'] = array(
        '#title' => t('Transcript tiers'),
        '#type' => 'fieldset',
        '#collapsed' => FALSE,
    );
    $default_tiers = array(
        'ts_content_epo' => 'Esperanto',
        'ts_content_qya' => 'Quenya',
        'ts_content_sjn' => 'Sindarin',
    );
    $form['transcripts_ui_tier_settings']['transcripts_ui_tiers'] = array(
        '#title' => t('Tiers'),
        '#type' => 'textarea',
        '#rows' => 10,
        '#default_value' => list_allowed_values_string(variable_get('transcripts_ui_tiers', $default_tiers)),
        '#description' => t('Enter a list of tiers, one per line, each in the form TIER_ID|TIER_NAME.'),
        '#required' => TRUE,
    );
    $default_speaker_names = array(
        '@script' => 'Script',
        'phonetic' => 'Phonetic',
    );
    $form['transcripts_ui_tier_settings']['transcripts_ui_speaker_names'] = array(
        '#title' => t('Speaker names'),
        '#type' => 'textarea',
        '#rows' => 10,
        '#default_value' => list_allowed_values_string(variable_get('transcripts_ui_speaker_names', $default_speaker_names)),
        '#description' => t('Enter a list of speaker name display formats, one per line, each in the form SPEAKER_NAME_DISPLAY_ID|SPEAKER_NAME_DISPLAY_FORMAT.'),
        '#required' => TRUE,
    );

    $form = system_settings_form($form);
    array_unshift($form['#submit'], 'transcripts_ui_admin_submit');
    return $form;
}

function transcripts_ui_admin_submit($form, &$form_state)
{
    $tiers = list_extract_allowed_values($form_state['values']['transcripts_ui_tiers'], 'list_text', FALSE);
    $speaker_names = list_extract_allowed_values($form_state['values']['transcripts_ui_speaker_names'], 'list_text', FALSE);
    $form_state['values']['transcripts_ui_tiers'] = $tiers;
    $form_state['values']['transcripts_ui_speaker_names'] = $speaker_names;
}

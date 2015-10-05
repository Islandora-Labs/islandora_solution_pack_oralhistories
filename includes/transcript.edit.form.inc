<?php
/**
 * @file
 *
 * Provides functions to process transcript edit form.
 */

/**
 * Builds transcript_edit_form.
 *
 * @param array $form
 * @param array $form_state
 * @return array $form
 */
function islandora_oralhistories_transcript_edit_form(array $form, array &$form_state, AbstractObject $object, AbstractDatastream $datastream) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/datastream');
  $file_url = islandora_view_datastream_retrieve_file_uri($datastream);
  $xml = simplexml_load_file($file_url);
  $form = array();
  $form['#tree'] = TRUE;

  if ($xml) {
    if ($xml->solespeaker) {
      $form['sole_speaker'] = array(
        '#type' => 'textfield',
        '#title' => t('Sole Speaker'),
        '#description' => t('The sole speaker\'s name'),
        '#required' => FALSE,
        '#default_value' => $xml->solespeaker,
        '#size' => 25,
      );
    }
    // Loop through xml elements.
    // @todo we need alter transcript xml structure.
    foreach ($xml->cue as $cue) {
      $children = $cue->children();
      foreach ($children as $child) {
        $string = (string) $child;
        $form[$string] = array(
          '#type' => 'textfield',
          '#title' => $string,
          '#description' => $string,
          '#required' => FALSE,
          '#default_value' => $string,
          '#size' => 25,
        );
      }

    }
  } else {
    drupal_set_message(t('%ds does not have a valid xml content', array('%ds' => $datastream->id)), 'status');
  }



  $form['#theme'] = 'islandora_oralhistories_transcript_edit_form';
  return $form;
}
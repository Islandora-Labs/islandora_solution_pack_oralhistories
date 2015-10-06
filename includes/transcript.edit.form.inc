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
  // To remove the temp file after form is populated?
  $files = file_load_multiple(array(), array('uri' => $file_url));
  $file = reset($files); // If empty, $file will be false, otherwise will contain the required file

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
    // @todo we need alter transcript xml structure?
    for ($i = 0; $i < $xml->count(); $i++) {
      $fieldset = 'cue' . $i;
      $textarea = 1;
      foreach ($xml->cue as $cue) {
        $children = $cue->children();
        $form[$fieldset] = array(
          '#type' => 'fieldset',
          '#title' => t('Cue-@count', array('@count' => $i)),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );

        foreach ($children as $child) {
          $node_name = (string) $child->getName();
          if (preg_match('/speaker/i', $node_name) || preg_match('/start/i', $node_name) || preg_match('/end/i', $node_name)) {
            $textarea = 0;
          } else {
            $textarea = 1;
          }
          $form[$fieldset][$node_name] = array(
            '#type' => $textarea ? 'textarea': 'textfield',
            '#title' => $node_name,
            '#description' => $node_name,
            '#required' => FALSE,
            '#default_value' => (string) $child,
            '#attributes' => array(
              'class' => array('textarea' . $textarea),
            ),
          );


        }
      }
    } // End for loop
  } else {
    drupal_set_message(t('%ds does not have a valid xml content', array('%ds' => $datastream->id)), 'status');
  }
  $form['#theme'] = 'islandora_oralhistories_transcript_edit_form';
  // buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  file_delete($file, TRUE);
  return $form;
}
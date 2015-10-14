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
      $form['solespeaker'] = array(
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
      $is_textarea = TRUE;
      foreach ($xml->cue as $cue) {
        $children = $cue->children();
        $form[$fieldset] = array(
          '#type' => 'fieldset',
          '#title' => t('Cue-@count', array('@count' => $i)),
          '#collapsible' => TRUE,
          '#collapsed' => ($i == 0) ? FALSE : TRUE,
        );

        foreach ($children as $child) {
          $node_name = (string) $child->getName();
          if (preg_match('/speaker/i', $node_name) || preg_match('/start/i', $node_name) || preg_match('/end/i', $node_name)) {
            $is_textarea = FALSE;
          } else {
            $is_textarea = TRUE;
          }
          $css_class = ($is_textarea) ? 'or-transcript-textarea' : 'or-transcript-textfield';
          $form[$fieldset][$node_name] = array(
            '#type' => $is_textarea ? 'textarea': 'textfield',
            '#title' => $node_name,
            '#description' => $node_name,
            '#required' => FALSE,
            '#default_value' => (string) $child,
            '#attributes' => array(
              'class' => array($css_class),
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

function islandora_oralhistories_transcript_edit_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/datastream');
  $object = $form_state['build_info']['args'][0];
  $datastream = $form_state['build_info']['args'][1];
  if (isset($object['TRANSCRIPT']) && islandora_datastream_access(ISLANDORA_REPLACE_DATASTREAM_CONTENT, $object['TRANSCRIPT'])) {

    $dom = new DOMDocument('1.0', 'utf-8');
    $cues = $dom->appendChild($dom->createElement('cues'));
    $keys = array_keys($form_state['values']);
    foreach ($keys as $key) {
      if (preg_match('/solespeaker/', $key)) {
        $solespeaker = $cues->appendChild($dom->createElement('solespeaker', $form_state['values'][$key]));
      }
      if (preg_match('/cue/', $key)) {
        $cue = $cues->appendChild($dom->createElement('cue'));
        foreach ($form_state['values'][$key] as $cue_name => $value) {
          $cue_name = $cue->appendChild($dom->createElement($cue_name, $value));
        }

      }
    }

    $dom->preserveWhiteSpace = FALSE;
    $dom->formatOutput = TRUE;

    try {
      $datastream->setContentFromString($dom->saveXML());
      drupal_set_message(t('datastream %s is updated.', array('%s' => $datastream->id)), 'status', FALSE);
    }
    catch (Exception $e) {
      drupal_set_message(t('Error updating datastream %s %t', array('%s' => $datastream->id, '%t' => $e->getMessage())), 'error', FALSE);
    }

  }

}
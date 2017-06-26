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
  if (!$object || !$datastream) {
    $path = current_path();
    $part_parts = explode('/', $path);
    $object = islandora_object_load($part_parts[2]);
    $datastream =  $object['TRANSCRIPT'];
  }
  $file_url = islandora_view_datastream_retrieve_file_uri($datastream);

  $xml = simplexml_load_file($file_url);
  $form = array();
  $form['#tree'] = TRUE;

  // Button to toggle cues.
  $form['toggle-cues'] = array(
    '#type' => 'button',
    '#value' => 'Toggle Cues',
    '#attributes' => array('onclick' => 'return (false);'),
  );

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
    for ($i = 0; $i < $xml->count(); $i++) {
      $fieldset = 'cue' . $i;
      $is_textarea = TRUE;
      $form[$fieldset] = array(
        '#type' => 'fieldset',
        '#title' => t('Cue-@count', array('@count' => $i)),
        '#collapsible' => TRUE,
        '#collapsed' => ($i == 0) ? FALSE : TRUE,
      );
      if (is_array($xml->cue[$i]) || is_object($xml->cue[$i])) {
        foreach ($xml->cue[$i] as $child) {
          $node_name = (string) $child->getName();
          if (preg_match('/speaker/i', $node_name) || preg_match(
              '/start/i',
              $node_name
            ) || preg_match('/end/i', $node_name)
          ) {
            $is_textarea = FALSE;
          }
          else {
            $is_textarea = TRUE;
          }
          $css_class = ($is_textarea) ? 'or-transcript-textarea' : 'or-transcript-textfield';
          $form[$fieldset][$node_name] = array(
            '#type' => $is_textarea ? 'textarea' : 'textfield',
            '#title' => $node_name,
            '#description' => $node_name,
            '#required' => FALSE,
            '#default_value' => (string) $child,
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

  // Handles toggle behavior.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'islandora_oralhistories') . '/js/transcript_edit.js',
  );

  return $form;
}

/**
 * Submit handler for islandora_oralhistories_transcript_edit_form.
 * @param array $form
 * @param array $form_state
 */
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
          // issue#58
          $child = $cue->appendChild($dom->createElement($cue_name));
          $child_node = dom_import_simplexml($child);
          $child_owner = $child_node->ownerDocument;
          $child_node->appendChild($child_owner->createCDATASection($value));
        }

      }
    }

    $dom->preserveWhiteSpace = FALSE;
    $dom->formatOutput = TRUE;
    try {
      $datastream->setContentFromString($dom->saveXML());
      drupal_set_message(t('datastream %s is updated.', array('%s' => $datastream->id)), 'status', FALSE);
      $nodePath = current_path();
      $nodePath = str_replace("/edit_form/TRANSCRIPT", "", $nodePath);
      $form_state['redirect'] = $nodePath;
    }
    catch (Exception $e) {
      drupal_set_message(t('Error updating datastream %s %t', array('%s' => $datastream->id, '%t' => $e->getMessage())), 'error', FALSE);
    }

  }

}


/**
 * Callback for menu 'islandora/object/%islandora_object/edit_form/%islandora_datastream/%ctools_js'.
 *
 * @param \AbstractObject $object
 * @param \AbstractDatastream $datastream
 * @param $ajax
 * @return array|mixed
 */
function edit_transcript_modal(AbstractObject $object, AbstractDatastream $datastream, $ajax) {
  if (!$object) {
    $path = current_path();
    $part_parts = explode('/', $path);
    $object = islandora_object_load($part_parts[2]);
    $datastream =  $object['TRANSCRIPT'];
  }
  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('Edit Transcript Modal Form'),
    );
    // pass arguments to ctools_modal_form_wrapper().
    $form_state['build_info']['args'][] = $object;
    $form_state['build_info']['args'][] = $datastream;

    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    $output = ctools_modal_form_wrapper(
      'islandora_oralhistories_transcript_edit_form',
      $form_state
    );

    if (!empty($form_state['executed'])) {

      // Add the responder javascript, required by ctools
      ctools_add_js('ajax-responder');

      // Create ajax command array, dismiss the modal window.
      $output = array();
      $output[] = ctools_modal_command_dismiss();
      $nodePath = current_path();
      $nodePath = str_replace("/edit_form/TRANSCRIPT/ajax", "", $nodePath);
      $output[] = ctools_ajax_command_redirect($nodePath);
    }


    // Return the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    return drupal_get_form(
      'islandora_oralhistories_transcript_edit_form',
      $object,
      $datastream
    );
  }
}
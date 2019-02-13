<?php
/**
 * @file
 * The edit form for editing xml-based TRANSCRIPT datastreams.
 */

/**
 * Builds the TRANSCRIPT edit form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object.
 *   An AbstractObject representing a Fedora object.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_oralhistories_transcript_edit_form(array $form, array &$form_state, AbstractObject $object) {
  if (isset($object['TRANSCRIPT'])) {
    $transcript = $object['TRANSCRIPT'];

    if ($transcript->mimetype == 'application/xml' || $transcript->mimetype == 'text/xml') {
      $xml = simplexml_load_string($transcript->content);

      if ($xml) {
        $form = array();
        $form['#tree'] = TRUE;
        $form_state['object'] = $object;
        $form_state['transcript'] = $transcript;

        // Toggle cue expansion.
        $form['toggle-cues'] = array(
          '#type' => 'button',
          '#value' => t('Toggle Cues'),
          '#attributes' => array('onclick' => 'return (false);'),
        );

        // Handle the solespeaker.
        if ($xml->solespeaker) {
          $form['solespeaker'] = array(
            '#type' => 'textfield',
            '#title' => t('Sole Speaker'),
            '#description' => t("The sole speaker's name"),
            '#default_value' => t($xml->solespeaker),
            '#size' => 25,
          );
        }
        
        // Handle the cue data, expand the first cue.
        $i = 0;
        
        foreach ($xml->cue as $cue) {
          $fieldset = 'cue_' . $i;
          $form['cues'][$fieldset] = array(
            '#type' => 'fieldset',
            '#title' => t('Cue @count', array('@count' => ($i + 1))),
            '#collapsible' => TRUE,
            '#collapsed' => ($i == 0) ? FALSE : TRUE,
          );

          if (is_object($cue)) {
            $textfield_node_names = array('solespeaker', 'speaker', 'start', 'end');

            foreach ($cue as $child) {
              $node_name = $child->getName();
              $is_textarea = TRUE;

              // Check to see if cue data should be in textfield.
              if (in_array($node_name, $textfield_node_names)) {
                $is_textarea = FALSE;
              }

              $form['cues'][$fieldset][$node_name] = array(
                '#type' => $is_textarea ? 'textarea' : 'textfield',
                '#title' => t(ucfirst($node_name)),
                '#default_value' => t((string) $child),
              );
            }
          }
          
          $i++;
        }

        $form['actions'] = array('#type' => 'actions');
        $form['actions']['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Submit'),
        );
        $form['#attached']['js'] = array(drupal_get_path('module', 'islandora_oralhistories') . '/js/transcript_edit.js');
        $form['#theme'] = 'islandora_oralhistories_transcript_edit_form';
        return $form;
      }
      else {
        drupal_set_message(t('Error loading @dsid datastream XML content', array('@dsid' => $transcript->id)), 'error');
      }
    }
    else {
      drupal_set_message(t('The @dsid datastream must contain XML content', array('@dsid' => $transcript->id)), 'error');
    }
  }
}

/**
 * Submit handler for islandora_oralhistories_transcript_edit_form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_oralhistories_transcript_edit_form_submit(array $form, array &$form_state) {
  $object = $form_state['object'];
  $transcript = $form_state['transcript'];

  $dom = new DOMDocument('1.0', 'utf-8');
  $dom->preserveWhiteSpace = FALSE;
  $dom->formatOutput = TRUE;

  $cues = $dom->appendChild($dom->createElement('cues'));

  // Insert solespeaker.
  if (isset($form_state['values']['solespeaker'])) {
    $cues->appendChild($dom->createElement('solespeaker', $form_state['values']['solespeaker']));
  }

  // Insert cue data.
  if (isset($form_state['values']['cues'])) {
    foreach ($form_state['values']['cues'] as $cue) {
      $dom_cue = $cues->appendChild($dom->createElement('cue'));

      foreach ($cue as $cue_data => $value) {
        $child = $dom_cue->appendChild($dom->createElement($cue_data));
        $child_owner = $child->ownerDocument;
        $child->appendChild($child_owner->createCDATASection($value));
      }
    }
  }

  try {
    $transcript->setContentFromString($dom->saveXML());
    drupal_set_message(t('The @dsid datastream has been updated', array('@dsid' => $transcript->id)), 'status');
    $form_state['redirect'] = "islandora/object/$object->id";
  }
  catch (Exception $e) {
    drupal_set_message(t('Error updating datastream @dsid: @msg', array('@dsid' => $transcript->id, '@msg' => $e->getMessage())), 'error');
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

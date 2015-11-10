<?php
function edit_annotation_modal(AbstractObject $object, $ajax) {
  if (!$object) {
    $path = current_path();
    $part_parts = explode('/', $path);
    $object = islandora_object_load($part_parts[2]);
  }
  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('Annotation Modal Form'),
    );
    // pass arguments to ctools_modal_form_wrapper().
    $form_state['build_info']['args'][] = $object;

    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    $output = ctools_modal_form_wrapper(
      'islandora_oralhistories_annotation_form',
      $form_state
    );

    if (!empty($form_state['executed'])) {

      // Add the responder javascript, required by ctools
      ctools_add_js('ajax-responder');

      // Create ajax command array, dismiss the modal window.
      $output = array();
      $output[] = ctools_modal_command_dismiss();
    }


    // Return the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    return drupal_get_form(
      'islandora_oralhistories_annotation_form',
      $object
    );
  }
}

function islandora_oralhistories_annotation_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/datastream');
  $form = array();
  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div id="islandora-oralhistories-annotation-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['web_annotation_new'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add new annotation'),
    '#attributes' => array(
      'id' => 'web-annotation-add-new',
    ),
  );
  $form['web_annotation'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="web-annotation-container-wrapper">',
    '#suffix' => '</div>',
    '#states' => array(
      'visible' => array(
        ':input[name="web_annotation_new"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['web_annotation']['start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start time'),
    '#description' => t('Start time in seconds'),
    '#default_value' => 0.00,
    '#size' => 25,
    '#attributes' => array(
      'class' => array('web-annotation-start'),
      'id' => 'web-annotation-start-time'
    ),
  );
  $form['web_annotation']['end'] = array(
    '#type' => 'hidden',
    '#title' => t('End time'),
    '#description' => t('End time in seconds'),
    '#size' => 25,
    '#attributes' => array(
      'class' => array('web-annotation-end'),
      'id' => 'web-annotation-end-time'
    ),
  );
  $form['web_annotation']['annotation_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Annotation content'),
    '#description' => t('Annotation content'),
    '#attributes' => array(
      'class' => array('web-annotation-body'),
      'id' => 'web-annotation-body'
    ),
  );
  $form['web_annotation']['annotation_motivation'] = array(
    '#type' => 'select',
    '#title' => t('Motivation'),
    '#description' => t('Reasons to create an annotation'),
    '#options' => array(
      'oa:commenting' => t('Commenting'),
      'oa:describing' => t('Describing'),
      'oa:replying' => t('Replying'),
      'oa:reviewing' => t('Reviewing'),
      'oa:tagging' => t('Tagging'),
    ),
    '#default_value' => 'oa:commenting',
    '#attributes' => array(
      'class' => array('web-annotation-motivation'),
      'id' => 'web-annotation-motivation'
    ),
  );
  $form['web_annotation']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    "#ajax" => array(
      "callback" => "web_annotation_form_ajax_callback",
      "wrapper" => "web-annotation-container-wrapper",
    ),
  );

  return $form;
}

function web_annotation_form_ajax_callback(array $form, array &$form_state) {
  $new_state = array();
  $new_state['build_info'] = $form_state['build_info'];
  $new_state['rebuild'] = TRUE;
  $new_state['no_cache'] = TRUE;
  $new_state['values'] = array();
  $new_state += form_state_defaults();

  $new_form_array = drupal_rebuild_form('islandora_oralhistories_annotation_form', $new_state);
  $new_form = drupal_render($new_form_array);

  $commands = array();
  $commands[] = ajax_command_html('#islandora-oralhistories-annotation-form-wrapper', $new_form);

  drupal_set_message('New annotation created', 'status', FALSE);
  $messages = theme_status_messages(array('display' => 'status'));
  $commands[] = ajax_command_append(NULL, $messages);

  return array('#type' => 'ajax', '#commands' => $commands);
}

//function islandora_oralhistories_annotation_form_submit(array $form, array &$form_state) {
//
//}
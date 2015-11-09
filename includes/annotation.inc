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

  $form['web_annotation_new'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add new annotation'),
    '#attributes' => array('id' => 'add-new-annotation'),
  );
  $form['web_annotation'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="web_annotation_new"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['web_annotation']['start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start time in seconds'),
    '#description' => t('Start time in seconds'),
    '#default_value' => 0.00,
    '#size' => 25,
    '#attributes' => array('class' => array('web-annotation-start')),
  );
  $form['web_annotation']['end'] = array(
    '#type' => 'textfield',
    '#title' => t('End time in seconds'),
    '#description' => t('End time in seconds'),
    '#size' => 25,
    '#attributes' => array('class' => array('web-annotation-end')),
  );
  $form['web_annotation']['annotation_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Annotation content'),
    '#description' => t('Annotation content'),
    '#attributes' => array('class' => array('web-annotation-body')),
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
    '#attributes' => array('class' => array('web-annotation-motivation')),
  );
  $form['web_annotation']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}
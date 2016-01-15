<?php
//function edit_annotation_modal(AbstractObject $object, $ajax) {
//  if (!$object) {
//    $path = current_path();
//    $part_parts = explode('/', $path);
//    $object = islandora_object_load($part_parts[2]);
//  }
//  if ($ajax) {
//    ctools_include('ajax');
//    ctools_include('modal');
//
//    $form_state = array(
//      'ajax' => TRUE,
//      'title' => t('Annotation Modal Form'),
//    );
//    // pass arguments to ctools_modal_form_wrapper().
//    $form_state['build_info']['args'][] = $object;
//
//    // Use ctools to generate ajax instructions for the browser to create
//    // a form in a modal popup.
//    $output = ctools_modal_form_wrapper(
//      'islandora_oralhistories_annotation_form',
//      $form_state
//    );
//
//    if (!empty($form_state['executed'])) {
//
//      // Add the responder javascript, required by ctools
//      ctools_add_js('ajax-responder');
//
//      // Create ajax command array, dismiss the modal window.
//      $output = array();
//      $output[] = ctools_modal_command_dismiss();
//    }
//
//
//    // Return the ajax instructions to the browser via ajax_render().
//    print ajax_render($output);
//    drupal_exit();
//  }
//  else {
//    return drupal_get_form(
//      'islandora_oralhistories_annotation_form',
//      $object
//    );
//  }
//}

function _get_web_annotation_form(AbstractObject $object) {
  $form = drupal_render(drupal_get_form('islandora_oralhistories_annotation_form', $object));
  $javascript = drupal_add_js(NULL, NULL);
  $settings = FALSE;

  if(isset($javascript['settings'])) {
    $settings = '<script type="text/javascript">jQuery.extend(Drupal.settings, ' .
      drupal_json_encode(call_user_func_array('array_merge_recursive', $javascript['settings']['data'])) .
      ');</script>';
  }
  $var1 = isset($_GET['var1']) ? $_GET['var1'] : 'No var1 passed';
  print "<pre>";
  print_r($var1);
  print "</pre>";

  die($form . $settings);
}

/**
 * Menu callback for "islandora/object/%islandora_object/web_annotation".
 *
 * @param \AbstractObject $object
 * @return string
 */
function _retrieve_web_annotations(AbstractObject $object) {
  // @todo: retrieve all annotations for a given $object, return in json.
  // @todo: should
  return 'Annotation list';
}

/**
 * Menu callback for "islandora/object/%islandora_object/web_annotation/create"
 *
 * Our annotationApp will post data to this url.
 * @return string
 */
function _create_web_annotation() {
  // @todo: add a new annotation.

  if (user_access(ISLANDORA_WEB_ANNOTATION_CREATE) && !empty($_POST['content'])) {
    $annotation = array();

    $annotation['author'] = isset($_POST['author']) ? $_POST['author'] : 'anonymous';
    $annotation['content'] = check_plain($_POST['content']);
    $annotation['target_pid'] = isset($_POST['targetPid']) ? $_POST['targetPid'] : '';
    $annotation['mf_start'] = isset($_POST['mediaFragmentStart']) ? floatval($_POST['mediaFragmentStart']) : 0.00;
    $annotation['mf_end'] = isset($_POST['mediaFragmentEnd']) ? floatval($_POST['mediaFragmentEnd']) : 0.00;
    $annotation['annotated_at'] = isset($_POST['annotatedAt']) ? $_POST['annotatedAt'] : getUTCTime();
    $annotation['target_url'] = isset($_POST['targetSource']) ? $_POST['targetSource'] : '';
    $annotation['scope'] = isset($_POST['scope']) ? $_POST['scope'] : '';

    $annoRDF = _createRDF($annotation);
    try {
      islandora_oralhistories_add_annotation($annoRDF, $annotation);
    }
    catch (exception $e) {
      drupal_set_message(t('Failed to add Web annotation'), 'error');
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('Failed to add Web annotation. Error message: @message<br/>Stack: @stack'),
            'message_sub' => array(
              '@message' => $e->getMessage(),
              '@stack' => $e->getTraceAsString(),
            ),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }



    drupal_json_output(array(
      'annotation' => $annoRDF,
      'response' => array(
        'code' => 'success',
        'message' => 'New annotation is created'
      )
    ));

  }
  else {
    drupal_json_output(array(
      'response' => array(
        'code' => 'error',
        'message' => 'No permission to create annotation or annotation content is empty'
      )
    ));
  }

}

/**
 * Menu callback for "islandora/object/%islandora_object/web_annotation/remove/%"
 * @param \AbstractObject $object
 * @param string $anno_object_pid
 * @return string
 */
function _remove_web_annotation(AbstractObject $object, $anno_object_pid = '') {
  // @todo: remove an existing annotation.
  return 'remove an existing annotation.';
}

/**
 * Menu callback for "islandora/object/%islandora_object/web_annotation/remove/%"
 * @param \AbstractObject $object
 * @param string $anno_object_pid
 * @return string
 */
function _update_web_annotation(AbstractObject $object, $anno_object_pid = '') {
  // @todo: update an existing annotation.
  return 'update an existing annotation.';
}


function islandora_oralhistories_annotation_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/datastream');
  $form = array();
  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div id="islandora-oralhistories-annotation-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['web_annotation'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="web-annotation-container-wrapper">',
    '#suffix' => '</div>',
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
      "wrapper" => "islandora-oralhistories-annotation-form-wrapper",
    ),
  );

  return $form;
}

function web_annotation_form_ajax_callback(array $form, array &$form_state) {
  if (form_get_errors()) {
    $form_state['rebuild'] = TRUE;
    $commands = array();
    $commands[] = ajax_command_prepend(NULL, theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
  }
  else {
    $system_message = drupal_get_messages();
    return t('Thank you for your submission!');
  }

}

/**
 * Helper function to get current date time as UTC format.
 * @return bool|string
 */
function getUTCTime() {
  return date("c", time());
}

/**
 * Helper function to generate UUID.
 * @return string valid version 4 UUIDs
 */
function generateUUID() {
  if (function_exists('com_create_guid') === true) {
    return trim(com_create_guid(), '{}');
  }
  return sprintf('%04X%04X-%04X-%04X-%04X-%04X%04X%04X', mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(16384, 20479), mt_rand(32768, 49151), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535));
}

/**
 * Helper function to create web annotation in rdf/xml format.
 * @param $anno
 * @return mixed|null
 */
function _createRDF($anno) {
  module_load_include('php', 'islandora_oralhistories', 'lib/EasyRdf/EasyRdf');
  // use EasyRdf to construct web annotation rdfxml
  EasyRdf_Namespace::set('oa', 'http://www.w3.org/ns/oa#');
  EasyRdf_Namespace::set('dc', 'http://purl.org/dc/elements/1.1/');
  EasyRdf_Namespace::set('dctypes', 'http://purl.org/dc/terms/');
  EasyRdf_Namespace::set('dcterms', 'http://purl.org/dc/dcmitype/');
  EasyRdf_Namespace::set('foaf', 'http://xmlns.com/foaf/0.1/');
  EasyRdf_Namespace::set('rdf', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
  EasyRdf_Namespace::set('rdfs', 'http://www.w3.org/2000/01/rdf-schema#');

  if (!empty($anno)) {
    $graph = new EasyRdf_Graph();
    $annoObj = array(
      'urn' => 'urn:uuid:' . generateUUID(),
      'url' => '',
    );
    $annoBody = array(
      'urn' => 'urn:uuid:' . generateUUID(),
      'url' => '',
    );
    $annoTarget = array(
      'urn' => 'urn:uuid:' . generateUUID(),
      'url' => $anno['target_url'],
    );

    $annoFragment = array(
      'urn' => 'urn:uuid:' . generateUUID(),
      'url' => '',
    );

    $annoObj['graph'] = $graph->resource($annoObj['urn'], 'rdf:Description');
//    $annoObj['graph']->setType('http://www.w3.org/ns/oa#Annotation');
    $annoObj['graph']->set('oa:annotatedBy', $anno['author']['name']);
    $annoObj['graph']->set('oa:annotatedAt', $anno['annotated_at']);
    $annoObj['graph']->set('oa:hasBody', $annoBody['urn']);
    $annoObj['graph']->set('oa:hasTarget', $annoTarget['urn']);
    $graph->add($annoObj['graph'], 'rdf:type', $graph->resource('http://www.w3.org/ns/oa#Annotation'));

    $annoBody['graph'] = $graph->resource($annoBody['urn'], 'rdf:Description');
    $annoBody['graph']->set('rdf:type', 'http://www.w3.org/ns/oa#EmbeddedContent');
    $annoBody['graph']->set('dc:format', 'text/plain');
    $annoBody['graph']->set('rdf:value', $anno['content']);

    $annoTarget['graph'] = $graph->resource($annoTarget['urn'], 'rdf:Description');
    $annoTarget['graph']->set('rdf:type', 'http://www.w3.org/ns/oa#SpecificResource');
    $annoTarget['graph']->set('oa:hasScope', $anno['scope']);
    $annoTarget['graph']->set('oa:hasSelector', $annoFragment['urn']);
    $annoTarget['graph']->set('oa:hasSource', $annoTarget['url']);

    $annoFragment['graph'] = $graph->resource($annoFragment['urn'], 'rdf:Description');
    $annoFragment['graph']->set('rdf:type', 'http://www.w3.org/ns/oa#FragmentSelector');
    $annoFragment['graph']->set('dcterms:conformsTo', 'http://www.w3.org/TR/media-frags/');
    $annoFragment['graph']->set('rdf:value', '#t=' . $anno['mf_start'] . ',' , $anno['mf_end']);
    $annoFragment['graph']->set('oa:hasSource', $annoTarget['url']);

    $format = 'rdfxml';
    return $graph->serialise($format);
  } else {
    return NULL;
  }
}

/**
 * Add a Web annotation CModel object.
 * @param $RDF
 * @param $annoArray
 */
function islandora_oralhistories_add_annotation($RDF, $anno) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  // Get namespace for Web annotation CModel.
  $collection_object = islandora_object_load('islandora:oralhistories_collection');
  $collection_policy = new CollectionPolicy($collection_object['COLLECTION_POLICY']->content);
  $collection_models = $collection_policy->getContentModels();
  if (isset($collection_models['islandora:livingresearchlab_mouseCModel'])) {
    $namespace = check_plain($collection_models['islandora:WADMCModel']['namespace']);
  }
  else {
    $namespace = 'islandora';
  }
  //@todo: create Web annotation CModel object
  //@todo: parse $RDF (rdfxml format)
  $object = $repository->constructObject($namespace);
  $annotation = islandora_oralhistories_parse_RDF($RDF);

}

function islandora_oralhistories_parse_RDF($RDF) {
  $xml = simplexml_load_string($RDF);
//  drupal_goto("islandora/object/islandora%3A363/web_annotation/add", array('query' => array('var1' => $xml)));
  return $xml;
}
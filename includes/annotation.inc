<?php
define('ISLANDORA_ORALHISTORIES_NAMESPACE_DC', 'http://purl.org/dc/elements/1.1/');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_DCTERMS', 'http://purl.org/dc/terms/');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_DCTYPES', 'http://purl.org/dc/dcmitype/');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_OA', 'http://www.w3.org/ns/oa#');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_FOAF', 'http://xmlns.com/foaf/0.1/');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_PROV', 'http://www.w3.org/ns/prov#');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_RDF', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_RDFS', 'http://www.w3.org/2000/01/rdf-schema#');
define('ISLANDORA_ORALHISTORIES_NAMESPACE_SKOS', 'http://www.w3.org/2004/02/skos/core#');


/**
 * Menu callback for "islandora/object/%islandora_object/web_annotation".
 *
 * @param \AbstractObject $object
 * @return string
 */
function _retrieve_web_annotations(AbstractObject $object) {
  // @todo: retrieve all annotations for a given $object, return in json.
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


    try {
      islandora_oralhistories_add_annotation($annotation);
    }
    catch (exception $e) {
      drupal_set_message(t('Failed to add Web annotation'), 'error');
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('Failed to add Web annotation. (line 73) Error message: @message<br/>Stack: @stack'),
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

    // This is to test RDF/XML is generated correctly.
    drupal_json_output(array(
      'annotation' => $annotation,
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



/**
 * Helper function to get current date time as UTC format.
 * @return bool|string
 */
function getUTCTime() {
  return gmdate("Y-m-d\TH:i:s\Z", time());
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
 *
 * @param array $anno
 *   Array contains POST data
 * @return mixed|null
 */
function _createRDF($anno) {
  module_load_include('inc', 'islandora_oralhistories', 'includes/rdf_builder');
  $children = array();
  $rdf = new IslandoraRdfBuilder();
  if (!empty($anno)) {
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
    // Add Annotation Object
    $children = array(
       array(
         'name' => 'rdf:type',
         'attributes' => array('rdf:resource' => 'http://www.w3.org/ns/oa#Annotation'),
         'value' => ''
       ),
      array(
        'name' => 'oa:hasBody',
        'attributes' => array('rdf:resource' => $annoBody['urn']),
        'value' => ''
      ),
      array(
        'name' => 'oa:hasTarget',
        'attributes' => array('rdf:resource' => $annoTarget['urn']),
        'value' => ''
      ),
      array(
        'name' => 'oa:annotatedAt',
        'value' => $anno['annotated_at']
      ),
      array(
        'name' => 'oa:annotatedBy',
        'value' => $anno['author']['name']
      ),
      array(
        'name' => 'oa:serializedAt',
        'value' => getUTCTime()
      ),
    );
    $rdf->addDescriptionNode($annoObj['urn'], $children);

    // Add Annotation body
    $children = array(
      array(
        'name' => 'rdf:type',
        'attributes' => array('rdf:resource' => 'http://www.w3.org/ns/oa#EmbeddedContent'),
        'value' => ''
      ),
      array(
        'name' => 'dc:format',
        'value' => 'text/plain',
      ),
      array(
        'name' => 'rdf:value',
        'value' => $anno['content'],
      ),
    );
    $rdf->addDescriptionNode($annoBody['urn'], $children);

    // Add Annotation target
    $children = array(
      array(
        'name' => 'rdf:type',
        'attributes' => array('rdf:resource' => 'http://www.w3.org/ns/oa#SpecificResource'),
        'value' => ''
      ),
      array(
        'name' => 'oa:hasSource',
        'attributes' => array('rdf:resource' => $annoTarget['url']),
        'value' => ''
      ),
      array(
        'name' => 'oa:hasScope',
        'attributes' => array('rdf:resource' => $anno['scope']),
        'value' => ''
      ),
      array(
        'name' => 'oa:hasSelector',
        'attributes' => array('rdf:resource' => $annoFragment['urn']),
        'value' => ''
      ),
    );
    $rdf->addDescriptionNode($annoTarget['urn'], $children);

    // Add FragmentSelector for Target
    $children = array(
      array(
        'name' => 'rdf:type',
        'attributes' => array('rdf:resource' => 'http://www.w3.org/ns/oa#FragmentSelector'),
        'value' => ''
      ),
      array(
        'name' => 'dcterms:conformsTo',
        'attributes' => array('rdf:resource' => 'http://www.w3.org/TR/media-frags/'),
        'value' => ''
      ),
      array(
        'name' => 'rdf:value',
        'value' => '#t=' . $anno['mf_start'] . ',' . $anno['mf_end']
      ),
    );
    $rdf->addDescriptionNode($annoFragment['urn'], $children);

    return $rdf->printRdf();

  }
}

/**
 * Add a Web annotation CModel object.
 * @param array $annotation
 */
function islandora_oralhistories_add_annotation($annotation) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  // Get namespace for Web annotation CModel.
  $collection_object = islandora_object_load('islandora:oralhistories_collection');
  $collection_policy = new CollectionPolicy($collection_object['COLLECTION_POLICY']->content);
  $collection_models = $collection_policy->getContentModels();
  if (isset($collection_models['islandora:WADMCModel'])) {
    $namespace = check_plain($collection_models['islandora:WADMCModel']['namespace']);
  }
  else {
    $namespace = 'islandora';
  }

  if (isset($annotation) && !empty($annotation)) {
    $RDF = _createRDF($annotation);
    try {
      $object = $repository->constructObject($namespace);
      $object->label = islandora_oralhistories_shorten_title($annotation['content'], 30);
      $object->models = array('islandora:WADMCModel');
      $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'islandora:oralhistories_collection');
      $object->relationships->add(FEDORA_RELS_EXT_URI, 'isAnnotationOf', $annotation['target_pid']);

      $dsid = 'WADM';
      $ds = $object->constructDatastream($dsid, 'M');
      $ds->label = $dsid;
      $ds->mimetype = 'text/xml';
      if (isset($RDF)) {
        $ds->setContentFromString($RDF);
      }
      $object->ingestDatastream($ds);
      $repository->ingestObject($object);
      watchdog('Islandora OralHistories', 'Add new annotation: %t', array('%t' => $object->id), WATCHDOG_INFO);
    }
    catch (Exception $e) {
      watchdog('Islandora OralHistories', 'Error adding annotation object: %t', array('%t' => $e->getMessage()), WATCHDOG_ERROR);
    }

  } else {
    // Throw error
    drupal_set_message(t('Empty RDF or annotation passed'), 'error', FALSE);
    watchdog('Islandora OralHistories', 'Empty annotation passed', WATCHDOG_ERROR);
  }
}

function islandora_oralhistories_parse_RDF($rdfxml) {
  module_load_include('inc', 'islandora_oralhistories', 'includes/rdf_builder');
  $annotation = array();

  try {
    $dom = new DOMDocument('1.0', 'utf-8');
    $dom->loadXML($rdfxml);
    $rdf = new IslandoraRdfBuilder($dom);
    $annotation['urn'] = $rdf->getAnnotationUrn();
    $annotation['target']['urn'] = $rdf->getAnnotationTargetUrn($annotation['urn']);
    $annotation['target']['scope'] = $rdf->getAnnotationScopeUrn($annotation['target']['urn'][0]);
    $annotation['target']['source'] = $rdf->getAnnotationSourceUrn($annotation['target']['urn'][0]);
    $annotation['fragment']['urn'] = $rdf->getAnnotationTargetSelectorUrn($annotation['target']['urn'][0]);
    $annotation['fragment']['value'] = $rdf->getAnnotationTargetSelectorFrag($annotation['fragment']['urn']);
    $annotation['body']['urn'] = $rdf->getAnnotationBodyUrn($annotation['urn']);
    $annotation['body']['value'] = $rdf->getAnnotationBodyContent($annotation['body']['urn'][0]);
  }
  catch (Exception $e) {
    drupal_set_message(t('Error parsing annotation rdfxml: %t', array('%t' => $e->getMessage())), 'error', FALSE);
    watchdog('Islandora OralHistories', 'Error parsing annotation rdfxml: %t', array('%t' => $e->getMessage()), WATCHDOG_ERROR);
  }
  return $annotation;
}

/**
 * Helper function to extract the first x number of characters from a given string.
 *
 * @param $title string
 * @param $length integer
 * @return string
 */
function islandora_oralhistories_shorten_title($title, $length) {
  if (strlen($title) > $length) {
    $title = substr($title, 0, strpos($title, ' ', $length));
  }
  return $title;
}
<?php

/**
 * @file
 * Build RDF like XML for Web Annotation standard.
 *
 * Modified and extended from Islandora Image Annotation Solution Pack.
 *
 * Convenience class to build RDF objects.
 */

/**
 * IslandoraRdfBuilder
 *
 * Islandora's wrapper to interact with
 * RDF data.
 * @author discoverygarden
 */
class IslandoraRdfBuilder {

  public $domDoc = NULL;

  /**
   * Constructor for IslandoraRdfBuilder class.
   *
   * @param XML $dom_doc
   *   The DOM document.
   */
  public function __construct($dom_doc = NULL) {
    if (!isset($dom_doc)) {
      $this->domDoc = new DOMDocument('1.0', 'utf-8');
      $this->domDoc->formatOutput = TRUE;
      $this->root = $this->domDoc->appendChild($this->domDoc->createElement('rdf:RDF'));
      $this->root->setAttribute('xmlns:dc', "http://purl.org/dc/elements/1.1/");
      $this->root->setAttribute('xmlns:dcterms', "http://purl.org/dc/terms/");
      $this->root->setAttribute('xmlns:dctypes', "http://purl.org/dc/dcmitype/");
      $this->root->setAttribute('xmlns:foaf', "http://xmlns.com/foaf/0.1/");
      $this->root->setAttribute('xmlns:oa', "http://www.w3.org/ns/oa#");
      $this->root->setAttribute('xmlns:prov', "http://www.w3.org/ns/prov#");
      $this->root->setAttribute('xmlns:rdf', "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
      $this->root->setAttribute('xmlns:rdfs', "http://www.w3.org/2000/01/rdf-schema#");
      $this->root->setAttribute('xmlns:skos', "http://www.w3.org/2004/02/skos/core#");
    }
    else {
      $this->domDoc = $dom_doc;
      $this->root = $this->domDoc->documentElement;
    }
  }

  /**
   * Returns value of IslandoraRdfBuilder object as an xml string.
   *
   * @return XML
   *   The XML we construct.
   */
  public function printRdf() {
    return $this->domDoc->saveXML();
  }

  /**
   * Add description node to existing RDF XML.
   *
   * @param string $about
   *   The resource identified in the RDF 'about' attribute.
   * @param array $children
   *   A structured array of child elements to this node.
   *
   *   Each array entry is a sub array with a manditory name, an optional array
   *   of attributes, and an optional value.
   */
  public function addDescriptionNode($about, $children = NULL) {
    $description = $this->domDoc->createElement('rdf:Description');
    $description = $this->root->appendChild($description);
    $description->setAttribute("rdf:about", $about);

    foreach ($children as $child) {
      $new = $this->domDoc->createElement($child['name']);
      $description->appendChild($new);
      if (isset($child['value'])) {
        $value = $this->domDoc->createTextNode($child['value']);
        $new->appendChild($value);
      }
      if (array_key_exists('attributes', $child)) {
        foreach ($child['attributes'] as $key => $att_val) {
          $new->setAttribute($key, $att_val);
        }
      }
    }
  }

  /**
   * Takes NodeList of Description from external RDF, adds to object's root DOM.
   *
   * @param NodeList $node_list
   *   A given NodeList containing RDF descriptions.
   */
  public function addDescriptionsFromNodelist($node_list) {
    foreach ($node_list as $dom_element) {
      $dom_node = $this->domDoc->importNode($dom_element, TRUE);
      $this->root->appendChild($dom_node);
    }
  }

  /**
   * Takes a Description element from external RDF, adds to object's root DOM.
   *
   * @param DOM $dom_element
   *   A given DOM document containing an RDF description element.
   */
  public function addDescriptionElement($dom_element) {
    $dom_node = $this->domDoc->importNode($dom_element, TRUE);
    $this->root->appendChild($dom_node);
  }

  /**
   * Sets the XPATH for the IslandoraRdfBuilder object.
   */
  public function setXPath() {
    $this->xpath = new DOMXPath($this->domDoc);
    $this->xpath->registerNamespace('rdf', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
    $this->xpath->registerNamespace('oa', 'http://www.w3.org/ns/oa#');
    $this->xpath->registerNamespace('dc', 'http://purl.org/dc/elements/1.1/');
    $this->xpath->registerNamespace('dcterms', 'http://purl.org/dc/terms/');
    $this->xpath->registerNamespace('dctypes', 'http://purl.org/dc/dcmitype/');
    $this->xpath->registerNamespace('foaf', 'http://xmlns.com/foaf/0.1/');
    $this->xpath->registerNamespace('prov', 'http://www.w3.org/ns/prov#');
    $this->xpath->registerNamespace('rdfs', 'http://www.w3.org/2000/01/rdf-schema#');
    $this->xpath->registerNamespace('skos', 'http://www.w3.org/2004/02/skos/core#');
  }

  /**
   * Deletes an RDF description element from the DOM.
   *
   * @param string $node_id
   *   The node id we are searching to remove from.
   */
  public function deleteDescriptionElement($node_id) {
    $xpath = new DOMXPath($this->domDoc);
    $candidates = $xpath->query("//rdf:Description[@rdf:about='$node_id']");
    foreach ($candidates as $candidate) {
      $candidate->parentNode->removeChild($candidate);
    }
  }

  /**
   * Convenience method to structure input arrays for addDescriptionNode.
   *
   * @param string $name
   *   Name of child tag.
   * @param array $attributes
   *   Array of key => value pairs (optional).
   * @param string $value
   *   Value for tag (optional).
   *
   * @return array
   *   A structured array used in addDescriptionNode.
   */
  public function buildChild($name, $attributes = NULL, $value = NULL) {
    $return_array = array();
    $return_array['name'] = $name;
    if ($value) {
      $return_array['value'] = $value;
    }
    if ($attributes) {
      foreach ($attributes as $key => $att_val) {
        $return_array['attributes'][$key] = $att_val;
      }
    }
    return $return_array;
  }

 /**
 * Retrieves the uuid of the node itself.
 *
 * @return string
 *   A uuid containing urn:uuid:. This is the uuid of the node.
 */
  public function getAnnotationUrn() {
    if (empty($this->xpath)) {
      $this->setXPath();
    }
    $candidates = $this->xpath->query('//rdf:Description');
    return $candidates->item(0)->getAttribute('rdf:about');
  }

  /**
   * Retrieves the uuid of the hasScope itself.
   *
   * * @param string $id
   *   The target uuid containing urn:uuid:.
   *
   * @return string
   *   A uuid containing urn:uuid:. This is the uuid of hasScope node.
   */
  public function getAnnotationScopeUrn($id) {
    if (empty($this->xpath)) {
      $this->setXPath();
    }
    $candidates = $this->xpath->query('//rdf:Description[@rdf:about="' . $id . '"]/oa:hasScope');
    foreach ($candidates as $candidate) {
      $resource[] = $candidate->getAttribute('rdf:resource');
    }
    return $resource;
  }

  /**
   * Retrieves the uuid of the hasBody node.
   *
   * @param string $id
   *   The uuid containing urn:uuid:.
   *
   * @return string
   *   A uuid containing urn:uuid:. This is the uuid of the hasBody node.
   */
  public function getAnnotationBodyUrn($id) {
    if (empty($this->xpath)) {
      $this->setXPath();
    }
    $candidates = $this->xpath->query('//rdf:Description[@rdf:about="' . $id . '"]/oa:hasBody');
    foreach ($candidates as $candidate) {
      $resource[] = $candidate->getAttribute('rdf:resource');
    }
    return $resource;
  }

  /**
 * Retrieves the uuid of the hasTarget node.
 *
 * @param string $id
 *   The uuid containing urn:uuid:.
 *
 * @return string
 *   A uuid containing urn:uuid:. This is the uuid of the hasTarget node.
 */
  public function getAnnotationTargetUrn($id) {
    if (empty($this->xpath)) {
      $this->setXPath();
    }
    $candidates = $this->xpath->query('//rdf:Description[@rdf:about="' . $id . '"]/oa:hasTarget');
    foreach ($candidates as $candidate) {
      $resource[] = $candidate->getAttribute('rdf:resource');
    }
    return $resource;
  }


}
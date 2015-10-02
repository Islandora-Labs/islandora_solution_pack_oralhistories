<?php

/**
 * @file
 * This is the template file for the object page for oralhistories
 *
 * Available variables:
 * - $islandora_object: The Islandora object rendered in this template file
 * - $islandora_dublin_core: The DC datastream object
 * - $dc_array: The DC datastream object values as a sanitized array. This
 *   includes label, value and class name.
 * - $islandora_object_label: The sanitized object label.
 * - $parent_collections: An array containing parent collection(s) info.
 *   Includes collection object, label, url and rendered link.
 *
 * @see template_preprocess_islandora_oralhistories()
 * @see theme_islandora_oralhistories()
 */
?>

<div class="islandora-oralhistories-object islandora" vocab="http://schema.org/" prefix="dcterms: http://purl.org/dc/terms/" typeof="VideoObject">
  <div class="islandora-oralhistories-content-wrapper  clearfix">
    <?php if ($islandora_content['viewer']): ?>
      <div class="islandora-oralhistories-content">
        <?php print $islandora_content['viewer']; ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="islandora-oralhistories-metadata">
    <?php print $description; ?>
    <?php if ($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
        <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
    <?php print $metadata; ?>
  </div>
</div>

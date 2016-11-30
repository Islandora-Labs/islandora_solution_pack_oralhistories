<?php
/**
 * @file
 *   Exposed Hooks
 */

/**
 * Retrieve transcript given some UI settings. If a term
 * has been searched for, then return highlights as well.
 *
 * This hook is invoked if you create a TranscriptUI object
 * by calling transcripts_ui_ui() with your module name as
 * the first argument.
 *
 * An array of time code units and highlights is returned.
 * Time code units should be an array of objects, each of
 * which must have the following properties:
 *
 *   $obj->id : unique identifier
 *   $obj->speaker : speaker name
 *   $obj->start : start time in seconds
 *   $obj->end : end time in seconds
 *
 * In addition, there should be a property for each tier.
 * Assuming your tiers are Esperanto, Quenya, and Sindarin:
 *
 *   $obj->ts_content_epo
 *   $obj->ts_content_qya
 *   $obj->ts_content_sjn
 *
 * Highlights are returned in Solr format, with snippets
 * keyed by id to a time code unit. Return NULL if there
 * hasn't been a term search or there are no highlights.
 *
 * @param TranscriptUI $ui
 *   A transcript ui containing relevant settings.
 *
 * @return array($tcus, $highlights)
 *
 */
function hook_transcripts_ui_transcript($ui)
{
    $response = solr_query($ui);

    $tcus = $response->response->docs;
    $highlights = isset($response->highlighting) ? $response->highlighting : NULL;

    return array($tcus, $highlights);
}

function hook_transcripts_ui_js_alter(&$scripts)
{
    $scripts['ui'] = drupal_get_path('module', 'my_module') . '/js/my_ui.js';
    $scripts['scroller'] = drupal_get_path('module', 'my_module') . '/js/my_scroller.js';
}
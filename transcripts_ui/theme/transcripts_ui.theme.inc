<?php

function theme_transcripts_ui_transcript_controls($vars)
{
    $out = drupal_render($vars['element']['content']);
    return $out;
}

function theme_transcripts_ui_transcript_options($vars)
{

    $out  = "<select multiple class='selectpicker tier-selector' data-header='Select languages'>";

    //language selector
    $out .= "<optgroup label='Transcript' data-type='languages'>";

    foreach ($vars['element']['data_tiers'] as $key => $val) {
        $out .= "<option value='{$key}'>{$val}</option>";
    }
    $out .= "</optgroup>";

    //speaker name selector
    $out .= "<optgroup label='Toggle speaker names on/off' data-type='speakers'>";
    foreach ($vars['element']['speaker_names'] as $key => $val) {
        $out .= "<option value='{$key}'>{$val}</option>";
    }
    $out .= "</optgroup>";

    $out .= "</select>";
    return $out;
}

function theme_transcripts_ui_transcript_navigation($vars)
{
    $out = "<div class='btn-group' role='group'>";
    $out .= "<button type='button' class='btn btn-default previous' title='Previous line'><span class='glyphicon glyphicon-arrow-left'></span> " . t('Prev') . "</button>";
    $out .= "<button type='button' class='btn btn-default sameagain' title='Same line'><span class='glyphicon glyphicon-repeat'></span>" . t('Same') . "</button>";
    $out .= "<button type='button' class='btn btn-default next' title='Next line'><span class='glyphicon glyphicon-arrow-right'></span>" . t('Next') . "</button>";
    $out .= "</div>";
    return $out;
}

function theme_transcripts_ui_transcript_search($vars)
{
    $out = drupal_render($vars['element']['search_form']);
    return $out;
}

function theme_transcripts_ui_speaker_name($vars)
{
    $vars['classes'][] = 'speaker-display';
    $vars['classes'][] = 'active';
    $classes = implode(' ', $vars['classes']);
    $out  = "<div class='speaker-name {$vars['speaker_turn']}'>";
    if (is_array($vars['speaker_name'])) {
        foreach ($vars['speaker_name'] as $key => $val) {
            $out .= "<span data-speaker-display='$key' class='$classes'>$val</span> ";
        }
    }
    $out .= "</div>";
    return $out;
}

function theme_transcripts_ui_tcu_tier($vars)
{
    $vars['classes'][] = 'tier';
    $vars['classes'][] = 'active';
    $classes = implode(' ', $vars['classes']);
    $out = "<div data-tier='{$vars['tier_name']}' class='$classes'>{$vars['tier_text']}</div>";
    return $out;
}

function theme_transcripts_ui_play_tcu($vars)
{
    $mins = floor($vars['element']['#time'] / 60);
    $secs = $vars['element']['#time'] % 60;
    $time = sprintf("%d:%02d", $mins, $secs);
    $classes = 'btn btn-default btn-icon play-tcu';
    $classes .= $vars['element']['#timecoded'] ? ' timed' : ' untimed';
    $out = "<button type='button' class='" .$classes ."' title='Play line'><span class='glyphicon glyphicon-play'></span> ";
    $out .= $time;
    if (isset($vars['element']['#text'])) {
        $out .= " " . $vars['element']['#text'];
    }
    $out .= "</button>";
    return $out;
}

<div class="col-sm-12 col-md-12">
  <video id="islandora-oralhistories-media-player" class="media-default-skin" controls
       preload="auto"  width="640" height="264" poster="<?php print $tn; ?>" data-setup="{}">
  <?php foreach ($sources as $source): ?>
    <source src="<?php print $source['url']; ?>" type='<?php print $source['mime']; ?>'>
  <?php endforeach; ?>

  <?php if (isset($tracks)): ?>
    <?php foreach ($tracks as $key => $track): ?>
      <?php if ($key == 0): ?>
        <?php if ($enable_caption_display): ?>
          <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
             kind="captions" label="<?php print $track['lang_code']; ?>" <?php print $track['default']; ?>>
        <?php else: ?>
          <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
             kind="metadata" label="<?php print $track['lang_code']; ?>" <?php print $track['default']; ?>>
        <?php endif; ?>
      <?php else: ?>
        <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
          kind="subtitles" label="<?php print $track['lang_code']; ?>">
      <?php endif; ?>
    <?php endforeach; ?>
  <?php endif; ?>
  <p> HTML5 Video/Audio is not supported by this browser.</p>
  </video>
</div>
<!--<div class="col-sm-12 col-md-12">-->
<!--  --><?php //if ($transcript_content): ?>
<!--    <fieldset id="transcript-display" class="collapsible clearfix">-->
<!--      <legend><span class="fieldset-legend">Transcript</span></legend>-->
<!--      <div class="fieldset-wrapper">-->
<!--        <div id="transcript-content">-->
<!--          --><?php //print $transcript_content; ?>
<!--        </div>-->
<!--      </div>-->
<!--    </fieldset>-->
<!--  --><?php //endif; ?>
<!--</div>-->
<div class="col-sm-12 col-md-12" id="transcript-content">
  <?php if ($transcript_content): ?>
          <?php print $transcript_content; ?>
  <?php endif; ?>
</div>

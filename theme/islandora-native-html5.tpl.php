<video id="islandora-oralhistories-media-player" class="media-default-skin" controls
     preload="auto"  width="640" height="264" poster="<?php print $tn; ?>" data-setup="{}">
<?php foreach ($sources as $source): ?>
  <source src="<?php print $source['url']; ?>" type='<?php print $source['mime']; ?>'>
<?php endforeach; ?>

<?php foreach ($tracks as $key => $track): ?>
  <?php if ($key == 0): ?>
    <?php if ($enable_caption_display): ?>
      <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
         kind="captions" label="<?php print $track['lang_code']; ?>" default>
    <?php else: ?>
      <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
         kind="metadata" label="<?php print $track['lang_code']; ?>" default>
    <?php endif; ?>
  <?php else: ?>
    <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
      kind="subtitles" label="<?php print $track['lang_code']; ?>">
  <?php endif; ?>
<?php endforeach; ?>
<p> HTML5 Video/Audio is not supported by this browser.</p>
</video>

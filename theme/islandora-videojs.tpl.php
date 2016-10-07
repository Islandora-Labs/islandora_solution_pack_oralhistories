<video id="islandora_videojs" class="video-js vjs-default-skin" controls
       preload="auto" width="640" height="264" poster="<?php print $tn; ?>"
       data-setup="{}">
  <?php foreach ($sources as $source): ?>
    <source src="<?php print $source['url']; ?>" type='<?php print $source['mime']; ?>'>
  <?php endforeach; ?>

<?php if (isset($tracks)): ?>
  <?php foreach ($tracks as $key => $track): ?>
    <?php if ($key == 0): ?>
      <?php if ($enable_caption_display): ?>
        <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
           kind="captions" label="<?php print $track['lang_code']; ?>" <?php print $track['default']; ?>>
      <?php else: ?> <!--Currently video.js throw errors when kind="metadata"-->
        <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
           kind="captions" label="<?php print $track['lang_code']; ?>" <?php print $track['default']; ?>>
      <?php endif; ?>
    <?php else: ?>
      <track id="track<?php print $key; ?>" src="<?php print $track['source_url']; ?>" srclang="<?php print $track['lang_code']; ?>"
        kind="subtitles" label="<?php print $track['lang_code']; ?>">
    <?php endif; ?>
  <?php endforeach; ?>
<?php endif; ?>
  <p> HTML5 Video/Audio is not supported by this browser.</p>
</video>
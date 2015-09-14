<?php
  unset($transcript_controls['content']['transcript_search']);
  print render($transcript_controls);
?>
<div class="transcript-container">
  <div class="transcript-content" id="transcript">
    <?php print render($transcript); ?>
  </div>
</div>

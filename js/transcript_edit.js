/**
 * @file
 * Handles toggle cues behavior.
 */

var g_Collapse = true;

(function($){
    Drupal.behaviors.islandoraOralHistories = {

        attach: function (context, settings) {
            // Position the button.
            jQuery('#edit-toggle-cues').css({'float':'right'});
            jQuery('#edit-toggle-cues').after("<br clear='all'/>");

            // Toggle button clicked.
            jQuery('#edit-toggle-cues').on('click', function(evt) {
                if (g_Collapse == true) {
                    g_Collapse = false;
                } else {
                    g_Collapse = true;
                }

                // Get top position, used to reposition after expand.
                var originalTopPosition = jQuery('#edit-toggle-cues').position().top;
                var oModalContent = jQuery("#modalContent");
                if (oModalContent.length > 0) {
                    originalTopPosition = jQuery('#modalContent').position().top;
                }

                // Loop through each fieldset element and mimic click.
                jQuery( ".fieldset-title" ).each(function( index ) {
                    var isVisible = jQuery(this).closest("fieldset").find("div").is(":visible");

                    // Expand
                    if (g_Collapse == false) {
                        if(isVisible === false) {
                            $(this).click();
                        }
                    // Collapse
                    } else {
                        if(isVisible === true) {
                            $(this).click();
                        }
                    }
                });

                // Reposition the window to the top.
                if (g_Collapse == false) {
                    setTimeout(function() {
                        document.body.scrollTop = document.documentElement.scrollTop = originalTopPosition;
                    }, 275);
                }
            });
        } // end attach function
    };

})(jQuery);



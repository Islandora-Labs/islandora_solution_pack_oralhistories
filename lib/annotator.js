/**
 * Created by ljiang on 2015-10-26.
 */
(function() {
    'use strict';

    // Create the button
    videojs.Annotator = videojs.Button.extend({
        init: function( player, options ) {
            // Initialize the button using the default constructor
            videojs.Button.call( this, player, options );

        }
    });

    // Set the text for the button
    videojs.Annotator.prototype.buttonText = 'Annotate Video';

    // These are the defaults for this class.
    videojs.Annotator.prototype.options_ = {};

    // videojs.Button uses this function to build the class name.
    videojs.Annotator.prototype.buildCSSClass = function() {
        // Add our className to the returned className
        return 'vjs-annotate-button ' + videojs.Button.prototype.buildCSSClass.call(this);
    };

    // videojs.Button already sets up the onclick event handler, we just need to overwrite the callback
    videojs.Annotator.prototype.onClick = function( e ) {
        // We need to stop this event before it bubbles up to "window" for our event listener below.
        e.stopImmediatePropagation();

        console.log("Annotate button is clicked!");
    };



    // This function will be called by video.js when it loops through all of the registered plugins.
    var pluginFn = function( options ) {
        // We need to pass off the options to the control bar button.
        var annotateComponet = new videojs.Annotator( this, options );

        // Set the default position for the sharing button. Default: control-bar
        var onScreen = options.onScreen || false;
        // Now we remove the onScreen option as it does not pertain to anything inside the button.
        delete options.onScreen;

        var annotateButton;

        // Should the button be added to the control bar or screen?
        if ( onScreen ) {
            annotateButton = this.addChild( annotateComponet );
        } else {
            annotateButton = this.controlBar.addChild( annotateComponet );
        }


    };

    videojs.plugin( 'annotator', pluginFn );

})();
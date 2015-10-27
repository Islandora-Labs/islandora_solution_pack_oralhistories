/**
 * Created by ljiang on 2015-10-26.
 */
(function() {
    'use strict';

    videojs.Annotator = videojs.Button.extend({
        init: function( player, options ) {
            videojs.Button.call( this, player, options );

        }
    });

    videojs.Annotator.prototype.buttonText = 'Annotate Video';

    videojs.Annotator.prototype.options_ = {};
    videojs.Annotator.prototype.buildCSSClass = function() {
        return 'vjs-annotate-button ' + videojs.Button.prototype.buildCSSClass.call(this);
    };

    videojs.Annotator.prototype.onClick = function( e ) {
        // We need to stop this event before it bubbles up to "window" for our event listener below.
        e.stopImmediatePropagation();

        console.log("Annotate button is clicked!");
    };

    var pluginFn = function( options ) {
        var annotateComponet = new videojs.Annotator( this, options );
        var annotateButton;
        annotateButton = this.controlBar.addChild( annotateComponet );
    };
    videojs.plugin( 'annotator', pluginFn );
})();
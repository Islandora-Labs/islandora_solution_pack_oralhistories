/**
 * Created by ljiang on 2015-10-26.
 */
(function() {
    'use strict';
    var annotationURL = '';

    videojs.Annotator = videojs.Button.extend({
        init: function( player, options ) {
            videojs.Button.call( this, player, options );

        }
    });
    videojs.Annotator.prototype.options_ = { };

    videojs.Annotator.prototype.createEl = function( tagName, props ) {
        // the new button
        console.log(this, 'this');
        if (this.options_.targetUrl) {
            annotationURL = this.options_.targetUrl + '/web_annotation/nojs';
        }
        var annotateBtn = videojs.createEl( 'div', {
            className: 'vjs-annotate-button ' + videojs.Button.prototype.buildCSSClass.call(this),
            innerHTML: '<a href="' + annotationURL + '"></a>'
        });

        return annotateBtn;
    };

    videojs.Annotator.prototype.buttonText = 'Annotate Video';

    videojs.Annotator.prototype.onClick = function( e ) {
        // We need to stop this event before it bubbles up to "window" for our event listener below.
        e.stopImmediatePropagation();

        console.log(e, 'e');
        return true;
    };

    var pluginFn = function( options ) {
        var annotateComponet = new videojs.Annotator( this, options );
        var annotateButton;
        annotateButton = this.controlBar.addChild( annotateComponet );
    };
    videojs.plugin( 'annotator', pluginFn );
})();
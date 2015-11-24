/**
 * @file
 * Annotation App js file
 */

(function ($) {
    Drupal.behaviors.annotationApp = {
        attach: function (context, settings) {
            console.dir(Drupal.settings.islandoraOralhistories);

            var AnnotationBox = React.createClass({
                displayName: "AnnotationBox",

                render: function () {
                    return React.createElement(
                        "div",
                        { className: "annotationBox" },
                        React.createElement(
                            "p",
                            null,
                            "this is new anntation box from reactjs."
                        )
                    );
                }
            });

            ReactDOM.render(React.createElement(AnnotationBox, { url: "/api/comments", pollInterval: 2000 }), document.getElementById('annotation-tab'));
        } // End of attach

    };
})(jQuery);
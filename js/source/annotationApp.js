/**
 * @file
 * Annotation App js file
 */

(function($){
    Drupal.behaviors.annotationApp = {
        attach: function (context, settings) {
            console.dir(Drupal.settings.islandoraOralhistories);

            var AnnotationBox = React.createClass({
                render: function() {
                    return (
                        <div className="annotationBox">
                            <p>this is new anntation box from reactjs.</p>
                        </div>
                    );
                }
            });


            ReactDOM.render(
                <AnnotationBox url="/api/comments" pollInterval={2000} />,
                document.getElementById('annotation-tab')
            );


        } // End of attach

    };
})(jQuery);

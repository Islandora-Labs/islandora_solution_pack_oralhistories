/**
 * @file
 * Annotation App js file
 */

(function($){
    Drupal.behaviors.annotationApp = {
        attach: function (context, settings) {
            //console.dir(Drupal.settings.islandoraOralhistories);
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;
            var enableAnnotation = Drupal.settings.islandoraOralhistories.enableAnnotationTabDisplay;
            var videoElement = $(".islandora-oralhistories-object").find('video, audio')[0];
            var apiUrl = '/islandora/object/' + targetObjectId + '/web_annotation/create';
            var user = Drupal.settings.islandoraOralhistories.user;
            var permissions = Drupal.settings.islandoraOralhistories.permissions;
            console.log(permissions);


            //AnnotationBox
            var AnnotationBox = React.createClass({
                loadAnnotationsFromServer: function() {
                    $.ajax({
                        url: this.props.url,
                        dataType: 'json',
                        cache: false,
                        success: function(data) {
                            this.setState({data: data});
                        }.bind(this),
                        error: function(xhr, status, err) {
                            console.error(this.props.url, status, err.toString());
                        }.bind(this)
                    });
                },
                handleAnnotationSubmit: function(annotation) {
                    var arr = this.state.annotations;
                    $.ajax({
                        url: this.props.url,
                        dataType: 'json',
                        type: 'POST',
                        data: annotation,
                        success: function(data) {
                            if (data == 'success') {
                                arr.push(annotation);
                                this.setState({annotations: arr});
                                this.setState({adding: false}); // Hide the form after submission
                            }
                            console.dir(data);

                        }.bind(this),
                        error: function(xhr, status, err) {
                            console.error(this.props.url, status, err.toString());
                        }.bind(this)
                    });
                },
                getInitialState: function() {
                    return {
                        annotations: []
                    };
                },
                componentDidMount: function() {
                    //this.loadAnnotationsFromServer();
                    //setInterval(this.loadAnnotationsFromServer, this.props.pollInterval);
                },
                addNewForm: function() {
                    this.setState({adding: true});
                },
                renderDisplay: function() {
                    //console.dir(this.state.data, 'data from annotationbox');
                    if (!this.state.adding) {
                        return (
                            <div className="annotationBox">
                                <div className="annotationToolbars">
                                    <button onClick={this.addNewForm}
                                            className="btn btn-success btn-sm glyphicon glyphicon-plus"> {Drupal.t('New Annotation')}</button>
                                </div>
                                <AnnotationList data={this.state.annotations} />
                            </div>
                        );
                    } else {
                        return (
                            <div className="annotationBox">
                                <div className="annotationToolbars">
                                    <button onClick={this.addNewForm}
                                            className="btn btn-success btn-sm glyphicon glyphicon-plus"> {Drupal.t('New Annotation')}</button>
                                </div>
                                <AnnotationForm onAnnotationSubmit={this.handleAnnotationSubmit}/>
                                <AnnotationList data={this.state.annotations} />
                            </div>
                        );
                    }
                },
                render: function() {
                    return this.renderDisplay();
                }
            });

            //AnnotationList
            var AnnotationList = React.createClass({
                nextIndex: function() {
                    this.uniqueId = this.uniqueId || 0;
                    return this.uniqueId++;
                },
                render: function() {
                    //console.dir(this.props.data);
                    var annotationItems = this.props.data.map(function(item, index) {
                        return (
                            <Annotation author={item.author} key={index} start={item.mediaFragmentStart} end={item.mediaFragmentEnd}>
                                {item.content}
                            </Annotation>
                        );
                    });
                    return (
                        <ul className="annotationList">
                            {annotationItems}
                        </ul>
                    );
                }
            });

            var AnnotationForm = React.createClass({
                utcTime: function() {
                    var d = new Date();
                    return d.getUTCFullYear() + '-' + (d.getUTCMonth() + 1) + '-' + d.getUTCDate() + 'T'
                        + d.getUTCHours() + ':' + d.getUTCMinutes() + ':' + d.getUTCSeconds() + 'Z';

                },
                handleSubmit: function(e) {
                    e.preventDefault();
                    var content = this.refs.annotationText.value.trim();
                    var start = parseFloat(this.refs.startTime.value.trim()).toFixed(2);
                    var end = 0.00;
                    if (isNaN(parseFloat(this.refs.endTime.value.trim()))) {
                        end = (parseFloat(this.refs.startTime.value.trim()) + 5.00).toFixed(2);
                    } else {
                        end = parseFloat(this.refs.endTime.value.trim()).toFixed(2);
                    }
                    if (!content || !start) {
                        // @todo validate fields;
                        return;
                    }
                    var annotation = {
                        author: user,
                        content: content,
                        targetPid: targetObjectId,
                        mediaFragmentStart: start,
                        mediaFragmentEnd: end,
                        annotatedAt: this.utcTime(),
                        targetSource: videoElement.currentSrc,
                        scope: videoElement.baseURI
                    };
                    this.props.onAnnotationSubmit(annotation);
                },
                render: function() {
                    return (
                        <form className="annotationForm form-horizontal" onSubmit={this.handleSubmit}>
                            <div className="form-group form-inline">
                                <label htmlFor="annotationStartTime" className="col-xs-2 control-label">Time</label>
                                <div className="col-xs-4">
                                    <input type="text" placeholder="Start Time" className="form-control" id="annotationStartTime" ref="startTime"/>
                                </div>
                                <label htmlFor="annotationEndTime" className="col-xs-2 control-label">End Time</label>
                                <div className="col-xs-4">
                                    <input type="text" placeholder="End Time" className="form-control" id="annotationEndTime" ref="endTime"/>
                                </div>
                            </div>
                            <div className="form-group">
                                <label htmlFor="annotationStartTime" className="col-xs-2 control-label">Annotation</label>
                                <div className="col-xs-10">
                                    <textarea placehoder="New annotation" className="form-control" id="annotationText" ref="annotationText"></textarea>
                                </div>
                            </div>
                            <div className="form-group">
                                <div className="col-xs-offset-2 col-xs-10">
                                    <button type="submit" className="btn btn-default">Submit</button>
                                </div>
                            </div>
                        </form>
                    );
                }
            });


            var AnnotationManager = React.createClass({
                render: function() {
                    return (
                        <span>
                            <button className="btn glyphicon glyphicon-pencil" />
                            <button className="btn glyphicon glyphicon-trash" />
                        </span>
                    );
                }
            });
            var Annotation = React.createClass({
                render: function() {
                    //console.log(this.state);
                    return (
                        <li className="annotationItem" data-begin={this.props.start} data-end={this.props.end} >
                            <span>
                                <button
                                    className="btn btn-default btn-xs">{this.props.start}<span className="glyphicon glyphicon-play"></span></button>
                            </span>
                            <span>{this.props.children}</span>
                            <AnnotationManager />
                        </li>
                    );
                }
            });



            ReactDOM.render(
                <AnnotationBox url={apiUrl} pollInterval={2000} />,
                document.getElementById('annotation-tab')
            );


        } // End of attach

    };
})(jQuery);

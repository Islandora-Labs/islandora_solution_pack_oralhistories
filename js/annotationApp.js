/**
 * @file
 * Annotation App js file
 */

(function ($) {
    Drupal.behaviors.annotationApp = {
        attach: function (context, settings) {
            //console.dir(Drupal.settings.islandoraOralhistories);
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;
            var enableAnnotation = Drupal.settings.islandoraOralhistories.enableAnnotationTabDisplay;
            var videoElement = $(".islandora-oralhistories-object").find('video, audio')[0];
            var apiUrl = '/islandora/object/' + targetObjectId + '/web_annotation';
            var user = Drupal.settings.islandoraOralhistories.user;
            var permissions = Drupal.settings.islandoraOralhistories.permissions;
            console.log(permissions);
            console.log(user);

            //AnnotationBox
            var AnnotationBox = React.createClass({
                displayName: 'AnnotationBox',

                loadAnnotationsFromServer: function () {
                    $.ajax({
                        url: this.props.url,
                        dataType: 'json',
                        cache: false,
                        success: (function (data) {
                            this.setState({ data: data });
                        }).bind(this),
                        error: (function (xhr, status, err) {
                            console.error(this.props.url, status, err.toString());
                        }).bind(this)
                    });
                },
                handleAnnotationSubmit: function (annotation) {
                    var arr = this.state.annotations;
                    $.ajax({
                        url: this.props.url + '/create',
                        dataType: 'json',
                        type: 'POST',
                        data: annotation,
                        success: (function (data) {
                            console.log(data);
                            if (data.response == 'success') {
                                arr.push(annotation);
                                this.setState({ annotations: arr });
                                this.setState({ adding: false }); // Hide the form after submission
                            }
                        }).bind(this),
                        error: (function (xhr, status, err) {
                            console.error(this.props.url, status, err.toString());
                        }).bind(this)
                    });
                },
                getInitialState: function () {
                    return {
                        annotations: []
                    };
                },
                componentDidMount: function () {
                    //this.loadAnnotationsFromServer();
                    //setInterval(this.loadAnnotationsFromServer, this.props.pollInterval);
                },
                addNewForm: function () {
                    this.setState({ adding: true });
                },
                renderDisplay: function () {
                    //console.dir(this.state.data, 'data from annotationbox');
                    if (JSON.parse(permissions.create)) {
                        if (!this.state.adding) {
                            return React.createElement(
                                'div',
                                { className: 'annotationBox' },
                                React.createElement(
                                    'div',
                                    { className: 'annotationToolbars' },
                                    React.createElement(
                                        'button',
                                        { onClick: this.addNewForm,
                                            className: 'btn btn-success btn-sm glyphicon glyphicon-plus' },
                                        ' ',
                                        Drupal.t('New Annotation')
                                    )
                                ),
                                React.createElement(AnnotationList, { data: this.state.annotations })
                            );
                        } else {
                            return React.createElement(
                                'div',
                                { className: 'annotationBox' },
                                React.createElement(
                                    'div',
                                    { className: 'annotationToolbars' },
                                    React.createElement(
                                        'button',
                                        { onClick: this.addNewForm,
                                            className: 'btn btn-success btn-sm glyphicon glyphicon-plus' },
                                        ' ',
                                        Drupal.t('New Annotation')
                                    )
                                ),
                                React.createElement(AnnotationForm, { onAnnotationSubmit: this.handleAnnotationSubmit }),
                                React.createElement(AnnotationList, { data: this.state.annotations })
                            );
                        }
                    } else if (JSON.parse(permissions.view)) {
                        return React.createElement(
                            'div',
                            { className: 'annotationBox' },
                            React.createElement(AnnotationList, { data: this.state.annotations })
                        );
                    }
                },
                render: function () {
                    return this.renderDisplay();
                }
            });

            //AnnotationList
            var AnnotationList = React.createClass({
                displayName: 'AnnotationList',

                nextIndex: function () {
                    this.uniqueId = this.uniqueId || 0;
                    return this.uniqueId++;
                },
                render: function () {
                    //console.dir(this.props.data);
                    var annotationItems = this.props.data.map(function (item, index) {
                        return React.createElement(
                            Annotation,
                            { index: index, author: item.author, key: index, start: item.mediaFragmentStart, end: item.mediaFragmentEnd },
                            item.content
                        );
                    });
                    return React.createElement(
                        'ul',
                        { className: 'annotationList' },
                        annotationItems
                    );
                }
            });

            var AnnotationForm = React.createClass({
                displayName: 'AnnotationForm',

                utcTime: function () {
                    var d = new Date();
                    return d.getUTCFullYear() + '-' + (d.getUTCMonth() + 1) + '-' + d.getUTCDate() + 'T' + d.getUTCHours() + ':' + d.getUTCMinutes() + ':' + d.getUTCSeconds() + 'Z';
                },
                handleSubmit: function (e) {
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
                render: function () {
                    return React.createElement(
                        'form',
                        { className: 'annotationForm form-horizontal', onSubmit: this.handleSubmit },
                        React.createElement(
                            'div',
                            { className: 'form-group form-inline' },
                            React.createElement(
                                'label',
                                { htmlFor: 'annotationStartTime', className: 'col-xs-2 control-label' },
                                'Time'
                            ),
                            React.createElement(
                                'div',
                                { className: 'col-xs-4' },
                                React.createElement('input', { type: 'text', placeholder: 'Start Time', className: 'form-control', id: 'annotationStartTime', ref: 'startTime' })
                            ),
                            React.createElement(
                                'label',
                                { htmlFor: 'annotationEndTime', className: 'col-xs-2 control-label' },
                                'End Time'
                            ),
                            React.createElement(
                                'div',
                                { className: 'col-xs-4' },
                                React.createElement('input', { type: 'text', placeholder: 'End Time', className: 'form-control', id: 'annotationEndTime', ref: 'endTime' })
                            )
                        ),
                        React.createElement(
                            'div',
                            { className: 'form-group' },
                            React.createElement(
                                'label',
                                { htmlFor: 'annotationStartTime', className: 'col-xs-2 control-label' },
                                'Annotation'
                            ),
                            React.createElement(
                                'div',
                                { className: 'col-xs-10' },
                                React.createElement('textarea', { placehoder: 'New annotation', className: 'form-control', id: 'annotationText', ref: 'annotationText' })
                            )
                        ),
                        React.createElement(
                            'div',
                            { className: 'form-group' },
                            React.createElement(
                                'div',
                                { className: 'col-xs-offset-2 col-xs-10' },
                                React.createElement(
                                    'button',
                                    { type: 'submit', className: 'btn btn-default' },
                                    'Submit'
                                )
                            )
                        )
                    );
                }
            });

            // @todo: we need consider permission for edit and delete.
            var AnnotationEditor = React.createClass({
                displayName: 'AnnotationEditor',

                render: function () {
                    return React.createElement(
                        'button',
                        { className: 'btn btn-default btn-xs' },
                        React.createElement('span', { className: 'glyphicon glyphicon-pencil' })
                    );
                }
            });
            var AnnotationRemover = React.createClass({
                displayName: 'AnnotationRemover',

                render: function () {
                    return React.createElement(
                        'button',
                        { className: 'btn btn-default btn-xs' },
                        React.createElement('span', { className: 'glyphicon glyphicon-trash' })
                    );
                }
            });
            var Annotation = React.createClass({
                displayName: 'Annotation',

                render: function () {
                    if (JSON.parse(permissions.edit_any)) {
                        if (JSON.parse(permissions.delete_any) || JSON.parse(permissions.delete_own) && user.uid == this.props.author.uid) {
                            return React.createElement(
                                'li',
                                { className: 'annotationItem', id: this.props.index, 'data-begin': this.props.start, 'data-end': this.props.end },
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(
                                        'button',
                                        {
                                            className: 'btn btn-default btn-xs' },
                                        this.props.start,
                                        React.createElement('span', { className: 'glyphicon glyphicon-play' })
                                    )
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    this.props.children
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(AnnotationEditor, null),
                                    React.createElement(AnnotationRemover, null)
                                )
                            );
                        } else {
                            return React.createElement(
                                'li',
                                { className: 'annotationItem', id: this.props.index, 'data-begin': this.props.start, 'data-end': this.props.end },
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(
                                        'button',
                                        {
                                            className: 'btn btn-default btn-xs' },
                                        this.props.start,
                                        React.createElement('span', { className: 'glyphicon glyphicon-play' })
                                    )
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    this.props.children
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(AnnotationEditor, null)
                                )
                            );
                        }
                    } else if (JSON.parse(permissions.edit_own) && user.uid == this.props.author.uid) {
                        if (JSON.parse(permissions.delete_any || JSON.parse(permissions.delete_own) && user.uid == this.props.author.uid)) {
                            return React.createElement(
                                'li',
                                { className: 'annotationItem', id: this.props.index, 'data-begin': this.props.start, 'data-end': this.props.end },
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(
                                        'button',
                                        {
                                            className: 'btn btn-default btn-xs' },
                                        this.props.start,
                                        React.createElement('span', { className: 'glyphicon glyphicon-play' })
                                    )
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    this.props.children
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(AnnotationEditor, null),
                                    React.createElement(AnnotationRemover, null)
                                )
                            );
                        } else {
                            return React.createElement(
                                'li',
                                { className: 'annotationItem', id: this.props.index, 'data-begin': this.props.start, 'data-end': this.props.end },
                                React.createElement(
                                    'span',
                                    null,
                                    React.createElement(
                                        'button',
                                        {
                                            className: 'btn btn-default btn-xs' },
                                        this.props.start,
                                        React.createElement('span', { className: 'glyphicon glyphicon-play' })
                                    )
                                ),
                                React.createElement(
                                    'span',
                                    null,
                                    this.props.children
                                )
                            );
                        }
                    }
                }
            });

            ReactDOM.render(React.createElement(AnnotationBox, { url: apiUrl, pollInterval: 2000 }), document.getElementById('annotation-tab'));
        } // End of attach

    };
})(jQuery);
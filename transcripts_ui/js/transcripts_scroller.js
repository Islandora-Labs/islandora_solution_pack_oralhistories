(function ($) {

    Drupal.settings.scrollingTranscript = [];

    ScrollingTranscript = (function () {
        var ui = [];

        function createUI($transcript) {
            var transid = $transcript.attr('data-transcripts-id');

            var html5 = {
                player: null,

                setVideo: function (element) {
                    var that = this;
                    player = element;

                    var playPause = function (e) {
                        if (!player.paused) { //if playing
                            that.checkNow(player.currentTime);
                        }
                    };

                    var timeUpdated = function (e) {
                        var now = player.currentTime;

                        //if playmode=playstop, then don't keep scrolling when you stop
                        if (!player.paused && that.one != null && now > that.one.attr('data-end')) {
                            player.pause();
                            that.lastNow = now;
                        }

                        //clean highlights and scroll
                        if (!player.paused || Math.abs(that.lastNow - now) > .2) {
                            that.checkScroll(now);
                        }
                    };

                    player.setAttribute('data-transcripts-id', that.trid);
                    player.addEventListener('play', playPause, false);
                    player.addEventListener('pause', playPause, false);
                    player.addEventListener('timeupdate', timeUpdated, false);
                    player.addEventListener('loadedmetadata', function () {
                        var jump = $.param.fragment();
                        if (jump != '') {
                            that.playOne($('#' + jump.replace('tcu/', '')));
                        }
                    });
                },

                playFrom: function (seconds) {
                    if (player != null) {
                        player.currentTime = seconds;
                        if (player.paused) player.play();
                    }
                },

                setCurrentTime: function (seconds) {
                    if (player != null) {
                        player.currentTime = seconds;
                    }
                }
            };

            var scroller = {
                trid: transid,
                container: null,
                one: null,
                sweetSpot: 0,
                resetSweet: true,
                playIndex: 0,
                startPointer: 0,
                lastNow: 0,

                starts: $('*[data-begin]', $transcript).not('.deleted').map(function (element, index) {
                    var o = {};
                    o.$item = $(this);
                    o.begin = $(this).attr('data-begin');
                    o.end = $(this).attr('data-end');
                    return o;
                }).toArray().sort(function (a, b) {
                    return a.begin - b.begin;
                }),

                ends: $('*[data-end]', $transcript).not('.deleted').map(function (element, index) {
                    var o = {};
                    o.$item = $(this);
                    o.begin = $(this).attr('data-begin');
                    o.end = $(this).attr('data-end');
                    return o;
                }).toArray().sort(function (a, b) {
                    return a.end - b.end;
                }),

                playOne: function ($item, noscroll, begin, end) {
                    var reset = this.resetSweet;

                    //to support transcript editing where times could be modified
                    if (begin === undefined) begin = $item.attr('data-begin');
                    if (end === undefined) end = $item.attr('data-end');

                    if (end-begin > 0) {
                        this.one = $item;
                        this.endAll();
                        if (reset) {
                            this.sweetSpot = $item.position().top;
                        }
                        this.playIndex = parseInt($item.attr('data-starts-index'));
                        this.playFrom(begin);
                    }
                },

                setOne: function ($tcu, noscroll) {
                    noscroll = noscroll || false;
                    this.one = $tcu;
                    this.playIndex = parseInt(this.one.attr('data-starts-index'));
                    if (!noscroll) {
                        this.startPlay($tcu); //scroll to sweet spot
                    }
                },

                checkNow: function (now) {
                    if (this.one != null && (now < parseFloat(this.one.attr('data-begin')) || now > parseFloat(this.one.attr('data-end')))) {
                    //if (this.one != null && (now < parseFloat(this.one.attr('data-begin')) - .1 || now > parseFloat(this.one.attr('data-end')) + .1)) {
                        this.one = null;
                    }
                },

                checkScroll: function (now) {
                    var that = this;
                    $('.playing', $transcript).each(function () {
                        if (now < $(this).attr('data-begin') || now > $(this).attr('data-end')) {
                            that.endPlay($(this));
                        }
                    });
                    if (now < this.lastNow) {
                        this.startPointer = 0; //go back to start
                        this.playIndex = 0;
                    }

                    while (typeof this.starts[this.startPointer] !== 'undefined' && now > this.starts[this.startPointer]['begin']) {
                        if (now < this.starts[this.startPointer]['end']) {
                            this.playIndex = this.startPointer;
                            this.startPlay(this.starts[this.startPointer].$item);
                        }
                        this.startPointer++;
                    }
                    this.lastNow = now;
                },

                setContainer: function ($container) {
                    this.container = $container;
                },

                startPlay: function ($id) {
                    $id.addClass('playing'); //sentence
                    var idTop = $id.position().top;

                    //sentence out of view above - do we ever get here?
                    if (idTop <= 0 || this.sweetSpot <= 0) {
                        this.sweetSpot = 0;
                        this.container.scrollTo($id);
                    }

                    //sentence above scroll sweet spot
                    else if (idTop < 0 || idTop < this.sweetSpot) {
                        this.container.scrollTo('-=' + (this.sweetSpot - idTop), {axis: 'y'});
                    }
                    //sentence below scroll sweet spot
                    else {
                        this.container.scrollTo('+=' + (idTop - this.sweetSpot), {axis: 'y'});

                        //sentence out of view below
                        if ($id.position().top > this.container.height() - $id.height()) {
                            this.container.scrollTo($id);
                        }
                    }
                },

                endPlay: function ($id) {
                    $id.removeClass('playing'); //sentence
                    this.sweetSpot = $id.position().top; //change sweet spot if user scrolls transcript while playing
                },

                endAll: function () {
                    var that = this;
                    $('.playing', $transcript).each(function () {
                        that.endPlay($(this));
                    });
                },

                previous: function () {
                    var n = this.playIndex > 0 ? this.playIndex - 1 : 0;
                    this.resetSweet = false; //will be set back to true after line is played
                    this.playOne($(this.starts[n].$item));
                },

                sameAgain: function () {
                    this.playOne($(this.starts[this.playIndex].$item));
                },

                next: function () {
                    var n = this.playIndex == this.starts.length - 1 ? this.playIndex : this.playIndex + 1;
                    this.resetSweet = false; //will be set back to true after line is played
                    this.playOne($(this.starts[n].$item));
                }

            };

            scroller.setContainer($transcript);
            for (var i = 0; i < scroller.starts.length; i++) {
                scroller.starts[i].$item.attr('data-starts-index', i);
            }
            $.extend(scroller, html5);

            $('button.play-tcu', $transcript).click(function () {
                var $tcu = $(this).parents('li[data-tcuid]');
                scroller.sweetSpot = $tcu.position().top;
                scroller.resetSweet = true;
                scroller.playOne($tcu);
            });

            return scroller;
        }

        return {
            getUI: function ($transcript) {
                var trid = $transcript.attr('data-transcripts-id');

                if (!ui[trid]) {
                    ui[trid] = createUI($transcript);
                }

                return ui[trid];
            }
        };
    })();

})(jQuery);

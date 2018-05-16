var express = require('express');
var router = express.Router();

var Event = require('../models/event');
var News = require('../models/news');

/* GET home page. */
router.get('/', function(req, res, next) {
    Event.find(function (err, docs) {
        var eventChunks = [];
        var chunkSize = 2;
        for (var i = 0; i < 4; i += chunkSize) {
            eventChunks.push(docs.slice(i, i + chunkSize));
        }
        res.render('index', {events: eventChunks});
    });

});

router.get('/newsFeed', function(req, res, next) {
    News.find({eventId: req.session.eventID})
        .exec(function (err, docs) {
            console.log(docs);
            var newsChunks = [];
            var chunkSize = 2;
            for (var i = 0; i < docs.length; i += chunkSize) {
                newsChunks.push(docs.slice(i, i + chunkSize));
            }
            res.render('newsFeed', {news: newsChunks, event: req.session.eventTitle, eventId: req.session.eventID, layout: 'main'});
        });

});

router.get('/newsFeed/:id/:event', function(req, res, next) {
    req.session.eventTitle = req.params.event;
    var str = req.params.id;
    req.session.eventID = str.toString();
    res.redirect('/newsFeed');
});




module.exports = router;
const express        = require('express');
const router         = express.Router();

router.use('*.css', (req, res, next) => {
    res.type('text/css');
    next();
});

router.use('*.js', (req, res, next) => {
    res.type('application/javascript');
    next();
});

router.get('/', (req, res) => {
    return res.sendFile('index.html');
});

router.get('/style.css', (req, res) => {
    return res.sendFile('style.css');
});

router.get('/script.js', (req, res) => {
    return res.sendFile('script.js');
});

module.exports = () => router;
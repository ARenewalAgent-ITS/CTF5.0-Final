const express      = require('express');
const path         = require('path');
const dotenv       = require('dotenv');
dotenv.config();
const app          = express();
const routes       = require('./routes');
const PORT         = process.env.PORT || 31337;
const HOST         = process.env.HOST || '0.0.0.0';

app.use('/', express.static(path.resolve('static')));

app.use(routes());

app.all('*', (req, res) => {
	return res.status(200).send(`${req.path} Path not found`);
});

(async () => {
	app.listen(PORT, HOST, () => console.log(`Listening on http://${HOST}:${PORT}`));
})();
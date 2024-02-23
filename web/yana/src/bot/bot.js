const puppeteer = require('puppeteer');

const CONFIG = {
    APPNAME: process.env['APPNAME'] || "Admin",
    APPURL: process.env['APPURL'] || "http://172.17.0.1",
    APPURLREGEX: process.env['APPURLREGEX'] || "^.*$",
    APPFLAG: process.env['APPFLAG'] || "dev{flag}",
    APPLIMITTIME: Number(process.env['APPLIMITTIME'] || "60"),
    APPLIMIT: Number(process.env['APPLIMIT'] || "5"),
}

console.table(CONFIG)

const initBrowser = puppeteer.launch({
    executablePath: "/usr/bin/chromium-browser",
    headless: 'new',
    args: [
        '--incognito',
        '--disable-dev-shm-usage',
        '--no-sandbox',
        '--disable-setuid-sandbox',
        '--disable-gpu',
        '--no-gpu',
        '--disable-default-apps',
        '--disable-translate',
        '--disable-device-discovery-notifications',
        '--disable-software-rasterizer',
        '--disable-xss-auditor'
    ],
    ipDataDir: '/home/bot/data/',
    ignoreHTTPSErrors: true
});

console.log("Bot started...");

module.exports = {
    name: CONFIG.APPNAME,
    urlRegex: CONFIG.APPURLREGEX,
    rateLimit: {
        windowS: CONFIG.APPLIMITTIME,
        max: CONFIG.APPLIMIT
    },
    bot: async (urlToVisit) => {
        const browser = await initBrowser;
        const context = await browser.createIncognitoBrowserContext()
        try {
            // Goto main page
            const page = await context.newPage();
            page.on('console', msg => console.log('PAGE LOG:', msg.text()));
            page.on('error', (err) => {
                console.log('error: ', err)
            })
            page.on('pageerror', (err) => {
                console.log('pageerror: ', err)
            })
            page.on('console', msg => {
                for (let i = 0; i < msg.args.length; ++i)
                  console.log(`${i}: ${msg.args[i]}`)
            })
        
            // Add flag to notes
            await page.goto(CONFIG.APPURL, {
                waitUntil: 'networkidle2'
            });

            // Add flag to notes
            console.log("bot adding flag to notes...")
            
            const flag = `<input value="${CONFIG.APPFLAG}">`
            await page.waitForSelector('#note');
            await page.type('#note', flag);
            // submit button
            await page.waitForSelector('#submit');
            await page.click('#submit');
            

            // Visit URL from user
            console.log(`bot visiting ${urlToVisit}`)
            await page.goto(urlToVisit, {
                waitUntil: 'networkidle2'
            });
            await page.waitForTimeout(5000);

            // Close
            console.log("browser close...")
            await context.close()
            return true;
        } catch (e) {
            console.error(e);
            await context.close();
            return false;
        }
    }
}

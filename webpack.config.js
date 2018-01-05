const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin')

const public = path.join(__dirname, 'public')
const resources = path.join(public, 'resources')

const deathPlugin = function () {
    this.plugin('done', stats => {
        const { errors = [] } = stats.compilation
        if (errors.length > 0) {
            errors.forEach(({ message }) => console.error(message))
            process.exit(errors.length)
        }
    })
}

const assetsPlugin = new CopyWebpackPlugin([
    { from: 'manifest.json', to: public },
    { from: 'assets/html/index.html', to: public },
    { from: 'assets/css/styles.css', to: public },
    { from: 'assets/images/rung-full.png', to: resources },
    { from: 'assets/images/rung.png', to: resources },
    { from: 'assets/images/rung16.png', to: resources },
    { from: 'assets/images/rung48.png', to: resources },
    { from: 'assets/images/rung128.png', to: resources }
]);

module.exports = {
    entry: {
        index: './lib/js/src/App.bs.js',
        observer: './lib/js/src/chrome/observer.bs.js'
    },
    output: {
        path: public,
        filename: '[name].min.js',
    },
    plugins: [
        deathPlugin,
        assetsPlugin
    ]
};

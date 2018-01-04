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

module.exports = {
    entry: './lib/js/src/index.bs.js',
    output: {
        path: public,
        filename: '[name].min.js',
    }
};

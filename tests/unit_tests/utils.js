const fs = require('fs');
const path = require('path');

const writeToFile = (buffer, name) => {
    const filePath = path.join(__dirname, name);
    fs.writeFileSync(filePath, buffer);
}

module.exports = { writeToFile };
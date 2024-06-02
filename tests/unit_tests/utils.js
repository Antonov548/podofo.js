const fs = require('fs');
const path = require('path');

const writeFile = (buffer, name) => {
  const filePath = path.join(__dirname, name);
  fs.writeFileSync(filePath, buffer);
}

const readFile = (name) => {
  const filePath = path.join(__dirname, `../test_data/${name}`);
  return fs.readFileSync(filePath);
}

module.exports = { writeFile, readFile };

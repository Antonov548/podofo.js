const Podofo = require('./podofo');

beforeAll(async () => {
  global.Podofo = await Podofo();
});
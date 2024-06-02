const Podofo = require('./podofo.node')

beforeAll(async () => {
  global.Podofo = await Podofo();
});

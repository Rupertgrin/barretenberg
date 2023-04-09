import { Fr } from '../types/index.js';
import {
  pedersenBufferToField,
  pedersenCommit,
  pedersenCompress,
  pedersenCompressFields,
  pedersenCompressWithHashIndex,
  pedersenHashInit,
  pedersenHashMultiple,
  pedersenHashMultipleWithHashIndex,
  pedersenHashPair,
  pedersenHashToTree,
  pedersenPlookupCommit,
  pedersenPlookupCompress,
  pedersenPlookupCompressFields,
} from './index.js';

describe('test generated bindings', () => {
  beforeAll(() => {
    pedersenHashInit();
  });

  it('pedersenCompressFields', () => {
    const result = pedersenCompressFields(new Fr(4n), new Fr(8n));
    expect(result).toEqual(new Fr(16672613430297770667465722499387909817686322516130512258122141976728892914370n));
  });

  it('pedersenPlookupCompressFields', () => {
    const result = pedersenPlookupCompressFields(new Fr(4n), new Fr(8n));
    expect(result).toEqual(new Fr(7508407170365331152493586290597472346478280823936748458450026785528968221772n));
  });

  it('pedersenCompress', () => {
    const result = pedersenCompress([new Fr(4n), new Fr(8n), new Fr(12n)]);
    expect(result).toEqual(new Fr(20749503715308760529311051818180468653739005441229560405092292242074298877245n));
  });

  it('pedersenPlookupCompress', () => {
    const result = pedersenPlookupCompress([new Fr(4n), new Fr(8n), new Fr(12n)]);
    expect(result).toEqual(new Fr(641613987782189905475142047603559162464012327378197326488471789040703504911n));
  });

  it('pedersenCompressWithHashIndex', () => {
    const result = pedersenCompressWithHashIndex([new Fr(4n), new Fr(8n)], 7);
    expect(result).toEqual(new Fr(21023575759217716079474317465087832600947311384619129919695080673364968932110n));
  });

  it('pedersenCommit', () => {
    const result = pedersenCommit([new Fr(4n), new Fr(8n), new Fr(12n)]);
    expect(result).toEqual(new Fr(20749503715308760529311051818180468653739005441229560405092292242074298877245n));
  });

  it('pedersenPlookupCommit', () => {
    const result = pedersenPlookupCommit([new Fr(4n), new Fr(8n)]);
    expect(result).toEqual(new Fr(7508407170365331152493586290597472346478280823936748458450026785528968221772n));
  });

  it('pedersenBufferToField', () => {
    const result = pedersenBufferToField(Buffer.from('Hello world! I am a buffer to be converted to a field!'));
    expect(result).toEqual(new Fr(4923399520610513632896240312051201308554838580477778325691012985962614653619n));
  });

  it('pedersenHashPair', () => {
    const result = pedersenHashPair(new Fr(4n), new Fr(8n));
    expect(result).toEqual(new Fr(16672613430297770667465722499387909817686322516130512258122141976728892914370n));
  });

  it('pedersenHashMultiple', () => {
    const result = pedersenHashMultiple([new Fr(4n), new Fr(8n), new Fr(12n)]);
    expect(result).toEqual(new Fr(20749503715308760529311051818180468653739005441229560405092292242074298877245n));
  });

  it('pedersenHashMultipleWithHashIndex', () => {
    const result = pedersenHashMultipleWithHashIndex([new Fr(4n), new Fr(8n)], 7);
    expect(result).toEqual(new Fr(21023575759217716079474317465087832600947311384619129919695080673364968932110n));
  });

  it('pedersenHashToTree', () => {
    const result = pedersenHashToTree([new Fr(4n), new Fr(8n), new Fr(12n), new Fr(16n)]);
    expect(result).toEqual([
      new Fr(4n),
      new Fr(8n),
      new Fr(12n),
      new Fr(16n),
      new Fr(16672613430297770667465722499387909817686322516130512258122141976728892914370n),
      new Fr(2061751319972418833696446570582220094103063223444445803798452382745574331894n),
      new Fr(14943923341655123040520893934058140700582123970938200411728845271286654065489n),
    ]);
  });
});

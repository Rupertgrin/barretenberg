import { randomBytes } from 'crypto';
import { toBigIntBE, toBufferBE } from 'bigint-buffer';
import { BufferReader } from '../serialize/index.js';

export class Fr {
  static MODULUS = 0x30644e72e131a029b85045b68181585d2833e84879b9709143e1f593f0000001n;
  static MAX_VALUE = this.MODULUS - 1n;
  static SIZE_IN_BYTES = 32;

  constructor(public readonly value: bigint) {
    if (value > Fr.MAX_VALUE) {
      throw new Error(`Fr out of range ${value}.`);
    }
  }

  static random() {
    const r = toBigIntBE(randomBytes(64)) % Fr.MODULUS;
    return new this(r);
  }

  static fromBuffer(buffer: Uint8Array | Buffer | BufferReader) {
    const reader = BufferReader.asReader(buffer);
    return new this(toBigIntBE(reader.readBytes(this.SIZE_IN_BYTES)));
  }

  static fromString(str: string) {
    return this.fromBuffer(Buffer.from(str.replace(/^0x/i, ''), 'hex'));
  }

  toBuffer() {
    return toBufferBE(this.value, Fr.SIZE_IN_BYTES);
  }

  toString() {
    return '0x' + this.value.toString(16);
  }

  equals(rhs: Fr) {
    return this.value === rhs.value;
  }

  isZero() {
    return this.value === 0n;
  }
}

export class Fq {
  static MODULUS = 0x30644e72e131a029b85045b68181585d97816a916871ca8d3c208c16d87cfd47n;
  static MAX_VALUE = this.MODULUS - 1n;
  static SIZE_IN_BYTES = 32;

  constructor(public readonly value: bigint) {
    if (value > Fq.MAX_VALUE) {
      throw new Error(`Fq out of range ${value}.`);
    }
  }

  static random() {
    const r = toBigIntBE(randomBytes(64)) % Fq.MODULUS;
    return new this(r);
  }

  static fromBuffer(buffer: Buffer | BufferReader) {
    const reader = BufferReader.asReader(buffer);
    return new this(toBigIntBE(reader.readBytes(this.SIZE_IN_BYTES)));
  }

  static fromString(str: string) {
    return this.fromBuffer(Buffer.from(str.replace(/^0x/i, ''), 'hex'));
  }

  toBuffer() {
    return toBufferBE(this.value, Fq.SIZE_IN_BYTES);
  }

  toString() {
    return '0x' + this.value.toString(16);
  }

  equals(rhs: Fq) {
    return this.value === rhs.value;
  }

  isZero() {
    return this.value === 0n;
  }
}

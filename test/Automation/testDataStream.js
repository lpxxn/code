var f = new File('e:/ds.bin');
f.open(File.ReadWrite);

var ds = new DataStream(f);
ds.writeInt8(-12).writeBool(true).writeUInt8(12).writeInt16(-1212);
ds.writeInt32(3232).writeUInt64(10000).writeInt64(-10000).writeUInt32(1234);
ds.writeString('我是中国人');
ds.writeString('Hello world');
ds.writeFloat(12.123);
ds.writeDouble(12.123);

f.seek(0);
log(ds.readInt8());
log(ds.readBool());
log(ds.readUInt8());
log(ds.readInt16());

log(ds.readInt32());
log(ds.readUInt64());
log(ds.readInt64());
log(ds.readUInt32());

log(ds.readString());
log(ds.readString());
log(ds.readFloat());
log(ds.readDouble());

log(ds.version());

f.close();

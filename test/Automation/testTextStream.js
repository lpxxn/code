var buf = new Buffer('abcdefg\n我是中国人');

//buf.writeString('abcdefg');
//buf.writeString('我是一个中国人');

log("buffer hex:", buf.toHexString());
log("buffer value:", buf.valueOf());

var ts = new TextStream(buf, File.ReadOnly);
ts.setCodec('utf-8');
log(ts.codecName());
log(ts.readLine());
log(ts.readLine());
log(ts.readLine());

var f = new File('e:/abc.txt');
f.open(File.WriteOnly);
f.write(buf);
log(f.writeString('abcd'));
log(f.writeString('中国人'));
log(f.writeString('efg'));

var ts2 = new TextStream(f);
ts2.writeNumber(12, 12.12, -123, -23.2323, 34);
ts2.writeString('abc', 'def g', -23.23, 11111, 3333, '我是中国人');
ts2.writeBuffer(buf);

f.close();
